/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "MeshBuilder.h"
#include "Chunk.h"
#include "../engine/Mesh.h"
#include "../engine/VertexBuffer.h"
#include "MeshCache.h"
#include "ChunkBufferPool.h"
#include "../engine/MortonCode.h"
#include "Surface.h"

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

boost::shared_ptr<ChunkBufferPool> MeshBuilder::build2()
{
	size_t vertices = 32768;
	size_t tris = 49152;
	size_t faces = 24576;
	boost::shared_ptr<MeshCache> mesh(new MeshCache(vertices, tris, faces));
	boost::shared_ptr<ChunkBufferPool> pool(new ChunkBufferPool());
	// build mesh data from chunks
	for (boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator it = chunks_.begin(); it != chunks_.end(); it++)
	{
		// only generate mesh data for chunks that have active blocks in them
		if (!(*it).second->empty())
		{
			generateChunk2(mesh, pool, (*it).second);
		}
	}
	return pool;
}

boost::shared_ptr<Surface> MeshBuilder::buildSurface()
{
	boost::shared_ptr<Surface> surface(new Surface());
	// build mesh data from chunks
	for (boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator it = chunks_.begin(); it != chunks_.end(); it++)
	{
		extractChunk(surface, (*it).second);
	}
	return surface;
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
				glm::ivec3 neighborBlockPosition = pos;
				switch (checkFaces[i])
				{
					case Voxel::BLOCK_FACE_LEFT:
						if (pos.x == 0 && chunkPosition.x > 0)
						{
							neighborChunkPosition.x -= 1;
							ignoreNeighbor = false;
							neighborBlockPosition.x = 15;
						}
						break;
					case Voxel::BLOCK_FACE_RIGHT:
						if (pos.x == chunkSize - 1)
						{
							neighborChunkPosition.x += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BACK:
						if (pos.z == 0 && chunkPosition.z > 0)
						{
							neighborChunkPosition.z -= 1;
							neighborBlockPosition.z = 15;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_FRONT:
						if (pos.z == chunkSize - 1)
						{
							neighborChunkPosition.z += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_TOP:
						if (pos.y == chunkSize - 1)
						{
							neighborChunkPosition.y += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BOTTOM:
						if (pos.y == 0 && chunkPosition.y > 0)
						{
							neighborChunkPosition.y -= 1;
							neighborBlockPosition.y = 15;
							ignoreNeighbor = false;
						}
						break;
				}
				if (!ignoreNeighbor)
				{
					unsigned int neighborChunkHash = codec.encode(neighborChunkPosition);
					boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator neighborChunk = chunks_.find(neighborChunkHash);
					if (neighborChunk != chunks_.end())
					{
						if ((*neighborChunk).second->active(neighborBlockPosition))
						{
							faces &= ~checkFaces[i];
						}
					}
				}
			}
		}
		mesh->extract((*it).second, faces);
	}

}

// generate a single mesh for the entire chunk
void MeshBuilder::generateChunk2(boost::shared_ptr<MeshCache> & mesh, boost::shared_ptr<ChunkBufferPool> & pool, boost::shared_ptr<Chunk> & chunk)
{
	unsigned int hash = 0;
	glm::ivec3 pos;
	MortonCode codec;
	unsigned int faces = 0;

	glm::ivec3 chunkPosition = chunk->position();
	unsigned int chunkSize = chunk->size();

	size_t chunkId = codec.encode(chunkPosition);
	mesh->reset(); // reset cache not the ptr

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
				glm::ivec3 neighborBlockPosition = pos;
				switch (checkFaces[i])
				{
					case Voxel::BLOCK_FACE_LEFT:
						if (pos.x == 0 && chunkPosition.x > 0)
						{
							neighborChunkPosition.x -= 1;
							ignoreNeighbor = false;
							neighborBlockPosition.x = 15;
						}
						break;
					case Voxel::BLOCK_FACE_RIGHT:
						if (pos.x == chunkSize - 1)
						{
							neighborChunkPosition.x += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BACK:
						if (pos.z == 0 && chunkPosition.z > 0)
						{
							neighborChunkPosition.z -= 1;
							neighborBlockPosition.z = 15;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_FRONT:
						if (pos.z == chunkSize - 1)
						{
							neighborChunkPosition.z += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_TOP:
						if (pos.y == chunkSize - 1)
						{
							neighborChunkPosition.y += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BOTTOM:
						if (pos.y == 0 && chunkPosition.y > 0)
						{
							neighborChunkPosition.y -= 1;
							neighborBlockPosition.y = 15;
							ignoreNeighbor = false;
						}
						break;
				}
				if (!ignoreNeighbor)
				{
					unsigned int neighborChunkHash = codec.encode(neighborChunkPosition);
					boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator neighborChunk = chunks_.find(neighborChunkHash);
					if (neighborChunk != chunks_.end())
					{
						if ((*neighborChunk).second->active(neighborBlockPosition))
						{
							faces &= ~checkFaces[i];
						}
					}
				}
			}
		}
		mesh->extract((*it).second, faces);
	}

	boost::shared_ptr<VertexBuffer> buffer(new VertexBuffer(mesh));
	pool->add(chunkId, buffer);
}


// generate a single surface for the entire chunk
void MeshBuilder::extractChunk(boost::shared_ptr<Surface> & surface, boost::shared_ptr<Chunk> & chunk)
{
	unsigned int hash = 0;
	glm::ivec3 pos;
	MortonCode codec;
	unsigned int faces = 0;

	glm::ivec3 chunkPosition = chunk->position();
	unsigned int chunkSize = chunk->size();

	Voxel::BlockFace checkFaces[6] = { 
		Voxel::BLOCK_FACE_FRONT, 
		Voxel::BLOCK_FACE_BACK, 
		Voxel::BLOCK_FACE_LEFT, 
		Voxel::BLOCK_FACE_RIGHT, 
		Voxel::BLOCK_FACE_TOP, 
		Voxel::BLOCK_FACE_BOTTOM 
	};
	boost::unordered_map<unsigned int, boost::shared_ptr<Voxel> > blocks = chunk->blocks();

	bool checkNeighborChunks = false;
	bool checkNeighborFaces = false;

	for (boost::unordered_map<unsigned int, boost::shared_ptr<Voxel> >::iterator it = blocks.begin(); it != blocks.end(); ++it)
	{
		// start with all possible faces
		faces = Voxel::BLOCK_FACE_FRONT|Voxel::BLOCK_FACE_BACK|Voxel::BLOCK_FACE_LEFT|Voxel::BLOCK_FACE_RIGHT|Voxel::BLOCK_FACE_TOP|Voxel::BLOCK_FACE_BOTTOM;

		hash = (*it).first;
		pos = codec.decode3(hash);
		/* commented out neighbor face elemination to debug mesh generation (didn't fix the problem)

		for (unsigned int i = 0; i < 6; i++)
		{
			// check for face occlusion from other blocks within the same chunk
			if (checkNeighborFaces && chunk->hidden(checkFaces[i], pos))
			{
				faces &= ~checkFaces[i];
			}
			else // check for face occlusion from blocks in adjacent chunks
			{
				bool ignoreNeighbor = true;
				glm::ivec3 neighborChunkPosition = chunkPosition;
				glm::ivec3 neighborBlockPosition = pos;
				switch (checkFaces[i])
				{
					case Voxel::BLOCK_FACE_LEFT:
						if (pos.x == 0 && chunkPosition.x > 0)
						{
							neighborChunkPosition.x -= 1;
							ignoreNeighbor = false;
							neighborBlockPosition.x = 15;
						}
						break;
					case Voxel::BLOCK_FACE_RIGHT:
						if (pos.x == chunkSize - 1)
						{
							neighborChunkPosition.x += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BACK:
						if (pos.z == 0 && chunkPosition.z > 0)
						{
							neighborChunkPosition.z -= 1;
							neighborBlockPosition.z = 15;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_FRONT:
						if (pos.z == chunkSize - 1)
						{
							neighborChunkPosition.z += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_TOP:
						if (pos.y == chunkSize - 1)
						{
							neighborChunkPosition.y += 1;
							ignoreNeighbor = false;
						}
						break;
					case Voxel::BLOCK_FACE_BOTTOM:
						if (pos.y == 0 && chunkPosition.y > 0)
						{
							neighborChunkPosition.y -= 1;
							neighborBlockPosition.y = 15;
							ignoreNeighbor = false;
						}
						break;
				}
				if (!ignoreNeighbor && checkNeighborChunks)
				{
					unsigned int neighborChunkHash = codec.encode(neighborChunkPosition);
					boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > >::iterator neighborChunk = chunks_.find(neighborChunkHash);
					if (neighborChunk != chunks_.end())
					{
						if ((*neighborChunk).second->active(neighborBlockPosition))
						{
							faces &= ~checkFaces[i];
						}
					}
				}
			}
		}
		*/
		surface->extract((*it).second, faces);
	}

}
