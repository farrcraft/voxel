/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Player.h"
#include "Camera.h"

float offset = 0.05f;

Player::Player(const glm::vec3 & pos) :
	position_(pos),
	verticalLookAngle_(0.0f),
	movement_(0)
{
	camera_.reset(new Camera());
	camera_->behavior(Camera::CAMERA_BEHAVIOR_FIRST_PERSON);
	camera_->position(position_);
	camera_->acceleration(glm::vec3(8.0f, 8.0f, 8.0f));
	camera_->velocity(glm::vec3(2.0f, 2.0f, 2.0f));
}

glm::vec3 Player::position() const
{
	return position_;
}

boost::shared_ptr<Camera> Player::camera()
{
	return camera_;
}

void Player::position(const glm::vec3 & pos)
{
	position_ = pos;
}

glm::vec3 Player::checkMoveDirection(unsigned int check, Movement direction, unsigned int axis, float magnitude)
{
	glm::vec3 vector(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = camera_->currentVelocity();
	velocity[axis] = 0.0f;
	if (check == direction)
	{
		if (!(movement_ & direction))
		{
			movement_ |= direction;
			camera_->currentVelocity(velocity.x, velocity.y, velocity.z);
		}
		vector[axis] = magnitude;
	}
	else
	{
		movement_ &= ~direction;
	}
	return vector;
}

void Player::move(Movement direction)
{
	glm::vec3 dir(0.0f, 0.0f, 0.0f);
	dir += checkMoveDirection(direction, MOVE_FORWARD, 2, 1.0f);
	dir += checkMoveDirection(direction, MOVE_BACKWARD, 2, -1.0f);
	dir += checkMoveDirection(direction, MOVE_RIGHT, 0, 1.0f);
	dir += checkMoveDirection(direction, MOVE_LEFT, 0, -1.0f);
	dir += checkMoveDirection(direction, MOVE_UP, 1, 1.0f);
	dir += checkMoveDirection(direction, MOVE_DOWN, 1, -1.0f);

	float elapsedTimeSec = 0.1f;
	camera_->updatePosition(dir, elapsedTimeSec);
}

void Player::look(Look direction)
{
	float cameraRotationSpeed = 0.4f;
	float pitch = 0.0f;
	float heading = 0.0f;
	/*
	pitch = -mouse.yDistanceFromWindowCenter() * cameraRotationSpeed;
	heading = mouse.xDistanceFromWindowCenter() * cameraRotationSpeed;
	*/

	float lookAngle = 1.0f;
	switch (direction)
	{
		case LOOK_UP:
			pitch = lookAngle * cameraRotationSpeed;
			break;
		case LOOK_DOWN:
			pitch = -lookAngle * cameraRotationSpeed;
			break;
		case LOOK_LEFT:
			heading = -lookAngle * cameraRotationSpeed;
			break;
		case LOOK_RIGHT:
			heading = lookAngle * cameraRotationSpeed;
			break;
	}

	camera_->rotate(heading, pitch, 0.0f);
}

