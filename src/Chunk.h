/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Voxel.h"

class Chunk
{

	public:
		Chunk();

		void generate(boost::shared_ptr<Mesh> mesh);
		void update();
		void render();

		void invalidate();

	private:
		std::vector<Voxel> blocks_;
		bool dirty_;
		unsigned int size_;
};
