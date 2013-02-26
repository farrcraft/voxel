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
	glUniform3fv(dirToLight, 1, glm::value_ptr(lightDirCameraSpace));

	program_->disable();

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	Mesh & mesh = scene_->mesh();
	unsigned int vbo = createVertexBuffer(mesh.vertices());
	mesh.addBuffer("vbo", vbo);

	unsigned int ebo = createIndexBuffer(mesh.tris());
	mesh.addBuffer("ebo", ebo);

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

unsigned int Renderer::createVertexBuffer(const std::vector<Vertex> & data)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// [TODO] - initial setup should pass NULL for the data instead of the actual data
	// use calls to glBufferSubData() to supply actual data - 
	// 3 calls to supply each set of data for vertices, normals, colors individually
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &data[0], GL_STATIC_DRAW);
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

	Mesh & mesh = scene_->mesh();
	drawMesh(mesh.buffer("ebo"), mesh.buffer("vbo"), mesh.tris().size());

	program_->disable();
}


void Renderer::drawMesh(unsigned int ebo, unsigned int vbo, unsigned int indices)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// vertex attributes
	glEnableVertexAttribArray(0);
	// color attributes
	glEnableVertexAttribArray(1);

	size_t colorData = sizeof(Vertex) / 2;
	// attribute, # of elements, element type, transpose?, offset between elements, offset of first element
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(colorData));

	// [TODO] - call glVertexAttribDivisor() for attrib pointers #1 & #2 (colors & normals)
	// a single color value is used for entire cubes and only one normal for every pair of tris (per cube face)
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::drawVoxel(const Voxel & voxel)
{
}

void Renderer::resize(int width, int height)
{
	glViewport(0, 0, width, height);
}
