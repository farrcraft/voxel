#include "Scene.h"
#include "Camera.h"

Scene::Scene() :
	player_(v3D::Vector3(0.0f, 0.0f, 5.0f))
{
	player_.camera()->createProjection();

	chunk_.generate(mesh_);
}


Player & Scene::player()
{
	return player_;
}

GameState & Scene::state()
{
	return state_;
}

boost::shared_ptr<Camera> Scene::camera()
{
	return player_.camera();
}

Mesh & Scene::mesh()
{
	return mesh_;
}
