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

#include <3dtypes/Matrix4.h>

#include <GL/glew.h>

#include <log4cxx/logger.h>


Renderer::Renderer(boost::shared_ptr<Scene> scene, AssetLoader loader) : 
	scene_(scene)
{
	// setup shaders
	std::vector<boost::shared_ptr<Shader>> shaders;
	boost::shared_ptr<Shader> shader;

	std::string script;
	script = loader.load("shader_v1.vert");
	shader.reset(new Shader (GL_VERTEX_SHADER, script));
	shaders.push_back(shader);

	script = loader.load("shader.frag");
	shader.reset(new Shader(GL_FRAGMENT_SHADER, script));
	shaders.push_back(shader);

	program_.reset(new Program(shaders));

	// setup shader program uniforms
	program_->enable();

	// camera & model matrices
	v3D::Matrix4 projection;
	projection = scene_->camera()->projection();
	unsigned int projectionMatrix = program_->uniform("projectionMatrix");
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, *projection);

	v3D::Matrix4 model;
	model.identity();
	unsigned int modelMatrix = program_->uniform("modelMatrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, *model);

	// lighting
	unsigned int dirToLight = program_->uniform("dirToLight");
	unsigned int lightIntensity = program_->uniform("lightIntensity");
	unsigned int ambientIntensity = program_->uniform("ambientIntensity");

	glUniform4f(lightIntensity, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(ambientIntensity, 0.2f, 0.2f, 0.2f, 1.0f);
	/*
	glm::vec4 g_lightDirection(0.866f, 0.5f, 0.0f, 0.0f);
	glutil::MatrixStack modelMatrix;
	// CalcMatrix() generates a world-to-camera matrix
	modelMatrix.SetMatrix(g_viewPole.CalcMatrix());
	glm::vec4 lightDirCameraSpace = modelMatrix.Top() * g_lightDirection;
	glUniform3fv(dirToLight, 1, glm::value_ptr(lightDirCameraSpace));
	*/
	v3D::Vector3 lightDirCameraSpace(0.0f, 1.0f, 0.0f);
	glUniform3fv(dirToLight, 1, *lightDirCameraSpace);

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

	unsigned int viewMatrix = program_->uniform("viewMatrix");
	v3D::Matrix4 view;
	scene_->camera()->createView();
	view = scene_->camera()->view();
	glUniformMatrix4fv(viewMatrix, 1, GL_TRUE, *view);

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
}
