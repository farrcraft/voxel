/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once
/*
#include "../engine/Vertex.h"
#include "../engine/Color.h"
#include "../engine/Normal.h"
*/
#include "../engine/Mesh.h"

#include <glm/glm.hpp>

#include <boost/shared_ptr.hpp>

/**
 * A single voxel
 */
class Voxel
{
	public:
		typedef enum {
			BLOCK_TYPE_AIR,
			BLOCK_TYPE_DIRT,
			BLOCK_TYPE_GRASS,
			BLOCK_TYPE_SAND,
			BLOCK_TYPE_STONE,
			BLOCK_TYPE_GRAVEL,
			BLOCK_TYPE_WATER,
			BLOCK_TYPE_ORE,
			BLOCK_TYPE_WOOD,
			BLOCK_TYPE_LAVA,
			BLOCK_TYPE_GLASS,
			BLOCK_TYPE_BEDROCK,
			BLOCK_TYPE_CLAY,
			BLOCK_TYPE_ICE,
			BLOCK_TYPE_SANDSTONE,
			BLOCK_TYPE_OBSIDIAN,
			BLOCK_TYPE_SNOW
		} BlockType;

		typedef enum {
			BLOCK_FACE_FRONT = 2,
			BLOCK_FACE_LEFT = 4,
			BLOCK_FACE_RIGHT = 8,
			BLOCK_FACE_BACK = 16,
			BLOCK_FACE_TOP = 32,
			BLOCK_FACE_BOTTOM = 64,
			BLOCK_FACE_ALL = 128,
			BLOCK_FACE_NONE = 256
		} BlockFace;

		/**
		 * Default constructor
		 */
		Voxel(BlockType type, const glm::vec3 & position);

		/**
		 * Generate vertex data
		 *
		 * @param mesh generate the voxel vertex data into this mesh
		 * @param faces specify which block faces should be generated
		 */
		void generate(boost::shared_ptr<Mesh> mesh, unsigned int faces);

	protected:
		/**
		 * Add a vertex to a mesh only if any of the inFaces bits are set in drawFaces
		 */
		unsigned int createFaceVertex(boost::shared_ptr<Mesh> mesh, const glm::vec3 & position, unsigned int drawFaces, unsigned int inFaces);
		/**
		 * Add a tri to a mesh only if any of the inFaces bits are set in drawFaces
		 */
		void createFaceTri(boost::shared_ptr<Mesh> mesh, unsigned int v1, unsigned int v2, unsigned int v3, unsigned int drawFaces, unsigned int inFaces);

		bool active() const;
		void active(bool enabled);

	private:
		BlockType type_;
		glm::vec3 position_;
		bool active_;
};
