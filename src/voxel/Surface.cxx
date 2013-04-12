/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Surface.h"
#include "../engine/MortonCode.h"
#include "Voxel.h"


const boost::unordered_map<unsigned int, glm::vec3> & Surface::vertices() const
{
	return vertices_;
}

const boost::unordered_map<unsigned int, glm::ivec3> & Surface::tris() const
{
	return tris_;
}

const std::list<glm::ivec4> & Surface::faces() const
{
	return faces_;
}

unsigned int Surface::addVertex(const glm::vec3 & point)
{
	MortonCode codec;
	unsigned int hash = codec.encode(glm::ivec3(
		static_cast<unsigned int>(point.x),
		static_cast<unsigned int>(point.y),
		static_cast<unsigned int>(point.z)
	));
	if (vertices_.find(hash) == vertices_.end())
	{
		vertices_[hash] = point;
	}
	return hash;
}

unsigned int Surface::addTri(const glm::ivec3 & vertices)
{
	MortonCode codec;
	unsigned int hash = codec.encode(vertices);
	if (tris_.find(hash) == tris_.end())
	{
		tris_[hash] = vertices;
	}
	return hash;
}

void Surface::addFace(unsigned int leftTri, unsigned int rightTri, unsigned int faceType, unsigned int blockType)
{
	faces_.push_back(glm::ivec4(leftTri, rightTri, faceType, blockType));
}


void Surface::extract(const boost::shared_ptr<Voxel> & voxel, unsigned int faces)
{
	/*
	if (!voxel->active() || (faces & Voxel::BLOCK_FACE_NONE))
	{
		return;
	}
	*/
	if (faces & Voxel::BLOCK_FACE_ALL)
	{
		faces = Voxel::BLOCK_FACE_FRONT|Voxel::BLOCK_FACE_BACK|Voxel::BLOCK_FACE_LEFT|Voxel::BLOCK_FACE_RIGHT|Voxel::BLOCK_FACE_TOP|Voxel::BLOCK_FACE_BOTTOM;
	}

	// front
	glm::vec3 p0(0.0f, 0.0f, 1.0f);
	glm::vec3 p1(1.0f, 0.0f, 1.0f);
	glm::vec3 p2(1.0f, 1.0f, 1.0f);
	glm::vec3 p3(0.0f, 1.0f, 1.0f);
	// back
	glm::vec3 p4(0.0f, 0.0f, 0.0f);
	glm::vec3 p5(1.0f, 0.0f, 0.0f);
	glm::vec3 p6(1.0f, 1.0f, 0.0f);
	glm::vec3 p7(0.0f, 1.0f, 0.0f);

	glm::vec3 position = voxel->position();

	createFace(voxel, faces, Voxel::BLOCK_FACE_FRONT, 
		p0 + position, 
		p1 + position, 
		p2 + position, 
		p3 + position);

	createFace(voxel, faces, Voxel::BLOCK_FACE_RIGHT, 
		p1 + position, 
		p5 + position, 
		p6 + position, 
		p2 + position);

	createFace(voxel, faces, Voxel::BLOCK_FACE_TOP, 
		p2 + position, 
		p6 + position, 
		p7 + position, 
		p3 + position);

	createFace(voxel, faces, Voxel::BLOCK_FACE_LEFT, 
		p4 + position, 
		p0 + position, 
		p3 + position, 
		p7 + position);

	createFace(voxel, faces, Voxel::BLOCK_FACE_BOTTOM, 
		p4 + position, 
		p5 + position, 
		p1 + position, 
		p0 + position);

	createFace(voxel, faces, Voxel::BLOCK_FACE_BACK, 
		p5 + position, 
		p4 + position, 
		p7 + position, 
		p6 + position);
}

void Surface::createFace(const boost::shared_ptr<Voxel> & voxel, unsigned int drawFaces, unsigned int inFaces, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3)
{
	/*
	if (!(drawFaces & inFaces))
	{
		return;
	}
	*/
	unsigned int p0 = addVertex(v0);
	unsigned int p1 = addVertex(v1);
	unsigned int p2 = addVertex(v2);
	unsigned int p3 = addVertex(v3);

	unsigned int left = addTri(glm::ivec3(p0, p1, p2));
	unsigned int right = addTri(glm::ivec3(p0, p2, p3));
	addFace(left, right, inFaces, voxel->type());
}
