/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <string>

/**
 * A GLSL shader
 */
class Shader
{
	public:
		/**
		 * Create a new GLSL shader
		 */
		Shader(unsigned int type, const std::string & shader);
		~Shader();

		/**
		 * Attach the shader to a shader program
		 */
		void attach(unsigned int program);
		/**
		 * Detach the shader from a shader program
		 */
		void detach(unsigned int program);

	private:
		unsigned int type_;
		unsigned int id_;
};