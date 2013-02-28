/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

/**
 * A single color structure
 * This is a basic data bag that can be used for creating and sending color data to the GPU.
 */
typedef struct Color
{
	float color_[4];
} Color;
