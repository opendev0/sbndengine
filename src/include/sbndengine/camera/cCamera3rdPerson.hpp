#ifndef __I_CAMERA_3RD_PERSON_HPP__
#define __I_CAMERA_3RD_PERSON_HPP__

#include "iCamera.hpp"
#include "libmath/CGlSlMath.hpp"
#include "cCamera1stPerson.hpp"

class cCamera3rdPerson : public iCamera 
{
	CVector<3, float> distance;
	CVector<3, float> position;
	
	bool mode;

public:

	cCamera3rdPerson()
	{
		reset();
		mode = true;
	}
	
	void reset() 
	{
		view_matrix.loadIdentity();
		position = CVector<3,float>();
	}
	
	inline void setup(CVector<3, float> dist) 
	{
		distance = dist;
	}
	
	inline void update(iRef<iPhysicsObject> physicsObject) {
		CQuaternion<float> quarternion;
		if (mode) {
			//only object rotation around the y axis is considered
			quarternion = physicsObject->object->rotation;
			quarternion.i = 0;
			quarternion.j = -quarternion.j;
			quarternion.k = 0;
			quarternion.normalize();
		}
		else {
			quarternion.reset();
		}
		
		std::cout << quarternion.getRotationMatrix() << std::endl;
		
		view_matrix = GLSL::translate(-distance)*quarternion.getRotationMatrix()*GLSL::translate(-physicsObject->object->position);
	}
	
	const CVector<3,float> getPosition() const {
		return position;
	}
	
	inline void changeMode() {
		mode = !mode;
	}
	
	void computeMatrices()
	{
		/**
		 * rotation of camera
		 */
	}
};

#endif