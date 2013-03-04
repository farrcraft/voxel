/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Item.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <boost/shared_ptr.hpp>

#include <vector>

class Camera;

class Player
{
	public:
		Player(const glm::vec3 & pos);

		typedef enum Movement
		{
			MOVE_UP			= 2,
			MOVE_DOWN		= 4,
			MOVE_LEFT		= 8,
			MOVE_RIGHT		= 16,
			MOVE_FORWARD	= 32,
			MOVE_BACKWARD	= 64
		} Movement;

		typedef enum Look
		{
			LOOK_UP		= 2,
			LOOK_DOWN	= 4,
			LOOK_LEFT	= 8,
			LOOK_RIGHT	= 16
		} Look;

		boost::shared_ptr<Camera> camera();

		glm::vec3 position() const;
		void position(const glm::vec3 & pos);

		void move(Movement direction);
		void look(Look direction);

	protected:
		glm::vec3 Player::checkMoveDirection(unsigned int check, Movement direction, unsigned int axis, float magnitude);

	private:
		glm::vec3 position_;
		boost::shared_ptr<Camera> camera_;
		float verticalLookAngle_;
		std::vector<Item> inventory_;
		unsigned int movement_;
};
