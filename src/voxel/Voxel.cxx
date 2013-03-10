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
	if (drawFaces & inFaces) 
	{
		return mesh->addVertex(position);
	}
	return -1;
}

void Voxel::createFaceTri(boost::shared_ptr<Mesh> mesh, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int drawFaces, unsigned int inFaces)
{
	if (drawFaces & inFaces)
	{
		mesh->addTri(v1, v2, v3);
	}
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
	// front
	unsigned int v0 = createFaceVertex(mesh, p0 + position_, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_LEFT|BLOCK_FACE_BOTTOM));
	unsigned int v1 = createFaceVertex(mesh, p1 + position_, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_RIGHT|BLOCK_FACE_BOTTOM));
	unsigned int v2 = createFaceVertex(mesh, p2 + position_, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_RIGHT|BLOCK_FACE_TOP));
	unsigned int v3 = createFaceVertex(mesh, p3 + position_, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_LEFT|BLOCK_FACE_TOP));
	// back
	unsigned int v4 = createFaceVertex(mesh, p4 + position_, faces, (BLOCK_FACE_BACK|BLOCK_FACE_LEFT|BLOCK_FACE_BOTTOM));
	unsigned int v5 = createFaceVertex(mesh, p5 + position_, faces, (BLOCK_FACE_BACK|BLOCK_FACE_RIGHT|BLOCK_FACE_BOTTOM));
	unsigned int v6 = createFaceVertex(mesh, p6 + position_, faces, (BLOCK_FACE_BACK|BLOCK_FACE_RIGHT|BLOCK_FACE_TOP));
	unsigned int v7 = createFaceVertex(mesh, p7 + position_, faces, (BLOCK_FACE_BACK|BLOCK_FACE_LEFT|BLOCK_FACE_TOP));

	// counter-clockwise winding
	// front - pos z
	createFaceTri(mesh, v0, v1, v2, faces, BLOCK_FACE_FRONT);
	createFaceTri(mesh, v0, v2, v3, faces, BLOCK_FACE_FRONT);
	// right - pos x
	createFaceTri(mesh, v1, v5, v6, faces, BLOCK_FACE_RIGHT);
	createFaceTri(mesh, v1, v6, v2, faces, BLOCK_FACE_RIGHT);
	// top - pos y
	createFaceTri(mesh, v2, v6, v7, faces, BLOCK_FACE_TOP);
	createFaceTri(mesh, v2, v7, v3, faces, BLOCK_FACE_TOP);

	// clockwise winding
	// left - neg x
	createFaceTri(mesh, v3, v4, v0, faces, BLOCK_FACE_LEFT);
	createFaceTri(mesh, v3, v7, v4, faces, BLOCK_FACE_LEFT);
	// bottom - neg y
	createFaceTri(mesh, v4, v5, v1, faces, BLOCK_FACE_BOTTOM);
	createFaceTri(mesh, v4, v1, v0, faces, BLOCK_FACE_BOTTOM);
	// back - neg z
	createFaceTri(mesh, v5, v7, v6, faces, BLOCK_FACE_BACK);
	createFaceTri(mesh, v5, v4, v7, faces, BLOCK_FACE_BACK);

	mesh->addNormal(glm::vec3(0.0f, 0.0f, 1.0f));
	mesh->addNormal(glm::vec3(1.0f, 0.0f, 0.0f));
	mesh->addNormal(glm::vec3(0.0f, 1.0f, 0.0f));
	mesh->addNormal(glm::vec3(-1.0f, 0.0f, 0.0f));
	mesh->addNormal(glm::vec3(0.0f, -1.0f, 0.0f));
	mesh->addNormal(glm::vec3(0.0f, 0.0f, -1.0f));
	// extra junk normals to match vertex count
	mesh->addNormal(glm::vec3(1.0f, 0.0f, 0.0f));
	mesh->addNormal(glm::vec3(1.0f, 0.0f, 0.0f));
}

bool Voxel::active() const
{
	return active_;
}

void Voxel::active(bool enabled)
{
	active_ = enabled;
}
