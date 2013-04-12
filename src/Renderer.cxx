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
#include "voxel/MeshBuilder.h"
#include "voxel/ChunkBufferPool.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <log4cxx/logger.h>


Renderer::Renderer(boost::shared_ptr<Scene> & scene, boost::shared_ptr<AssetLoader> & loader) : 
	scene_(scene),
	debug_(false),
	builder_(scene->chunks())
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

	// setup shaders
	std::string shaderName;
	shaderName = "shaders/voxel_ads";
	boost::shared_ptr<Program> voxelProgram(new Program(Shader::SHADER_TYPE_VERTEX|Shader::SHADER_TYPE_FRAGMENT, shaderName, loader));

	// setup shader program uniforms
	voxelProgram->enable();

	// camera & model matrices
	glm::mat4 model(1.0f);
	unsigned int modelMatrix = voxelProgram->uniform("modelMatrix");
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(model));

	// lighting
	//glm::mat4 view = scene_->camera()->view();
	//glm::mat4 worldToCam = glm::transpose(glm::inverse(view));
	glm::vec4 pos(50.0f, 0.0f, 0.0f, 1.0f);
	//pos = worldToCam * pos;
	/*
	msg << " light position: " << glm::to_string(pos) << std::endl;
	LOG4CXX_INFO(logger, msg.str());
	*/
	Light light(
		pos, // position
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

	voxelProgram->disable();
	programs_["voxel"] = voxelProgram;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// build mesh data from world chunks
	//MeshBuilder generator(scene_->chunks());
	//pool_ = generator.build();
	pool_.reset(new ChunkBufferPool());

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
	boost::shared_ptr<Program> textProgram(new Program(Shader::SHADER_TYPE_VERTEX|Shader::SHADER_TYPE_FRAGMENT, "shaders/text", loader));
	programs_["text"] = textProgram;

	debugOverlay_.reset(new DebugOverlay(scene_, textProgram, loader));
}


void Renderer::draw(Hookah::Window * window)
{
	glClearColor(0.4f, 0.6f, 0.9f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	boost::shared_ptr<Program> program = programs_["voxel"];
	program->enable();
	
	//glVertexAttribDivisor(1, 0);
	//glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

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
		/*
		glm::vec4 sunPos(0.0f, 500.0f, 0.0f, 1.0f);
		sunPos = worldToCam * sunPos;
		unsigned int lightPosition = program->uniform("Light.Position");
		glUniform4fv(lightPosition, 1, glm::value_ptr(sunPos));
		*/
		scene_->camera()->dirty(false);
	}

	pool_->render();

	program->disable();

	// render text
	if (debug_)
	{
		debugOverlay_->render();
	}
}

void Renderer::tick(unsigned int delta)
{
	builder_.build(pool_, 16);
	if (debug_)
	{
		debugOverlay_->update(delta);
	}
}

void Renderer::resize(int width, int height)
{
	glViewport(0, 0, width, height);

	float w = static_cast<float>(width);
	float h = static_cast<float>(height);
	// update the aspect ratio of the camera & reload the projection matrix
	scene_->camera()->perspective(
		90.0f,  // x fov
		w / h,  // aspect
		0.1f,	// near
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

	// update orthographic view matrix used for text rendering
	boost::shared_ptr<Program> textProgram = programs_["text"];
	textProgram->enable();
	unsigned int MVPMatrix = textProgram->uniform("MVPMatrix");
	glm::mat4 mvp = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(MVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
	textProgram->disable();
}

void Renderer::debug(bool status)
{
	debug_ = status;
	debugOverlay_->enable(status);
}
