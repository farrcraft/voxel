/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class VertexBuffer;

class ChunkBufferPool
{
	public:
		void add(size_t chunkId, const boost::shared_ptr<VertexBuffer> & buffer);
		boost::shared_ptr<VertexBuffer> get(size_t chunkId);
		void render() const;

	private:
		boost::unordered_map<size_t, boost::shared_ptr<VertexBuffer> > pool_;
};