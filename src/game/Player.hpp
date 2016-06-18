#ifndef PLAYER_H
#define PLAYER_H

//#include "Direction.hpp"

class Player
{
private:
	iRef<iPhysicsObject> physicsObject;
	cCamera3rdPerson &camera;
	
	float velocity;
	float angularVelocity;

	iEngine &engine;


public:
	Player(const iRef<iPhysicsObject> &physicsObject, cCamera3rdPerson &camera, iEngine &engine)
		: physicsObject(physicsObject), camera(camera), engine(engine)
	{
	}
	
	
	inline iRef<iPhysicsObject> getPhysicsObject() 
	{
		return physicsObject;
	}
	
	inline CVector<3, float> getPosition() 
	{
		return physicsObject->object->position;
	}
	
	inline float getAngularVelocity() 
	{
		return angularVelocity;
	}
	
	inline void setAngularVelocity(float vel)
	{
		angularVelocity = vel;
	}
	
	
	/*
	 * reset position, rotation and velocities of the player to zero
	 */
	void reset()
	{
		physicsObject->object->position = CVector<3, float> ();
		physicsObject->velocity = CVector<3, float> ();
		
		physicsObject->object->rotation = CQuaternion<float> ();
		physicsObject->angular_velocity = CVector<3, float> ();
	}
	
	
	/*
	 * translate the player by dist
	 */
	inline void translate(CVector<3, float> dist) 
	{
		physicsObject->object->translate(dist);
	}
	
	/*
	 * rotate the player by degree[0] around x-axis, degree[1] around y-axis, and degree[2] around z-axis
	 */
	void rotate(CVector<3, float> degree)
	{
		physicsObject->object->rotate(CVector<3, float> (1, 0, 0), degree[0]);
		physicsObject->object->rotate(CVector<3, float> (0, 1, 0), degree[1]);
		physicsObject->object->rotate(CVector<3, float> (0, 0, 1), degree[2]);
	}

	inline void jump()
	{
		physicsObject->velocity[1] = 7;
	}



	bool checkCollision(iRef<iPhysicsObject> object)
	{
		/* Proposal:
		 * 1) Save collisions before iterative resolution, so they can be fetched from the engine (look at debug-collisions)
		 * 2) Run object against collision list
		 */

		 return false;
	}
};

#endif // PLAYER_H
