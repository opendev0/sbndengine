/*
 * Copyright 2010 Martin Schreiber
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cPhysicsIntersections.hpp"
#include "sbndengine/physics/iPhysicsObject.hpp"
#include "sbndengine/engine/cObjectFactoryBox.hpp"
#include "sbndengine/engine/cObjectFactoryPlane.hpp"
#include "sbndengine/engine/cObjectFactorySphere.hpp"
#include "gamemath.hpp"
#include "worksheets_precompiler.hpp"



/**
 * LAB WORKSHEET 2, ASSIGNMENT 1
 *
 * this is the easiest case.
 *
 * since we previously tested the collision with the bounding sphere, those 2 objects already collide.
 * therefore we just compute the interpenetration depth.
 */
bool CPhysicsIntersections::sphereSphere(iPhysicsObject &physics_object1, iPhysicsObject &physics_object2, CPhysicsCollisionData &c)
{
#if WORKSHEET_2
	c.physics_object1 = &physics_object1;
	c.physics_object2 = &physics_object2;
	c.collision_normal = (physics_object2.object->position - physics_object1.object->position).getNormalized();
	c.collision_point1 = physics_object1.object->position + c.collision_normal * physics_object1.object->objectFactory->bounding_sphere_radius;
	c.collision_point2 = physics_object2.object->position - c.collision_normal * physics_object2.object->objectFactory->bounding_sphere_radius;
	c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();

	return true;
#else
	return false;
#endif
}

/**
 * LAB WORKSHEET 2, ASSIGNMENT 2
 *
 * the idea to compute the distance between the sphere
 * center and a plane is to use a change of basis.
 *
 * After projecting the sphere into world space, we simply use the
 * inverse model matrix of the plane to project the sphere
 * to the "plane basis".
 *
 * Since the plane factory builds up the plane aligned along the
 * x and z axis, there's no need to use another "plane space"
 * basis change!
 */
bool CPhysicsIntersections::spherePlane(iPhysicsObject &physics_object_sphere, iPhysicsObject &physics_object_plane, CPhysicsCollisionData &c)
{
#if WORKSHEET_2
	cObjectFactoryPlane &planeFactory = *static_cast<cObjectFactoryPlane *>(&physics_object_plane.object->objectFactory.getClass());
	float sphereRadius = physics_object_sphere.object->objectFactory->bounding_sphere_radius;
	CMatrix4<float> &planeMatrix = physics_object_plane.object->model_matrix;
	vec4f spherePos = physics_object_plane.object->inverse_model_matrix * physics_object_sphere.object->position;

	// Check if sphere is inside plane bounds
	if (spherePos[1] > sphereRadius
			|| spherePos[0] + sphereRadius < 0 || spherePos[0] - sphereRadius > planeFactory.size_x
			|| spherePos[2] + sphereRadius < 0 || spherePos[2] - sphereRadius > planeFactory.size_z)
		return false;

	std::cout << "Sphere-plane collision detected" << std::endl;

	c.physics_object1 = &physics_object_plane;
	c.physics_object2 = &physics_object_sphere;
	c.collision_normal = physics_object_plane.object->inverse_model_matrix.getTranspose() * Vector(0, 1, 0);
	c.collision_point1 = planeMatrix * Vector(spherePos[0], 0, spherePos[2]);
	c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
	c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();


#else
	return false;
#endif
}


/**
 * LAB WORKSHEET 4, ASSIGNMENT 1
 *
 * compute the intersection between a sphere and a box
 */
bool CPhysicsIntersections::sphereBox(iPhysicsObject &physics_object_sphere, iPhysicsObject &physics_object_box, CPhysicsCollisionData &c)
{
#if WORKSHEET_4

#else
	return false;
#endif
}


bool CPhysicsIntersections::planePlane(iPhysicsObject &physics_object_plane1, iPhysicsObject &physics_object_plane2, CPhysicsCollisionData &c)
{
	/*
	 * meaningless implementation
	 */
	std::cerr << "TODO: meaningless implementation" << std::endl;
	return false;
}



/**
 * LAB WORKSHEET 4, ASSIGNMENT 2
 *
 * compute the intersection between a plane and a box
 */
bool CPhysicsIntersections::planeBox(iPhysicsObject &physics_object_plane, iPhysicsObject &physics_object_box, CPhysicsCollisionData &c)
{
#if WORKSHEET_4

#else
	return false;
#endif
}

/**
 * LAB WORKSHEET 5, ASSIGNMENT 1
 *
 * compute the intersection between a plane and a box
 */
bool CPhysicsIntersections::boxBox(iPhysicsObject &physics_object_box1, iPhysicsObject &physics_object_box2, CPhysicsCollisionData &c)
{
#if WORKSHEET_5

#else
	return false;
#endif
}



/**
 * compute the collision data for 2 objects if there is an intersection.
 *
 * the collision data is inserted to the collision list.
 */
bool CPhysicsIntersections::multiplexer(iPhysicsObject &physics_object1, iPhysicsObject &physics_object2, CPhysicsCollisionData &collisionData)
{
	/**
	 * we have to care about the possible combinations (sphere, plane, box, plane, ...)
	 * to reduce the number of implementations, we "sort" them in a good order (e. g. replace plane-sphere test to a sphere-plane test)
	 *
	 * TODO: if the number of possibilities grow, replace this with a method address matrix
	 */
	switch (physics_object1.object->objectFactory->type)
	{
		case iObjectFactory::TYPE_SPHERE:
			switch (physics_object2.object->objectFactory->type)
			{
				case iObjectFactory::TYPE_SPHERE:
					return sphereSphere(physics_object1, physics_object2, collisionData);
					break;
				case iObjectFactory::TYPE_PLANE:
					return spherePlane(physics_object1, physics_object2, collisionData);
					break;
				case iObjectFactory::TYPE_BOX:
					return sphereBox(physics_object1, physics_object2, collisionData);
					break;
			}
			break;

		case iObjectFactory::TYPE_PLANE:
			switch (physics_object2.object->objectFactory->type)
			{
				case iObjectFactory::TYPE_SPHERE:
					return spherePlane(physics_object2, physics_object1, collisionData);	// switch o1&o2 !!!
					break;
				case iObjectFactory::TYPE_PLANE:
					return planePlane(physics_object1, physics_object2, collisionData);
					break;
				case iObjectFactory::TYPE_BOX:
					return planeBox(physics_object1, physics_object2, collisionData);
					break;
			}
			break;

		case iObjectFactory::TYPE_BOX:
			switch (physics_object2.object->objectFactory->type)
			{
				case iObjectFactory::TYPE_SPHERE:
					return sphereBox(physics_object2, physics_object1, collisionData);	// switch o1&o2 !!!
					break;
				case iObjectFactory::TYPE_PLANE:
					return planeBox(physics_object2, physics_object1, collisionData);	// switch o1&o2 !!!
					break;
				case iObjectFactory::TYPE_BOX:
					return boxBox(physics_object1, physics_object2, collisionData);
					break;
			}
			break;

	default:
		std::cerr << "ERROR" << std::endl;
	}
	return false;
}
