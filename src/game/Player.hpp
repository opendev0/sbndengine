#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.hpp"

class Player
{
private:
	iRef<iPhysicsObject> physicsObject;
	cCamera3rdPerson &camera;

	// Separate velocities to not bother with collision impulses
	float velocity;
	float angularVelocity;

	iEngine &engine;


public:
	Player(iRef<iPhysicsObject> physicsObject, cCamera3rdPerson &camera, iEngine &engine)
		: physicsObject(physicsObject), camera(camera), engine(engine)
	{
		camera.setup(getPosition(), CVector<3, float> (0, 2, 3));
	}

	inline CVector<3, float> getPosition()
	{
		return physicsObject->object->position;
	}

	inline void setPosition(const CVector<3, float> &pos)
	{
		physicsObject->object->position = pos;
	}

	inline void setVelocity(float velocity)
	{
		this->velocity = velocity;
	}

	inline void setAngularVelocity(float angularVelocity)
	{
		this->angularVelocity = angularVelocity;
	}

	void move()
	{
		physicsObject->object->translate(CVector<3, float>(0, 0, velocity) * camera.view_matrix * engine.time.frame_elapsed_seconds);
		physicsObject->object->rotate(CVector<3, float>(0, 1, 0), -angularVelocity * engine.time.frame_elapsed_seconds);

		camera.update(physicsObject->object->position);
		camera.rotate(angularVelocity * engine.time.frame_elapsed_seconds);
		camera.frustum(-1.5f, 1.5f, -1.5f * engine.window.aspect_ratio, 1.5f * engine.window.aspect_ratio, 1, 100);
		camera.computeMatrices();
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
