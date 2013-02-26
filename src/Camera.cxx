#include "Camera.h"

Camera::Camera() :
	near_(1.0f),
	far_(1000.0f),
	pixelAspect_(1.33f),
	fov_(60.0f),
	eye_(0.0f, 0.0f, -1.0f), 
	up_(0.0f, 1.0f, 0.0f), 
	right_(1.0f, 0.0f, 0.0f), 
	direction_(0.0f, 0.0f, 1.0f)
{
}

/*
build either an orthographic or perspective projection matrix
the resulting matrix is similar to what glOrtho() and glFrustum() would build
*/
void Camera::createProjection() // active scene bound
{
	float aspect = pixelAspect_;
	/*
		glFrustum(left, right, bottom, top, near, far)
		glFrustum(-aspect, aspect, -1., 1., _viewport->_camera->near(), _viewport->_camera->far());

		[(2*near)/(right-left)	0						A	0]
		[0						(2*near)/(top-bottom)	B	0]
		[0						0						C	D]
		[0						0						-1	0]

		A = (right + left) / (right - left)
		B = (top + bottom) / (top - bottom)
		C = (far + near) / (far - near)
		D = (2 * near * far) / (far - near)

		instead of doing what glFrustum does we could do gluPerspective
		which does the same as the frustum method
	*/
	// gluPerspective part
	float xmin, xmax, ymin, ymax;
	ymax = near_ * tan(fov_ * PI / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	// glFrustum part
	float left = xmin;
	float right = xmax;
	float bottom = ymin;
	float top = ymax;
	float x = (2.0f * near_) / (right - left);
	float y = (2.0f * near_) / (top - bottom);
	float A = (right + left) / (right - left);
	float B = (top + bottom) / (top - bottom);
	float C = -(far_ + near_) / (far_ - near_);
	float D = -(2.0f * far_ * near_) / (far_ - near_);

	projection_[0] = x;
	projection_[1] = 0.0f;
	projection_[2] = A;
	projection_[3] = 0.0f;
	projection_[4] = 0.0f;
	projection_[5] = y;
	projection_[6] = B;
	projection_[7] = 0.0f;
	projection_[8] = 0.0f;
	projection_[9] = 0.0f;
	projection_[10] = C;
	projection_[11] = D;
	projection_[12] = 0.0f;
	projection_[13] = 0.0f;
	projection_[14] = -1.0f;
	projection_[15] = 0.0f;
}

void Camera::createView()
{
	// set viewing matrix
	view_.identity();
	v3D::Vector3 e = -eye_;

	v3D::Matrix4 rot = rotation_.matrix().transpose();
	view_.translate(e[0], e[1], e[2]);
	view_ *= rot;
}

v3D::Matrix4 Camera::view() const
{
	return view_;
}

v3D::Matrix4 Camera::projection() const
{
	return projection_;
}

void Camera::eye(const v3D::Vector3 & position)
{
	eye_ = position;
}

/*
	pedestal - move eye on up axis - move up/down
	pedestal - same as truck but use up vector instead of right vector
*/
void Camera::pedestal(float delta)
{
	v3D::Vector3 ed = up_ * delta;
	eye_ += ed;
}

/*
	pan - move horizontally around a fixed axis (the camera's y axis) - camera rotation & look at position changes 
			but eye position doesn't - look left/right
	pan and tilt are types of rotation with restrictions
*/
void Camera::pan(float angle)
{
	v3D::Vector3 axis(0.0, 1.0, 0.0);
	v3D::Quaternion local_rotation;
	local_rotation.rotation(axis, angle);
	v3D::Quaternion total;
	total = rotation_;
	total = total * local_rotation;
	rotation_ = total;
}

/*
	tilt - move vertically around a fixed axis (camera's x axis) - look up/down
*/
void Camera::tilt(float angle)
{
	v3D::Vector3 axis(1.0, 0.0, 0.0);
	v3D::Quaternion local_rotation;
	local_rotation.rotation(axis, angle);
	v3D::Quaternion total;
	total = rotation_;
	total = total * local_rotation;
	rotation_ = total;
}

/*
	dolly - move eye forward or backward along direction of view
	dolly, truck, pedestal are types of translations with special restrictions
	dolly - same as pedestal but use direction vector instead of up vector
*/
void Camera::dolly(float d)
{
	v3D::Vector3 ed = direction_ * d;
	eye_ += ed;
}

/*
	truck - move eye on axis perpendicular to direction of view and up axis - move left/right
	truck - multiply delta value and right vector to get eye delta - right vector must be 
	normalized - add eye delta to current eye position
*/
void Camera::truck(float delta)
{
	v3D::Vector3 ed = right_ * delta;
	eye_ += ed;
}
