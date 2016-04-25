#ifndef __I_CAMERA_1ST_PERSON_HPP__
#define __I_CAMERA_1ST_PERSON_HPP__

#include "iCamera.hpp"
#include "libmath/CGlSlMath.hpp"
#include "libmath/CQuaternion.hpp"

/**
 * this implements a 1st person free flight cammera.
 *
 * e. g. its possible to freely rotate, move to the right, left according
 * to the current cammera angle.
 */
class cCamera1stPerson: public iCamera
{
	/**
	 * center of camera
	 */
	CVector<3,float> position;

	/**
	 * angles of camera rotation
	 */
	float angle_x, angle_y, angle_z;

public:

	/**
	 * reset to origin and zero rotation
	 */
	inline void reset()
	{
		position.setZero();
		view_matrix.loadIdentity();
		angle_x = angle_y = angle_z = 0;
	}

	cCamera1stPerson()
	{
		reset();
	}

	/**
	 * return the current camera position
	 */
	inline const CVector<3,float> getPosition()	const
	{
		return position;
	}

	/**
	 * move the player relative to the current view
	 */
	inline void moveRelative(const CVector<3,float> &movement)
	{
		position += movement*CMatrix3<float>(view_matrix);
	}

	/**
	 * set the position of the player
	 */
	inline void setPosition(const CVector<3,float> &p_position)
	{
		position = p_position;
	}

	/**
	 * rotate the player using directly a quaternion
	 */
	/*
	inline void rotate(const CQuaternion<float> &quaternion)
	{
		rotation_quaternion *= quaternion;
	}
	*/

	/**
	 * reset the rotation
	 */
	inline void resetRotation()
	{
//		rotation_quaternion.reset();
		angle_x = angle_y = angle_z = 0;
	}

	/**
	 * rotate the player around axes
	 */
	inline void rotate(float p_angle_x, float p_angle_y, float p_angle_z)
	{
		angle_x += p_angle_x;
		angle_y += p_angle_y;
		angle_z += p_angle_z;
	}

	inline void computeMatrices()
	{
		/**
		 * rotation of camera
		 */
		CQuaternion<float> rotation_quaternion;

		rotation_quaternion.setRotation(CVector<3,float>(0, 1, 0), angle_y);
		rotation_quaternion.rotatePost(CVector<3,float>(1, 0, 0), angle_x);

		view_matrix = CMatrix4<float>(rotation_quaternion.getRotationMatrix())*GLSL::translate(-position);
	}
};

#endif
