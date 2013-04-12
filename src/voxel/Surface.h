/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <boost/unordered_map.hpp>
#include <glm/glm.hpp>

#include <list>

class Voxel;

/**
 * Polygon surface data extracted from a Voxel that can be passed to a vertex buffer object
 */
class Surface
{
	public:
		void extract(const boost::shared_ptr<Voxel> & voxel, unsigned int faces);

		const boost::unordered_map<unsigned int, glm::vec3> & vertices() const;
		const boost::unordered_map<unsigned int, glm::ivec3> & tris() const;
		const std::list<glm::ivec4> & faces() const;

	protected:
		unsigned int addVertex(const glm::vec3 & point);
		unsigned int addTri(const glm::ivec3 & vertices);
		void addFace(unsigned int leftTri, unsigned int rightTri, unsigned int faceType, unsigned int blockType);

		void createFace(const boost::shared_ptr<Voxel> & voxel, unsigned int drawFaces, unsigned int inFaces, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3);

	private:
		boost::unordered_map<unsigned int, glm::vec3> vertices_; // unique vertices hashed by morton code
		boost::unordered_map<unsigned int, glm::ivec3> tris_;	 // unique tris hashed by morton code - tri indices refer to the morton code hash values in the vertices map
		std::list<glm::ivec4> faces_; // this is the actual data used for building the vertex buffer. each list element stores <tri hash, block type, face type>
};