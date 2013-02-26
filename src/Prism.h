/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Vertex.h"

#include <3dtypes/Vector3.h>

#include <vector>

/**
 * A simple prism object
 */
class Prism
{
	public:
		/**
		 * Default constructor
		 */
		Prism();

		/**
		 * Retrieve the vertex data representing the object
		 *
		 */
		std::vector<Vertex> & data();
		/**
		 * Generate a vertex from a vector
		 *
		 */
		Vertex vertex(const v3D::Vector3 & vec);

	private:
		std::vector<Vertex> vertices_;
};
