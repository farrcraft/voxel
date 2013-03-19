/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Voxel.h"

Voxel::Voxel(BlockType type, const glm::vec3 & position) :
	type_(type),
	active_(true),
	position_(position)
{
	if (type_ == BLOCK_TYPE_AIR)
	{
		active_ = false;
	}
}

unsigned int Voxel::createFaceVertex(boost::shared_ptr<Mesh> mesh, const glm::vec3 & position, unsigned int drawFaces, unsigned int inFaces)
{
	if (!(drawFaces & inFaces))
	{
		return -1;
	}
	return mesh->addVertex(position);
}

void Voxel::createFaceTri(boost::shared_ptr<Mesh> mesh, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int drawFaces, unsigned int inFaces)
{
	if (drawFaces & inFaces)
	{
		mesh->addTri(v1, v2, v3);
	}
}

void Voxel::createFace(boost::shared_ptr<Mesh> mesh, unsigned int drawFaces, unsigned int inFaces, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3, const glm::vec3 & normal)
{
	if (!(drawFaces & inFaces))
	{
		return;
	}
	unsigned int p0 = mesh->addVertex(v0, normal);
	unsigned int p1 = mesh->addVertex(v1, normal);
	unsigned int p2 = mesh->addVertex(v2, normal);
	unsigned int p3 = mesh->addVertex(v3, normal);

	createFaceTri(mesh, p0, p1, p2, drawFaces, inFaces);
	createFaceTri(mesh, p0, p2, p3, drawFaces, inFaces);
}

void Voxel::generate(boost::shared_ptr<Mesh> mesh, unsigned int faces)
{
	if (!active_ || (faces & BLOCK_FACE_NONE))
	{
		return;
	}

	if (faces & BLOCK_FACE_ALL)
	{
		faces = BLOCK_FACE_FRONT|BLOCK_FACE_BACK|BLOCK_FACE_LEFT|BLOCK_FACE_RIGHT|BLOCK_FACE_TOP|BLOCK_FACE_BOTTOM;
	}

	float width = 0.5f;
	float height = 0.5f;
	float length = 0.5f;

	// front
	glm::vec3 p0(-width, -height, length);
	glm::vec3 p1(width, -height, length);
	glm::vec3 p2(width, height, length);
	glm::vec3 p3(-width, height, length);
	// back
	glm::vec3 p4(-width, -height, -length);
	glm::vec3 p5(width, -height, -length);
	glm::vec3 p6(width, height, -length);
	glm::vec3 p7(-width, height, -length);

	createFace(mesh, faces, BLOCK_FACE_FRONT, 
		p0 + position_, 
		p1 + position_, 
		p2 + position_, 
		p3 + position_, 
		glm::vec3(0.0f, 0.0f, 1.0f));

	createFace(mesh, faces, BLOCK_FACE_RIGHT, 
		p1 + position_, 
		p5 + position_, 
		p6 + position_, 
		p2 + position_, 
		glm::vec3(1.0f, 0.0f, 0.0f));

	createFace(mesh, faces, BLOCK_FACE_TOP, 
		p2 + position_, 
		p6 + position_, 
		p7 + position_, 
		p3 + position_, 
		glm::vec3(0.0f, 1.0f, 0.0f));

	createFace(mesh, faces, BLOCK_FACE_LEFT, 
		p4 + position_, 
		p0 + position_, 
		p3 + position_, 
		p7 + position_, 
		glm::vec3(-1.0f, 0.0f, 0.0f));

	createFace(mesh, faces, BLOCK_FACE_BOTTOM, 
		p4 + position_, 
		p5 + position_, 
		p1 + position_, 
		p0 + position_, 
		glm::vec3(0.0f, -1.0f, 0.0f));

	createFace(mesh, faces, BLOCK_FACE_BACK, 
		p5 + position_, 
		p4 + position_, 
		p7 + position_, 
		p6 + position_, 
		glm::vec3(0.0f, 0.0f, -1.0f));
}

bool Voxel::active() const
{
	return active_;
}

void Voxel::active(bool enabled)
{
	active_ = enabled;
}
