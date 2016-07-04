#ifndef __I_CAMERA_3RD_PERSON_HPP__
#define __I_CAMERA_3RD_PERSON_HPP__

#include "iCamera.hpp"
#include "libmath/CGlSlMath.hpp"
#include "cCamera1stPerson.hpp"

class cCamera3rdPerson : public iCamera 
{
	CVector<3, float> distance;
	CVector<3, float> position;
	float angle_x, angle_y, angle_z;

public:

	cCamera3rdPerson()
	{
		reset();
	}
	
	void reset() 
	{
		view_matrix.loadIdentity();
		position = CVector<3,float>();
	}
	
	inline void setup(CVector<3, float> position, CVector<3, float> dist) 
	{
		reset();
		angle_x = angle_y = angle_z = 0;
		distance = dist;
		setPosition(position);
		computeMatrices();
		moveRelative(dist);
	}
	
	inline void update(CVector<3, float> position) {
		
		reset();
		setPosition(position);
		computeMatrices();
		moveRelative(distance);
		computeMatrices();
	}
	
	const CVector<3,float> getPosition() const {
		return position;
	}
	
	inline void moveRelative(const CVector<3, float> &movement)
	{
		position += movement * CMatrix3<float>(view_matrix);
	}
	
	inline void setPosition(const CVector<3, float> &p_position)
	{
		position = p_position;
	}
	
	inline void rotate(float deg)
	{
		angle_y += deg;
	}
	
	void computeMatrices()
	{
		/**
		 * rotation of camera
		 */
		CQuaternion<float> rotation_quaternion;
		
		rotation_quaternion.setRotation(CVector<3, float>(0, 1, 0), angle_y);
		
		view_matrix = CMatrix4<float>(rotation_quaternion.getRotationMatrix()) * GLSL::translate(-position);
	}
};

#endif