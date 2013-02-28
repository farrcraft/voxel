#include "Scene.h"
#include "Camera.h"

Scene::Scene()
{
	player_.reset(new Player(v3D::Vector3(0.0f, 0.0f, 5.0f)));
	chunk_.reset(new Chunk());
	mesh_.reset(new Mesh());
	state_.reset(new GameState());

	player_->camera()->createProjection();

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
