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

class MeshBuilder
{
	public:
		MeshBuilder(boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > > & chunks);

		boost::shared_ptr<Mesh> build();

	protected:
		/**
		 * Genereate mesh geometry for a chunk
		 */
		void MeshBuilder::generateChunk(boost::shared_ptr<Mesh> & mesh, boost::shared_ptr<Chunk> & chunk);

	private:
		boost::unordered_map<unsigned int, boost::shared_ptr<Chunk > > chunks_;
};
