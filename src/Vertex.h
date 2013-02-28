/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

/**
 * A single vertex structure
 * This is a basic data bag that can be used for creating and sending vertex data to the GPU.
 */
typedef struct Vertex
{
	float position_[3];
} Vertex;

bool operator == (const Vertex & v1, const Vertex & v2);

