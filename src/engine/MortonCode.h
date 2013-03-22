/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <glm/glm.hpp>


class MortonCode
{
	public:
		unsigned int encode(const glm::ivec2 & vec) const;
		unsigned int encode(const glm::ivec3 & vec) const;

		glm::ivec2 decode2(unsigned int code) const;
		glm::ivec3 decode3(unsigned int code) const;
};
