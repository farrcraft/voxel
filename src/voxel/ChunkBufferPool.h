/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <gl/VertexBuffer.h>

class ChunkBufferPool
{
	public:
		void add(size_t chunkId, const boost::shared_ptr<v3D::VertexBuffer> & buffer);
		boost::shared_ptr<v3D::VertexBuffer> get(size_t chunkId);
		void render() const;

	private:
		boost::unordered_map<size_t, boost::shared_ptr<v3D::VertexBuffer> > pool_;
};