/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Renderer.h"
#include "AssetLoader.h"
#include "engine/Program.h"
#include "Scene.h"
#include "engine/Camera.h"
#include "engine/Light.h"
#include "engine/Material.h"
#include "engine/VertexBuffer.h"

#include "font/TextBuffer.h"
#include "font/TextureFont.h"
#include "font/FontCache.h"
#include "font/TextureAtlas.h"

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <log4cxx/logger.h>


Renderer::Renderer(boost::shared_ptr<Scene> scene, boost::shared_ptr<AssetLoader> loader) : 
	scene_(scene)
{
	// setup shaders
	boost::shared_ptr<Program> voxelProgram;

	std::vector<boost::shared_ptr<Shader>> shaders;
	boost::shared_ptr<Shader> shader;

	std::string script;
	script = loader->load("shader_v2.vert");
	shader.reset(new Shader (GL_VERTEX_SHADER, script));
	shaders.push_back(shader);

	script = loader->load("shader_v2.frag");
	shader.reset(new Shader(GL_FRAGMENT_SHADER, script));
	shaders.push_back(shader);

	voxelProgram.reset(new Program(shaders));

	// setup shader program uniforms
	voxelProgram->enable();

	// camera & model matrices
	glm::mat4 model(1.0f);
	unsigned int modelMatrix = voxelProgram->uniform("modelMatrix");
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
	light.program(voxelProgram);

	Material material(
		glm::vec3(0.5f, 0.5f, 0.5f), // ambient
		glm::vec3(0.8f, 0.8f, 0.8f), // diffuse
		glm::vec3(0.4f, 0.6f, 0.9f), // specular
		0.5f // shininess
	);
	material.program(voxelProgram);

	voxelProgram->disable();
	programs_["voxel"] = voxelProgram;

	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	boost::shared_ptr<Mesh> mesh = scene_->mesh();
	buffer_.reset(new VertexBuffer(mesh));

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// CCW winding is default
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_DEPTH_CLAMP);

	// setup the shader program for text rendering
	std::vector<boost::shared_ptr<Shader>> textShaders;
	boost::shared_ptr<Program> textProgram;

	script = loader->load("text.vert");
	shader.reset(new Shader (GL_VERTEX_SHADER, script));
	textShaders.push_back(shader);

	script = loader->load("text.frag");
	shader.reset(new Shader(GL_FRAGMENT_SHADER, script));
	textShaders.push_back(shader);

	textProgram.reset(new Program(textShaders));
	programs_["text"] = textProgram;

	// setup text buffer
	text_.reset(new TextBuffer(textProgram, TextBuffer::LCD_FILTERING_ON));
	TextBuffer::Markup markup;
	markup.bold_ = false;
	markup.italic_ = false;
	markup.rise_ = 0.0f;
	markup.spacing_ = 0.0f;
	markup.gamma_ = 1.5f;
	markup.underline_ = false;
	markup.overline_ = false;
	markup.strikethrough_ = false;
	markup.foregroundColor_ = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	markup.backgroundColor_ = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	markup.size_ = 24.0f;
	std::string filename = loader->path() + std::string("Vera.ttf");
	markup.font_.reset(new TextureFont(text_->cache()->atlas(), filename, markup.size_));
	// characters to cache
    const wchar_t *cache = L" !\"#$%&'()*+,-./0123456789:;<=>?"
                           L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                           L"`abcdefghijklmnopqrstuvwxyz{|}~";
	markup.font_->loadGlyphs(cache);

	glm::vec2 pen(20.0f, 200.0f);
	text_->addText(pen, markup, L"Now is the time for all good men");
	text_->upload();
	std::string atlas("atlas.bmp");
	text_->cache()->atlas()->write(atlas);
}


void Renderer::draw(Hookah::Window * window)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	boost::shared_ptr<Program> program = programs_["voxel"];
	program->enable();

	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

	// [TODO] - these uniforms only need changed with the underlying data changes
	// e.g., no need to update the view matrix if the camera's view hasn't changed since the last frame
	unsigned int viewMatrix = program->uniform("viewMatrix");
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
	unsigned int normalMatrix = program->uniform("normalMatrix");
	glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(worldToCam));
	/*
	glm::vec4 lightDirCameraSpace = worldToCam * lightDirection;
	glUniform3fv(dirToLight, 1, glm::value_ptr(lightDirCameraSpace));
	*/

	buffer_->render();

	program->disable();

	// render text
	text_->render();
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
	glm::mat4 mvp = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
	glUniformMatrix4fv(MVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
	textProgram->disable();

}
