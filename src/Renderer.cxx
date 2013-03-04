/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Renderer.h"
#include "Program.h"
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <log4cxx/logger.h>


Renderer::Renderer(boost::shared_ptr<Scene> scene, AssetLoader loader) : 
	scene_(scene)
{
	// setup shaders
	std::vector<boost::shared_ptr<Shader>> shaders;
	boost::shared_ptr<Shader> shader;

	std::string script;
	script = loader.load("shader_v2.vert");
	shader.reset(new Shader (GL_VERTEX_SHADER, script));
	shaders.push_back(shader);

	script = loader.load("shader_v2.frag");
	shader.reset(new Shader(GL_FRAGMENT_SHADER, script));
	shaders.push_back(shader);

	program_.reset(new Program(shaders));

	// setup shader program uniforms
	program_->enable();

	// camera & model matrices
	glm::mat4 model(1.0f);
	unsigned int modelMatrix = program_->uniform("modelMatrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

	// lighting
	/*
	unsigned int lightIntensity = program_->uniform("lightIntensity");
	unsigned int ambientIntensity = program_->uniform("ambientIntensity");

	glUniform4f(lightIntensity, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(ambientIntensity, 0.2f, 0.2f, 0.2f, 1.0f);
	*/
	Light light(
		glm::vec4(0.0f, 10.0f, 0.0f, 1.0f), // position
		glm::vec3(0.8f, 0.8f, 0.8f), // ambient
		glm::vec3(0.3f, 0.3f, 0.3f), // diffuse
		glm::vec3(0.2f, 0.2f, 0.2f) // specular
	);
	light.program(program_);

	Material material(
		glm::vec3(0.5f, 0.5f, 0.5f), // ambient
		glm::vec3(0.4f, 0.6f, 0.9f), // diffuse
		glm::vec3(0.3f, 0.3f, 0.3f), // specular
		0.5f // shininess
	);
	material.program(program_);

	program_->disable();

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	boost::shared_ptr<Mesh> mesh = scene_->mesh();
	unsigned int vbo = createVertexBuffer(mesh);
	mesh->addBuffer("vbo", vbo);

	unsigned int ebo = createIndexBuffer(mesh->tris());
	mesh->addBuffer("ebo", ebo);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
}


unsigned int Renderer::createIndexBuffer(const std::vector<unsigned int> & data)
{
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	// NB - ebo's are bound to the vao so the vao must already be bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ebo;
}

unsigned int Renderer::createVertexBuffer(boost::shared_ptr<Mesh> mesh)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<Vertex> & vertices = mesh->vertices();
	std::vector<Color> & colors = mesh->colors();
	std::vector<Normal> & normals = mesh->normals();

	size_t vertexSize = sizeof(Vertex) * vertices.size();
	size_t colorSize = sizeof(Color) * colors.size();
	size_t normalSize = sizeof(Normal) * normals.size();

	size_t bufferSize = vertexSize + colorSize + normalSize;

	// allocate buffer
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	// fill buffer with mesh data
	unsigned int offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertexSize, &vertices[0]);
	offset += vertexSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, colorSize, &colors[0]);
	offset += colorSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, normalSize, &normals[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vbo;
}


void Renderer::draw(Hookah::Window * window)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program_->enable();

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	// [TODO] - these uniforms only need changed with the underlying data changes
	// e.g., no need to update the view matrix if the camera's view hasn't changed since the last frame
	unsigned int viewMatrix = program_->uniform("viewMatrix");
	glm::mat4 view;
	view = scene_->camera()->view();
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view));

	// lighting
	/*
	unsigned int dirToLight = program_->uniform("dirToLight");
	glm::vec4 lightDirection(0.0f, 1.0f, 0.0f, 0.0f);
	*/
	// world-to-camera matrix is the transpose inverse of the view matrix
	glm::mat4 worldToCam = glm::transpose(glm::inverse(view));
	unsigned int normalMatrix = program_->uniform("normalMatrix");
	glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(worldToCam));
	/*
	glm::vec4 lightDirCameraSpace = worldToCam * lightDirection;
	glUniform3fv(dirToLight, 1, glm::value_ptr(lightDirCameraSpace));
	*/

	drawMesh(scene_->mesh());

	program_->disable();
}


void Renderer::drawMesh(boost::shared_ptr<Mesh> mesh)
{
	unsigned int ebo = mesh->buffer("ebo");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	unsigned int vbo = mesh->buffer("vbo");
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// vertex attributes
	glEnableVertexAttribArray(0);
	// color attributes
	glEnableVertexAttribArray(1);
	// normal attributes
	glEnableVertexAttribArray(2);

	size_t colorData = mesh->vertices().size() * sizeof(Vertex);
	size_t normalData = colorData + (mesh->colors().size() * sizeof(Color));
	// attribute, # of elements, element type, transpose?, offset between elements, offset of first element
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Color), reinterpret_cast<void*>(colorData));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), reinterpret_cast<void*>(normalData));

	// a single color value is used for entire cubes and only one normal for every pair of tris (per cube face)
	glVertexAttribDivisor(1, 36);
	glVertexAttribDivisor(2, 6);

	unsigned int indices = mesh->tris().size();
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


void Renderer::resize(int width, int height)
{
	glViewport(0, 0, width, height);

	// update the aspect ratio of the camera & reload the projection matrix
	scene_->camera()->perspective(
		90.0f, // x fov
		static_cast<float>(width) / static_cast<float>(height), // aspect
		0.1f, // near
		1000.0f // far
	);
	glm::mat4 projection;
	projection = scene_->camera()->projection();
	unsigned int projectionMatrix = program_->uniform("projectionMatrix");
	// location, count, transpose, data
	program_->enable();
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
	program_->disable();

}
