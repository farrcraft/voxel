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

		boost::shared_ptr<Player> player();
		boost::shared_ptr<GameState> state();
		boost::shared_ptr<Camera> camera();
		boost::shared_ptr<Mesh> mesh();

	private:
		boost::shared_ptr<Player> player_;
		boost::shared_ptr<GameState> state_;
		boost::shared_ptr<Mesh> mesh_;
		boost::shared_ptr<Chunk> chunk_;
};