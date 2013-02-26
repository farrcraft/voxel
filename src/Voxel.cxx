/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Voxel.h"

Voxel::Voxel(BlockType type, const v3D::Vector3 & position) :
	type_(type),
	active_(true),
	position_(position)
{
	if (type_ == BLOCK_TYPE_AIR)
	{
		active_ = false;
	}
}

unsigned int Voxel::createFaceVertex(Mesh & mesh, const v3D::Vector3 & position, const v3D::Vector3 & color, unsigned int drawFaces, unsigned int inFaces)
{
	if (drawFaces & inFaces) 
	{
		return mesh.addVertex(vertex(position, color));
	}
	return -1;
}

void Voxel::createFaceTri(Mesh & mesh, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int drawFaces, unsigned int inFaces)
{
	if (drawFaces & inFaces)
	{
		mesh.addTri(v1, v2, v3);
	}
}

void Voxel::generate(Mesh & mesh, unsigned int faces)
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

	v3D::Vector3 color(0.4f, 0.6f, 0.9f);

	v3D::Vector3 p1(width, height, length);
	v3D::Vector3 p2(width, -height, length);
	v3D::Vector3 p3(-width, height, length);
	v3D::Vector3 p4(-width, -height, length);
	v3D::Vector3 p5(width, height, -length);
	v3D::Vector3 p6(-width, height, -length);
	v3D::Vector3 p7(width, -height, -length);
	v3D::Vector3 p8(-width, -height, -length);

	unsigned int v1 = createFaceVertex(mesh, p1 + position_, color, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_RIGHT|BLOCK_FACE_TOP));
	unsigned int v2 = createFaceVertex(mesh, p2 + position_, color, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_RIGHT|BLOCK_FACE_BOTTOM));
	unsigned int v3 = createFaceVertex(mesh, p3 + position_, color, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_LEFT|BLOCK_FACE_TOP));
	unsigned int v4 = createFaceVertex(mesh, p4 + position_, color, faces, (BLOCK_FACE_FRONT|BLOCK_FACE_LEFT|BLOCK_FACE_BOTTOM));
	unsigned int v5 = createFaceVertex(mesh, p5 + position_, color, faces, (BLOCK_FACE_BACK|BLOCK_FACE_RIGHT|BLOCK_FACE_TOP));
	unsigned int v6 = createFaceVertex(mesh, p6 + position_, color, faces, (BLOCK_FACE_BACK|BLOCK_FACE_LEFT|BLOCK_FACE_TOP));
	unsigned int v7 = createFaceVertex(mesh, p7 + position_, color, faces, (BLOCK_FACE_BACK|BLOCK_FACE_RIGHT|BLOCK_FACE_BOTTOM));
	unsigned int v8 = createFaceVertex(mesh, p8 + position_, color, faces, (BLOCK_FACE_BACK|BLOCK_FACE_LEFT|BLOCK_FACE_BOTTOM));

	// front
	createFaceTri(mesh, v1, v2, v3, faces, BLOCK_FACE_FRONT);
	createFaceTri(mesh, v2, v4, v3, faces, BLOCK_FACE_FRONT);
	// back
	createFaceTri(mesh, v5, v6, v7, faces, BLOCK_FACE_BACK);
	createFaceTri(mesh, v7, v6, v8, faces, BLOCK_FACE_BACK);
	// left
	createFaceTri(mesh, v3, v4, v8, faces, BLOCK_FACE_LEFT);
	createFaceTri(mesh, v3, v8, v6, faces, BLOCK_FACE_LEFT);
	// right
	createFaceTri(mesh, v1, v7, v2, faces, BLOCK_FACE_RIGHT);
	createFaceTri(mesh, v1, v5, v7, faces, BLOCK_FACE_RIGHT);
	//top
	createFaceTri(mesh, v5, v1, v3, faces, BLOCK_FACE_TOP);
	createFaceTri(mesh, v5, v3, v6, faces, BLOCK_FACE_TOP);
	// bottom
	createFaceTri(mesh, v7, v4, v2, faces, BLOCK_FACE_BOTTOM);
	createFaceTri(mesh, v7, v8, v4, faces, BLOCK_FACE_BOTTOM);

}

Vertex Voxel::vertex(const v3D::Vector3 &vec, const v3D::Vector3 & color)
{
	Vertex v;
	v.position_[0] = vec[0];
	v.position_[1] = vec[1];
	v.position_[2] = vec[2];
	v.position_[3] = 1.0f;

	v.color_[0] = color[0];
	v.color_[1] = color[1];
	v.color_[2] = color[2];
	v.color_[3] = 1.0f;

	return v;
}

bool Voxel::active() const
{
	return active_;
}

void Voxel::active(bool enabled)
{
	active_ = enabled;
}
