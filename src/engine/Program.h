/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Shader.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <map>

class AssetLoader;

/**
 * A GLSL shader program
 */
class Program
{
	public:
		/**
		 * Construct a new program a collection of shaders
		 */
		Program(std::vector<boost::shared_ptr<Shader>> & theShaders);
		Program(unsigned int shaderTypes, const std::string & name, boost::shared_ptr<AssetLoader> loader);
		~Program();

		/**
		 * Start using the program
		 */
		void enable();

		/**
		 * Stop using the program
		 */
		void disable();

		/**
		 * Get the id of a uniform shader program variable
		 *
		 */
		unsigned int uniform(const std::string & name);

	protected:
		void shaders(std::vector<boost::shared_ptr<Shader>> & theShaders);

	private:
		unsigned int id_;
		std::map<std::string, unsigned int> uniforms_;
		bool enabled_;
};
