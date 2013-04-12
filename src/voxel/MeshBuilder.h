/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class Mesh;
class Chunk;
class Surface;
class ChunkBufferPool;
class MeshCache;

class MeshBuilder
{
	public:
		MeshBuilder(boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > > & chunks);

		boost::shared_ptr<Mesh> build();
		boost::shared_ptr<Surface> buildSurface();

		boost::shared_ptr<ChunkBufferPool> build2();

	protected:
		/**
		 * Genereate mesh geometry for a chunk
		 */
		void generateChunk(boost::shared_ptr<Mesh> & mesh, boost::shared_ptr<Chunk> & chunk);
		void extractChunk(boost::shared_ptr<Surface> & surface, boost::shared_ptr<Chunk> & chunk);

		void generateChunk2(boost::shared_ptr<MeshCache> & mesh, boost::shared_ptr<ChunkBufferPool> & pool, boost::shared_ptr<Chunk> & chunk);

	private:
		boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > > chunks_;
};
