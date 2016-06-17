#ifndef __I_CAMERA_3RD_PERSON_HPP__
#define __I_CAMERA_3RD_PERSON_HPP__

#include "iCamera.hpp"
#include "libmath/CGlSlMath.hpp"
#include "cCamera1stPerson.hpp"

class cCamera3rdPerson : public iCamera 
{
	CVector<3, float> distance;
	CVector<3, float> position;

public:

	cCamera3rdPerson()
	{
		view_matrix.loadIdentity();
		position = CVector<3,float>();
	}
	
	inline void setup(CVector<3, float> dist) 
	{
		distance = dist;
	}
	
	inline void update(iRef<iPhysicsObject> physicsObject) {
		
		//only object rotation around the y axis is considered
		CQuaternion<float> quarternion = physicsObject->object->rotation;
		quarternion.i = 0;
		quarternion.j = -quarternion.j;
		quarternion.k = 0;
		quarternion.normalize();
		
		view_matrix = GLSL::translate(-distance)*quarternion.getRotationMatrix()*GLSL::translate(-physicsObject->object->position);
	}
	
	const CVector<3,float> getPosition() const {
		return position;
	}
	
	void computeMatrices()
	{
		/**
		 * rotation of camera
		 */
	}
};

#endif