/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Player.h"
#include "GameState.h"
#include "Chunk.h"

#include <boost/shared_ptr.hpp>

class Scene
{
	public:
		Scene();

		Player & player();
		GameState & state();
		boost::shared_ptr<Camera> camera();
		Mesh & mesh();

	private:
		Player player_;
		GameState state_;
		Mesh mesh_;
		Chunk chunk_;
};
