/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Renderer.h"

#include "AssetLoader.h"
#include "Scene.h"
#include "DebugOverlay.h"

#include "engine/Program.h"
#include "engine/Camera.h"
#include "engine/Light.h"
#include "engine/Material.h"
#include "engine/VertexBuffer.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <log4cxx/logger.h>


Renderer::Renderer(boost::shared_ptr<Scene> scene, boost::shared_ptr<AssetLoader> loader) : 
	scene_(scene),
	debug_(false)
{
	// log GL version info
	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
	std::stringstream msg;
	const GLubyte * renderer = glGetString(GL_RENDERER);
	const GLubyte * vendor = glGetString(GL_VENDOR);
	const GLubyte * version = glGetString(GL_VERSION);
	const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	msg << "Renderer: " << renderer << std::endl;
	msg << " Vendor: " << vendor << std::endl;
	msg << " GL Version: " << version << std::endl;
	msg << " GLSL: " << glslVersion << std::endl;
	LOG4CXX_INFO(logger, msg.str());

	// use simple diffuse or ADS lighting?
	bool adsLighting = true;
	std::string shaderName;

	if (adsLighting)
	{
		shaderName = "shaders/voxel_ads";
	}
	else
	{
		shaderName = "shaders/voxel_diffuse";
	}

	// setup shaders
	boost::shared_ptr<Program> voxelProgram = createProgram(Shader::SHADER_TYPE_VERTEX|Shader::SHADER_TYPE_FRAGMENT, shaderName, loader);

	// setup shader program uniforms
	voxelProgram->enable();

	// camera & model matrices
	glm::mat4 model(1.0f);
	unsigned int modelMatrix = voxelProgram->uniform("modelMatrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

	// lighting
	if (adsLighting)
	{
		glm::mat4 view = scene_->camera()->view();
		glm::vec4 pos(0.0f, 100.0f, 0.0f, 1.0f);
		Light light(
			view * pos, // position
			glm::vec3(0.4f, 0.4f, 0.4f), // ambient
			glm::vec3(1.0f, 1.0f, 1.0f), // diffuse
			glm::vec3(1.0f, 1.0f, 1.0f) // specular
		);
		light.program(voxelProgram);

		Material material(
			glm::vec3(0.9f, 0.5f, 0.3f), // ambient
			glm::vec3(0.9f, 0.5f, 0.3f), // diffuse
			glm::vec3(0.8f, 0.8f, 0.8f), // specular
			100.0f // shininess
		);
		material.program(voxelProgram);
	}
	else
	{
		// simple diffuse shading
		// uniforms:
		// LightPosition (in eye coordinates)
		// Kd - diffuse reflectivity
		// Ld - light source intensity
		unsigned int LightPosition = voxelProgram->uniform("LightPosition");
		unsigned int Ld = voxelProgram->uniform("Ld");
		unsigned int Kd = voxelProgram->uniform("Kd");

		glUniform4fv(LightPosition, 1, glm::value_ptr(glm::vec4(0.0f, 100.0f, 0.0f, 1.0f)));
		glUniform3fv(Ld, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform3fv(Kd, 1, glm::value_ptr(glm::vec3(0.6f, 0.5f, 0.2f)));
	}

	voxelProgram->disable();
	programs_["voxel"] = voxelProgram;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	boost::shared_ptr<Mesh> mesh = scene_->mesh();
	buffer_.reset(new VertexBuffer(mesh));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);
	// CCW winding is default
	glFrontFace(GL_CCW);

	// setup the shader program for text rendering
	boost::shared_ptr<Program> textProgram = createProgram(Shader::SHADER_TYPE_VERTEX|Shader::SHADER_TYPE_FRAGMENT, "shaders/text", loader);
	programs_["text"] = textProgram;

	debugOverlay_.reset(new DebugOverlay(scene_, textProgram, loader));
}

boost::shared_ptr<Program> Renderer::createProgram(unsigned int shaderTypes, const std::string & name, boost::shared_ptr<AssetLoader> loader)
{
	std::vector<boost::shared_ptr<Shader>> shaders;
	boost::shared_ptr<Program> program;
	std::string script;
	boost::shared_ptr<Shader> shader;

	if (shaderTypes & Shader::SHADER_TYPE_VERTEX)
	{
		std::string filename = name + std::string(".vert");
		script = loader->load(filename);
		shader.reset(new Shader(Shader::SHADER_TYPE_VERTEX, script));
		shaders.push_back(shader);
	}

	if (shaderTypes & Shader::SHADER_TYPE_FRAGMENT)
	{
		std::string filename = name + std::string(".frag");
		script = loader->load(filename);
		shader.reset(new Shader( Shader::SHADER_TYPE_FRAGMENT, script));
		shaders.push_back(shader);
	}
	program.reset(new Program(shaders));
	return program;
}

void Renderer::draw(Hookah::Window * window)
{
	glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	boost::shared_ptr<Program> program = programs_["voxel"];
	program->enable();
	
	glVertexAttribDivisor(1, 0);

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	if (scene_->camera()->dirty())
	{
		unsigned int viewMatrix = program->uniform("viewMatrix");
		glm::mat4 view;
		view = scene_->camera()->view();
		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view));

		// world-to-camera matrix is the transpose inverse of the view matrix
		glm::mat4 worldToCam = glm::transpose(glm::inverse(view));
		unsigned int normalMatrix = program->uniform("normalMatrix");
		glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(worldToCam));

		scene_->camera()->dirty(false);
	}

	buffer_->render();

	program->disable();

	// render text
	if (debug_)
	{
		debugOverlay_->render();
	}
}

void Renderer::tick(unsigned int delta)
{
	if (debug_)
	{
		debugOverlay_->update(delta);
	}
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
	boost::shared_ptr<Program> voxelProgram = programs_["voxel"];
	voxelProgram->enable();
	unsigned int projectionMatrix = voxelProgram->uniform("projectionMatrix");
	// location, count, transpose, data
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
	voxelProgram->disable();

	boost::shared_ptr<Program> textProgram = programs_["text"];
	textProgram->enable();
	unsigned int MVPMatrix = textProgram->uniform("MVPMatrix");
	glm::mat4 mvp = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(MVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
	textProgram->disable();
}

void Renderer::debug(bool status)
{
	debug_ = status;
	debugOverlay_->enable(status);
}
