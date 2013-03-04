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

class Material
{
	public:
		Material(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess);

		void program(boost::shared_ptr<Program> program);

	private:
		glm::vec3 ambient_;
		glm::vec3 specular_;
		glm::vec3 diffuse_;
		float shininess_;
};
