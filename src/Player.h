#pragma once

#include "Item.h"

#include <3dtypes/Vector3.h>

#include <boost/shared_ptr.hpp>

#include <vector>

class Camera;

class Player
{
	public:
		Player(const v3D::Vector3 & pos);

		typedef enum Movement
		{
			MOVE_UP,
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
			MOVE_FORWARD,
			MOVE_BACKWARD
		} Movement;

		typedef enum Look
		{
			LOOK_UP,
			LOOK_DOWN,
			LOOK_LEFT,
			LOOK_RIGHT
		} Look;

		boost::shared_ptr<Camera> camera();

		v3D::Vector3 position() const;
		void position(const v3D::Vector3 & pos);

		void move(Movement direction);
		void look(Look direction);

	private:
		v3D::Vector3 position_;
		boost::shared_ptr<Camera> camera_;
		float verticalLookAngle_;
		std::vector<Item> inventory_;
};
