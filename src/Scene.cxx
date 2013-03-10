#include "Scene.h"
#include "engine/Camera.h"

Scene::Scene()
{
	player_.reset(new Player(glm::vec3(0.0f, 0.0f, 25.0f)));
	chunk_.reset(new Chunk());
	mesh_.reset(new Mesh());
	state_.reset(new GameState());

	chunk_->generate(mesh_);
}


boost::shared_ptr<Player> Scene::player()
{
	return player_;
}

boost::shared_ptr<GameState> Scene::state()
{
	return state_;
}

boost::shared_ptr<Camera> Scene::camera()
{
	return player_->camera();
}

boost::shared_ptr<Mesh> Scene::mesh()
{
	return mesh_;
}

void Scene::tick(unsigned int delta)
{
	player_->tick(delta);
}

