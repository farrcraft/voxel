/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "MeshBuilder.h"
#include "Chunk.h"
#include "../engine/Mesh.h"
#include "../engine/MortonCode.h"

MeshBuilder::MeshBuilder(boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > > & chunks) :
	chunks_(chunks)
{
}

boost::shared_ptr<Mesh> MeshBuilder::build()
{
	boost::shared_ptr<Mesh> mesh(new Mesh());
	// build mesh data from chunks
	for (boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator it = chunks_.begin(); it != chunks_.end(); it++)
	{
		generateChunk(mesh, (*it).second);
	}
	return mesh;
}

// generate a single mesh for the entire chunk
void MeshBuilder::generateChunk(boost::shared_ptr<Mesh> & mesh, boost::shared_ptr<Chunk> & chunk)
{
	unsigned int hash = 0;
	glm::ivec3 pos;
	MortonCode codec;
	unsigned int faces = 0;

	glm::ivec3 chunkPosition = chunk->position();
	unsigned int chunkSize = chunk->size();

	Voxel::BlockFace checkFaces[6] = { 
		Voxel::BLOCK_FACE_FRONT, Voxel::BLOCK_FACE_BACK, 
		Voxel::BLOCK_FACE_LEFT, Voxel::BLOCK_FACE_RIGHT, 
		Voxel::BLOCK_FACE_TOP, Voxel::BLOCK_FACE_BOTTOM 
	};
	boost::unordered_map<unsigned int, boost::shared_ptr<Voxel> > blocks = chunk->blocks();

	for (boost::unordered_map<unsigned int, boost::shared_ptr<Voxel> >::iterator it = blocks.begin(); it != blocks.end(); ++it)
	{
		// start with all possible faces
		faces = Voxel::BLOCK_FACE_FRONT|Voxel::BLOCK_FACE_BACK|Voxel::BLOCK_FACE_LEFT|Voxel::BLOCK_FACE_RIGHT|Voxel::BLOCK_FACE_TOP|Voxel::BLOCK_FACE_BOTTOM;

		hash = (*it).first;
		pos = codec.decode3(hash);

		for (unsigned int i = 0; i < 6; i++)
		{
			// check for face occlusion from other blocks within the same chunk
			if (chunk->hidden(checkFaces[i], pos))
			{
				faces &= ~checkFaces[i];
			}
			else // check for face occlusion from blocks in adjacent chunks
			{
				bool ignoreNeighbor = true;
				glm::ivec3 neighborChunkPosition = chunkPosition;
				switch (checkFaces[i])
				{
					case Voxel::BLOCK_FACE_LEFT:
						if (pos.x == 0 && chunkPosition.x > 0)
						{
							neighborChunkPosition.x -= 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_RIGHT:
						if (pos.x == chunkSize - 1)
						{
							neighborChunkPosition.x += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_FRONT:

						break;
					case Voxel::BLOCK_FACE_BACK:
						break;
					case Voxel::BLOCK_FACE_TOP:
						break;
					case Voxel::BLOCK_FACE_BOTTOM:
						break;
				}
				if (!ignoreNeighbor)
				{
					unsigned int neighborChunkHash = codec.encode(neighborChunkPosition);
					boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator neighborChunk = chunks_.find(neighborChunkHash);
					if (neighborChunk != chunks_.end())
					{
						glm::ivec3 neighborBlockPosition = pos;
						neighborBlockPosition.x = 15;
						if ((*neighborChunk).second->active(neighborBlockPosition))
						{
							faces &= ~checkFaces[i];
						}
					}
				}
			}
		}
		(*it).second->generate(mesh, faces);
	}

}
