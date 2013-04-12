/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <list>
#include <string>

class Voxel;

class Mesh
{
	public:
		Mesh();

		unsigned int addTri(unsigned int a, unsigned int b, unsigned int c);
		unsigned int addVertex(const glm::vec3 & vertex);

		void extract(const boost::shared_ptr<Voxel> & voxel, unsigned int faces);
		void createFace(unsigned int type, unsigned int drawFaces, unsigned int inFaces, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3);

		boost::unordered_map<unsigned int, glm::vec3> & vertices();
		boost::unordered_map<unsigned int, glm::ivec3> & tris();
		boost::unordered_map<unsigned int, glm::ivec4> & faces();

	private:
		boost::unordered_map<unsigned int, glm::vec3> vertices_;
		boost::unordered_map<unsigned int, glm::ivec3> tris_;
		boost::unordered_map<unsigned int, glm::ivec4> faces_;
};