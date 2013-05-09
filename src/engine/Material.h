/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <gl/Program.h>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class Material
{
	public:
		Material(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, float shininess);

		void program(boost::shared_ptr<v3D::Program> program);
		void uniform(const std::string & name);

	private:
		glm::vec3 ambient_;
		glm::vec3 specular_;
		glm::vec3 diffuse_;  // material color
		float shininess_;
		std::string name_;
};
