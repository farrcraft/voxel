/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>

#include <list>
#include <string>

class Mesh
{
	public:
		Mesh();

		unsigned int addVertex(const glm::vec3 & vertex);
		void addTri(unsigned int a, unsigned int b, unsigned int c);
		void addNormal(const glm::vec3 & normal);

		unsigned int findVertex(const glm::vec3 & vertex);

		unsigned int addVertex(const glm::vec3 & vertex, const glm::vec3 & normal);
		unsigned int findVertex(const glm::vec3 & vertex, const glm::vec3 & normal);

		std::list<glm::vec3> & vertices();
		std::list<glm::ivec3> & tris();
		std::list<glm::vec3> & normals();

	private:
		std::list<glm::ivec3> tris_;
		std::list<glm::vec3> vertices_;
		std::list<glm::vec3> normals_;
};