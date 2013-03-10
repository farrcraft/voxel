/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Chunk.h"

Chunk::Chunk() :
	dirty_(false),
	size_(16),
	empty_(false)
{
	// chunk is a volume cube of size_ voxels per dimension
	//unsigned int count = size_ * size_ * size_;
	unsigned int half = size_  / 2;

	for (unsigned int x = 0; x < size_; x++)
	{
		for (unsigned int y = 0; y < size_; y++)
		{
			for (unsigned z = 0; z < size_; z++)
			{
				glm::vec3 position((float) x, (float) y, (float) z);
				Voxel::BlockType type = Voxel::BLOCK_TYPE_AIR;
				// not doing any generation yet - just enable every other block in the chunk for now
				//if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2)
				//if (x % 2 == 0 && y % 2 == 0 && z % 2 == 0)
				unsigned int length = (x - half) * (x - half) + (y - half) * (y - half) + (z - half) * (z - half);
				if (sqrt(static_cast<float>(length)) <= static_cast<float>(half))
				{
					type = Voxel::BLOCK_TYPE_DIRT;
				}
				Voxel voxel(type, position);
				blocks_.push_back(voxel);
			}
		}
	}
}

// generate a single mesh for the entire chunk
void Chunk::generate(boost::shared_ptr<Mesh> mesh)
{
	for (std::vector<Voxel>::iterator it = blocks_.begin(); it != blocks_.end(); ++it)
	{
		// [TODO] - optimize this so touching block faces don't get rendered
		// e.g. if two blocks are next to each other in a row -
		// the left block's right faces won't be rendered & the right block's left faces won't be rendered
		it->generate(mesh, Voxel::BLOCK_FACE_ALL);
	}

}


void Chunk::update()
{
}

void Chunk::render()
{
}
