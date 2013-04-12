#include "Mesh.h"
#include "MortonCode.h"
#include "../voxel/Voxel.h"

Mesh::Mesh()
{
}

unsigned int Mesh::addTri(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned int index = tris_.size();
	tris_[index] = glm::ivec3(a, b, c);
	return index;

	/*
	this doesn't work...

	glm::ivec3 tri(a, b, c);
	MortonCode codec;
	unsigned int hash = codec.encode(tri);
	tris_[hash] = tri;
	return hash;
	*/
}

unsigned int Mesh::addVertex(const glm::vec3 & vertex)
{
	MortonCode codec;
	unsigned int hash = codec.encode(glm::ivec3(
		static_cast<unsigned int>(vertex.x),
		static_cast<unsigned int>(vertex.y),
		static_cast<unsigned int>(vertex.z)
	));
	//if (vertices_.find(hash) == vertices_.end())
	//{
		vertices_[hash] = vertex;
	//}
	return hash;
}

boost::unordered_map<unsigned int, glm::vec3> & Mesh::vertices()
{
	return vertices_;
}

boost::unordered_map<unsigned int, glm::ivec3> & Mesh::tris()
{
	return tris_;
}

boost::unordered_map<unsigned int, glm::ivec4> & Mesh::faces()
{
	return faces_;
}

void Mesh::extract(const boost::shared_ptr<Voxel> & voxel, unsigned int faces)
{
	if (!voxel->active() || (faces & Voxel::BLOCK_FACE_NONE))
	{
		return;
	}

	if (faces & Voxel::BLOCK_FACE_ALL)
	{
		faces = Voxel::BLOCK_FACE_FRONT|Voxel::BLOCK_FACE_BACK|Voxel::BLOCK_FACE_LEFT|Voxel::BLOCK_FACE_RIGHT|Voxel::BLOCK_FACE_TOP|Voxel::BLOCK_FACE_BOTTOM;
	}

	glm::vec3 position = voxel->position();
	unsigned int type = voxel->type();

	// front
	glm::vec3 p0 = glm::vec3(0.0f, 0.0f, 1.0f) + position;
	glm::vec3 p1 = glm::vec3(1.0f, 0.0f, 1.0f) + position;
	glm::vec3 p2 = glm::vec3(1.0f, 1.0f, 1.0f) + position;
	glm::vec3 p3 = glm::vec3(0.0f, 1.0f, 1.0f) + position;
	// back
	glm::vec3 p4 = glm::vec3(0.0f, 0.0f, 0.0f) + position;
	glm::vec3 p5 = glm::vec3(1.0f, 0.0f, 0.0f) + position;
	glm::vec3 p6 = glm::vec3(1.0f, 1.0f, 0.0f) + position;
	glm::vec3 p7 = glm::vec3(0.0f, 1.0f, 0.0f) + position;

	createFace(type, faces, Voxel::BLOCK_FACE_FRONT,  p0, p1, p2, p3);
	createFace(type, faces, Voxel::BLOCK_FACE_RIGHT,  p1, p5, p6, p2);
	createFace(type, faces, Voxel::BLOCK_FACE_TOP,    p2, p6, p7, p3);
	createFace(type, faces, Voxel::BLOCK_FACE_LEFT,   p4, p0, p3, p7);
	createFace(type, faces, Voxel::BLOCK_FACE_BOTTOM, p4, p5, p1, p0);
	createFace(type, faces, Voxel::BLOCK_FACE_BACK,   p5, p4, p7, p6);
}

void Mesh::createFace(unsigned int type, unsigned int drawFaces, unsigned int inFaces, const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2, const glm::vec3 & v3)
{
	if (!(drawFaces & inFaces))
	{
		return;
	}

	unsigned int p0 = addVertex(v0);
	unsigned int p1 = addVertex(v1);
	unsigned int p2 = addVertex(v2);
	unsigned int p3 = addVertex(v3);

	unsigned int t1 = addTri(p0, p1, p2);
	unsigned int t2 = addTri(p0, p2, p3);

	unsigned int index = faces_.size();
	faces_[index] = glm::ivec4(t1, t2, inFaces, type);
}
