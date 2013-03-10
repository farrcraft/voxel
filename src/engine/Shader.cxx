/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Shader.h"

#include <GL/glew.h>

#include <log4cxx/logger.h>

#include <fstream>
#include <iostream>

Shader::Shader(unsigned int type, const std::string & shader) : 
	type_(type), 
	id_(0)
{
	id_ = glCreateShader(type_);
	const char *data = shader.c_str();
	glShaderSource(id_, 1, &data, NULL);

	glCompileShader(id_);

	GLint status;
	glGetShaderiv(id_, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(id_, infoLogLength, NULL, infoLog);

		std::string shaderType;
		switch(type)
		{
			case GL_VERTEX_SHADER: 
				shaderType = "vertex"; 
				break;
			case GL_GEOMETRY_SHADER: 
				shaderType = "geometry"; 
				break;
			case GL_FRAGMENT_SHADER: 
				shaderType = "fragment"; 
				break;
		}

		std::string msg = std::string("Compile failure in ") + shaderType + std::string(" shader: ") + std::string(infoLog);
		delete[] infoLog;
		log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("voxel.log"));
		LOG4CXX_ERROR(logger, msg);
		throw std::runtime_error(msg);
	}
}

Shader::~Shader()
{
	glDeleteShader(id_);
}

void Shader::attach(unsigned int program)
{
	glAttachShader(program, id_);
}

void Shader::detach(unsigned int program)
{
	glDetachShader(program, id_);
}

