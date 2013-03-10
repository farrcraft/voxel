/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

class Program;

class Light
{
	public:
		Light(const glm::vec4 & position, const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular);

		void program(boost::shared_ptr<Program> program);

	private:
		glm::vec4 position_;
		glm::vec3 ambient_;
		glm::vec3 diffuse_;
		glm::vec3 specular_;
};