#include "Player.h"
#include "Camera.h"

float offset = 0.05f;

Player::Player(const v3D::Vector3 & pos) :
	position_(pos),
	verticalLookAngle_(0.0f)
{
	camera_.reset(new Camera());
	camera_->eye(position_);
}

v3D::Vector3 Player::position() const
{
	return position_;
}

boost::shared_ptr<Camera> Player::camera()
{
	return camera_;
}

void Player::position(const v3D::Vector3 & pos)
{
	position_ = pos;
}

void Player::move(Movement direction)
{
	switch (direction)
	{
		case MOVE_UP:
			position(position_ += v3D::Vector3(0.0f, -offset, 0.0f));
			camera_->pedestal(-offset);
			break;
		case MOVE_DOWN:
			position(position_ += v3D::Vector3(0.0f, offset, 0.0f));
			camera_->pedestal(offset);
			break;
		case MOVE_LEFT:
			position(position_ += v3D::Vector3(-offset, 0.0f, 0.0f));
			camera_->truck(-offset);
			break;
		case MOVE_RIGHT:
			position(position_ += v3D::Vector3(offset, 0.0f, 0.0f));
			camera_->truck(offset);
			break;
		case MOVE_FORWARD:
			position(position_ + v3D::Vector3(0.0f, 0.0f, -offset));
			camera_->dolly(-offset);
			break;
		case MOVE_BACKWARD:
			position(position_ += v3D::Vector3(0.0f, 0.0f, offset));
			camera_->dolly(offset);
			break;
	}
}

void Player::look(Look direction)
{
	float lookAngle = 1.0f;
	float maximumAngle = 45.0f;

	switch (direction)
	{
		case LOOK_UP:
			if (verticalLookAngle_ < maximumAngle)
			{
				camera_->tilt(lookAngle);
				verticalLookAngle_ += lookAngle;
			}
			break;
		case LOOK_DOWN:
			if (verticalLookAngle_ > -maximumAngle)
			{
				camera_->tilt(-lookAngle);
				verticalLookAngle_ -= lookAngle;
			}
			break;
		case LOOK_LEFT:
			camera_->pan(lookAngle);
			break;
		case LOOK_RIGHT:
			camera_->pan(-lookAngle);
			break;
	}
}

