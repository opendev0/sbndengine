#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "src/include/libmath/CVector3.hpp"
#include "src/sbndengine/physics/cPhysicsCollisionData.hpp"

class Enemy
{
public:
	Enemy(iRef<iPhysicsObject> physicsObject, const iEngine &engine) : physicsObject(physicsObject), engine(engine), velocity(4)
	{
		// Set inverse mass to 0, so no gravitation or other external forces are applied to the object
		physicsObject->inv_mass = 0;
	}

	inline CVector<3, float> getPosition() const
	{
		return physicsObject->object->position;
	}

	inline CVector<3, float> getVelocity() const
	{
		return physicsObject->velocity;
	}

	inline void setVelocity()
	{
		this->velocity = velocity;
	}

	virtual void move() = 0;

	inline bool defeated(CPhysicsCollisionData &collision) const
	{
		return collision.collision_normal.dotProd(engine.physics.getGravitation()) * ((physicsObject == collision.physics_object1) ? (-1) : 1) > 0;
	}

	inline bool operator==(const iRef<iPhysicsObject> &o) const
	{
		return (o == physicsObject);
	}

	inline bool operator!=(const iRef<iPhysicsObject> &o) const
	{
		return !(o == physicsObject);
	}

protected:
	iRef<iPhysicsObject> physicsObject;
	const iEngine &engine;
	float velocity;
};

#endif // ENEMY_HPP
