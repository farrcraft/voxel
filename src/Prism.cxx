/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Prism.h"


Prism::Prism()
{
	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -1.25f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f,  0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f,  0.25f, -2.75f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -1.25f)));
	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -1.25f)));

	vertices_.push_back(vertex(v3D::Vector3(0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -2.75f)));
	vertices_.push_back(vertex(v3D::Vector3(-0.25f, -0.25f, -1.25f)));


	// color data
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 0.0f, 1.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));
	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));
	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));

	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));
	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));
	vertices_.push_back(vertex(v3D::Vector3(0.8f, 0.8f, 0.8f)));

	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.5f, 0.5f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));
	vertices_.push_back(vertex(v3D::Vector3(1.0f, 0.0f, 0.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));

	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));
	vertices_.push_back(vertex(v3D::Vector3(0.0f, 1.0f, 1.0f)));
}

std::vector<Vertex> & Prism::data()
{
	return vertices_;
}

Vertex Prism::vertex(const v3D::Vector3 &vec)
{
	Vertex v;
	v.position_[0] = vec[0];
	v.position_[1] = vec[1];
	v.position_[2] = vec[2];
	v.position_[3] = 1.0f;

	return v;
}
