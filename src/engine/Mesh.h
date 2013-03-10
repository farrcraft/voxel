/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <map>

class Mesh
{
	public:
		Mesh();

		unsigned int addVertex(const glm::vec3 & vertex);
		void addTri(unsigned int a, unsigned int b, unsigned int c);
		void addNormal(const glm::vec3 & normal);

		unsigned int findVertex(const glm::vec3 & vertex);

		std::vector<glm::vec3> & vertices();
		std::vector<unsigned int> & tris();
		std::vector<glm::vec3> & normals();

	private:
		std::vector<unsigned int> tris_;
		std::vector<glm::vec3> vertices_;
		std::vector<glm::vec3> normals_;
};