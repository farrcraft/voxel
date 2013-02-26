/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <3dtypes/Vector3.h>
#include <3dtypes/Matrix4.h>
#include <3dtypes/Quaternion.h>

class Camera
{
	public:
		Camera();

		void eye(const v3D::Vector3 & position);

		// get
		v3D::Matrix4	projection() const;
		v3D::Matrix4 view() const;

		/**
 		 *	Create a projection matrix.
		 *	The resulting matrix can be retrieved by calling projection().
		 *	If the camera is set to orthographic then an orthographic projection will be created.
		 *	Otherwise a perspective projection will be used.
		 */
		void createProjection();
		/**
		 *	Create a viewing matrix.
		 *	The resulting matrix can be retrieved by calling view().
		 *	The viewing matrix contains the translation and rotation portion of  the 
		 *	camera transformation.
		 */
		void createView();

		/**
	 	 *	Pedestal the camera.
		 *	Move eye on up axis - move up/down
		 *	same as truck but use up vector instead of right vector
		 */
		void pedestal(float d);
		/**
		 *	Dolly the camera.
		 *	Move eye forward or backward along direction of view
		 *	same as pedestal but use direction vector instead of up vector
		 */
		void dolly(float d);
		/**
		 *	Truck the camera.
		 *	Move eye on axis perpendicular to direction of view and up axis - move 
		 *	left/right multiply delta value and right vector to get eye delta - 
		 *	right vector must be normalized - add eye delta to current eye position
		 */
		void truck(float d);
		/**
		 *	Pan the camera.
		 *	Move horizontally around a fixed axis (the camera's y axis) - camera 
		 *	rotation & look at position changes but eye position doesn't - look left/right
		 */
		void pan(float angle);
		/**
		 *	Tilt the camera.
		 *	Move vertically around a fixed axis (camera's x axis) - look up/down
		 */
		void tilt(float angle);

	private:
		float				near_;
		float				far_;
		float				pixelAspect_;
		float				fov_;
		v3D::Vector3		eye_;
		v3D::Vector3		up_;
		v3D::Vector3		direction_;
		v3D::Vector3		right_;
		v3D::Quaternion		rotation_;
		v3D::Matrix4		projection_;
		v3D::Matrix4		view_;			// viewing transformation
};