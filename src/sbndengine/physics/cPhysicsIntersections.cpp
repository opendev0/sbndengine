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
	float radius1 = static_cast<cObjectFactorySphere *>(&physics_object1.object->objectFactory.getClass())->radius;
	float radius2 = static_cast<cObjectFactorySphere *>(&physics_object2.object->objectFactory.getClass())->radius;
	
	c.physics_object1 = &physics_object1;
	c.physics_object2 = &physics_object2;
	c.collision_normal = (physics_object2.object->position - physics_object1.object->position).getNormalized();
	c.collision_point1 = physics_object1.object->position + c.collision_normal * radius1;
	c.collision_point2 = physics_object2.object->position - c.collision_normal * radius2;
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
	float sphereRadius = static_cast<cObjectFactorySphere *>(&physics_object_sphere.object->objectFactory.getClass())->radius;
	CMatrix4<float> &planeMatrix = physics_object_plane.object->model_matrix;
	vec4f spherePos = physics_object_plane.object->inverse_model_matrix * physics_object_sphere.object->position;

	// Check if sphere is inside plane bounds
	if (spherePos[1] > sphereRadius
			|| fabs(spherePos[0]) - sphereRadius > planeFactory.size_x / 2 || fabs(spherePos[2]) - sphereRadius > planeFactory.size_z / 2)
		return false;

	c.physics_object1 = &physics_object_plane;
	c.physics_object2 = &physics_object_sphere;
	c.collision_normal = physics_object_plane.object->inverse_model_matrix.getTranspose() * Vector(0, 1, 0);
	c.collision_point1 = planeMatrix * Vector(spherePos[0], 0, spherePos[2]);
	c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
	c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
	
	return true;
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
    float sphereRadius = static_cast<cObjectFactorySphere *>(&physics_object_sphere.object->objectFactory.getClass())->radius;
    vec4f spherePos = physics_object_box.object->inverse_model_matrix * physics_object_sphere.object->position;
    Vector boxHalfSize = static_cast<cObjectFactoryBox *>(&physics_object_box.object->objectFactory.getClass())->half_size;
    
    c.physics_object1 = &physics_object_box;
    c.physics_object2 = &physics_object_sphere;
    
    //planes
    //yz-plane
    if (fabs(spherePos[0]) < sphereRadius + boxHalfSize[0] && fabs(spherePos[1]) < boxHalfSize[1] && fabs(spherePos[2]) < boxHalfSize[2]) {
                    
        int sgn = (spherePos[0] >= 0) - (spherePos[0] < 0);
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(sgn, 0, 0);
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(sgn*boxHalfSize[0], spherePos[1], spherePos[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    //xz-plane
    if (fabs(spherePos[1]) < sphereRadius + boxHalfSize[1] && fabs(spherePos[0]) < boxHalfSize[0] && fabs(spherePos[2]) < boxHalfSize[2]) {
                   
        int sgn = (spherePos[1] >= 0) - (spherePos[1] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(0, sgn, 0);
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(spherePos[0], sgn*boxHalfSize[1], spherePos[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    //xy-plane
    if (fabs(spherePos[2]) < sphereRadius + boxHalfSize[2] && fabs(spherePos[0]) < boxHalfSize[0] && fabs(spherePos[1]) < boxHalfSize[1]) {
                    
        int sgn = (spherePos[2] >= 0) - (spherePos[2] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(0, 0, sgn);
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(spherePos[0], spherePos[1], sgn*boxHalfSize[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    
    //edges
    //edges parallel to x
    if (fabs(spherePos[0]) < boxHalfSize[0] && vec2f(fabs(spherePos[1]) - boxHalfSize[1], fabs(spherePos[2]) - boxHalfSize[2]).length() - sphereRadius <= 0) {
        
        int ySgn = (spherePos[1] >= 0) - (spherePos[1] < 0);
        int zSgn = (spherePos[2] >= 0) - (spherePos[2] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(0, spherePos[1] - ySgn*boxHalfSize[1], spherePos[2] - ySgn*boxHalfSize[2]).getNormalized();
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(spherePos[0], ySgn*boxHalfSize[1], zSgn*boxHalfSize[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }

    //edges parallel to y
    if (fabs(spherePos[1]) < boxHalfSize[1] && vec2f(fabs(spherePos[0]) - boxHalfSize[0], fabs(spherePos[2]) - boxHalfSize[2]).length() - sphereRadius <= 0) {
        
        int xSgn = (spherePos[0] >= 0) - (spherePos[0] < 0);
        int zSgn = (spherePos[2] >= 0) - (spherePos[2] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(spherePos[0] - xSgn*boxHalfSize[0], 0, spherePos[2] - zSgn*boxHalfSize[2]).getNormalized();
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(xSgn*boxHalfSize[0], spherePos[1], zSgn*boxHalfSize[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    //edges parallel to z
    if (fabs(spherePos[2]) < boxHalfSize[2] && vec2f(fabs(spherePos[0]) - boxHalfSize[0], fabs(spherePos[1]) - boxHalfSize[1]).length() - sphereRadius <= 0) {
        
        int xSgn = (spherePos[0] >= 0) - (spherePos[0] < 0);
        int ySgn = (spherePos[1] >= 0) - (spherePos[1] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(spherePos[0] - xSgn*boxHalfSize[0], spherePos[1] - ySgn*boxHalfSize[1], 0).getNormalized();
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(xSgn*boxHalfSize[0], ySgn*boxHalfSize[1], spherePos[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal *sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    
    //corners
    if (Vector(fabs(spherePos[0]) - boxHalfSize[0], fabs(spherePos[1]) - boxHalfSize[1], fabs(spherePos[2]) - boxHalfSize[2]).getLength() - sphereRadius <= 0) {
        
        int xSgn = (spherePos[0] >= 0) - (spherePos[0] < 0);
        int ySgn = (spherePos[1] >= 0) - (spherePos[1] < 0);
        int zSgn = (spherePos[2] >= 0) - (spherePos[2] < 0);
        
        c.collision_normal = physics_object_box.object->inverse_model_matrix.getTranspose() * Vector(spherePos[0] - xSgn*boxHalfSize[0], spherePos[1] - ySgn*boxHalfSize[1], spherePos[2] - zSgn*boxHalfSize[2]).getNormalized();
        c.collision_point1 = physics_object_box.object->model_matrix * Vector(xSgn*boxHalfSize[0], ySgn*boxHalfSize[1], zSgn*boxHalfSize[2]);
        c.collision_point2 = physics_object_sphere.object->position - (c.collision_normal * sphereRadius);
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
        
        return true;
    }
    
    return false;
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
    iRef<iObject> box = physics_object_box.object;
    Vector boxHalfSize = static_cast<cObjectFactoryBox *>(&box->objectFactory.getClass())->half_size;
    iRef<iObject> plane = physics_object_plane.object;
    cObjectFactoryPlane &planeFactory = *static_cast<cObjectFactoryPlane *>(&physics_object_plane.object->objectFactory.getClass());
    
    int sideOfPlane = 0;
    float maxPenetrationBelowPlane = 0;
    float maxPenetrationAbovePlane = 0;
    Vector maxPenetrationVertexBelowPlane;
    Vector maxPenetrationVertexAbovePlane;
    
    std::list<Vector> vertecesOutsidePlane;
    
    
    Vector vertexList[8] = {Vector(-boxHalfSize[0], -boxHalfSize[1], -boxHalfSize[2]), Vector(-boxHalfSize[0], -boxHalfSize[1], boxHalfSize[2]), 
                            Vector(-boxHalfSize[0], boxHalfSize[1], -boxHalfSize[2]), Vector(-boxHalfSize[0], boxHalfSize[1], boxHalfSize[2]),
                            Vector(boxHalfSize[0], -boxHalfSize[1], -boxHalfSize[2]), Vector(boxHalfSize[0], -boxHalfSize[1], boxHalfSize[2]), 
                            Vector(boxHalfSize[0], boxHalfSize[1], -boxHalfSize[2]), Vector(boxHalfSize[0], boxHalfSize[1], boxHalfSize[2])};
    
    for (Vector* arr = vertexList; arr != vertexList + 8; arr++) {
        Vector current = plane->inverse_model_matrix * box->model_matrix * *arr;
        
        //vertex is outside of plane
        if (fabs(current[0]) > planeFactory.size_x / 2 || fabs(current[2]) > planeFactory.size_z / 2) {
            
            vertecesOutsidePlane.push_back(*arr);                       
            if (current[1] <= 0) {
                sideOfPlane--;
            }
            else {
                sideOfPlane++;
            }
            continue;
        }
        
        //vertex is below plane
        if (current[1] <= 0) {
            sideOfPlane--;
            //vertex is the farthest below plane
            if (current[1] < maxPenetrationBelowPlane) {
                maxPenetrationBelowPlane = current[1];
                maxPenetrationVertexBelowPlane = current;
            }
        }
        //vertex is above or in plane
        else {
            sideOfPlane++;
            //vertex is the farthest above plane
            if (current[1] > maxPenetrationAbovePlane) {
                maxPenetrationAbovePlane = current[1];
                maxPenetrationVertexAbovePlane = current;
            }
        }
    }

    if (abs(sideOfPlane) + vertecesOutsidePlane.size() == 8 || vertecesOutsidePlane.size() == 8) {
        return false;
    }

    c.physics_object1 = &physics_object_plane;
    c.physics_object2 = &physics_object_box;
    
    if (!vertecesOutsidePlane.empty()) {
        Vector calculateNeighbors[3] = {Vector(-1, 1, 1), Vector(1, -1, 1), Vector(1, 1, -1)};
        for (std::list<Vector>::iterator it = vertecesOutsidePlane.begin(); it != vertecesOutsidePlane.end(); ++it) {
            Vector current = plane->inverse_model_matrix * box->model_matrix * *it;
            
            
            if (sideOfPlane < 0 && current[1] >= maxPenetrationAbovePlane && current[1] >= 0) {
                for (Vector* factor = calculateNeighbors; factor != calculateNeighbors + 3; factor++) {
                    Vector neighbour = plane->inverse_model_matrix * box->model_matrix * (current * *factor);
                    if (!(fabs(neighbour[0]) > planeFactory.size_x / 2 || fabs(neighbour[2]) > planeFactory.size_z / 2) && neighbour[1] < 0) {
                        Vector u = (neighbour - current).getNormalized();
                        Vector v = Vector(0, 0, planeFactory.size_z).getNormalized();
                        
                        float a = u.dotProd(u);
                        float b = u.dotProd(v);
                        float c2 = v.dotProd(v);
                        float d = u.dotProd(current - Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2));
                        float e = v.dotProd(current - Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2));
                        float boxClosest = (b*e - c2*d)/(a*c2 - b*b);
                        float planeClosest = (a*e - b*d)/(a*c2 - b*b);
                        
                        Vector boxClosestVertex = current + u*boxClosest;
                        Vector planeClosestVertex = Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2) + v*planeClosest;
                        
                        if (boxClosestVertex[1] > 0) {
                            std::cout << (boxClosestVertex - planeClosestVertex).getLength() << std::endl;
                            c.collision_normal = plane->inverse_model_matrix.getTranspose() * (planeClosestVertex - boxClosestVertex);
                            c.collision_point1 = plane->model_matrix * planeClosestVertex;
                            c.collision_point2 = plane->model_matrix * boxClosestVertex;
                            c.interpenetration_depth = (c.collision_point1 - c.collision_point2).getLength();
                            return true;
                        }

                    }
                }
            }
            
            else if (sideOfPlane >= 0 && current[1] < maxPenetrationBelowPlane && current[1] < 0) {
                for (Vector* factor = calculateNeighbors; factor != calculateNeighbors + 3; factor++) {
                    Vector neighbour = plane->inverse_model_matrix * box->model_matrix * (current * *factor);
                    if (!(fabs(neighbour[0]) > planeFactory.size_x / 2 || fabs(neighbour[2]) > planeFactory.size_z / 2) && neighbour[1] >= 0) {
                        Vector u = (neighbour - current).getNormalized();
                        Vector v = Vector(0, 0, planeFactory.size_z).getNormalized();
                        
                        float a = u.dotProd(u);
                        float b = u.dotProd(v);
                        float c2 = v.dotProd(v);
                        float d = u.dotProd(current - Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2));
                        float e = v.dotProd(current - Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2));
                        float boxClosest = (b*e - c2*d)/(a*c2 - b*b);
                        float planeClosest = (a*e - b*d)/(a*c2 - b*b);
                        
                        Vector boxClosestVertex = current + u*boxClosest;
                        Vector planeClosestVertex = Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2) + v*planeClosest;
                        
                        if (boxClosestVertex[1] <= 0) {
                            std::cout << (boxClosestVertex - planeClosestVertex).getLength() << std::endl;
                            c.collision_normal = plane->inverse_model_matrix.getTranspose() * (planeClosestVertex - boxClosestVertex);
                            c.collision_point1 = plane->model_matrix * planeClosestVertex;
                            c.collision_point2 = plane->model_matrix * boxClosestVertex;
                            c.interpenetration_depth = (c.collision_point1 - c.collision_point2).getLength();
                            return true;
                        }
                    }
                }
            }
        }
        
        return false;
    }
    
    //more points below plane than above => move box downwards
    if (sideOfPlane < 0) {
        c.collision_normal = plane->inverse_model_matrix.getTranspose() * Vector(0, -1, 0);
        c.collision_point1 = plane->model_matrix * Vector(maxPenetrationVertexAbovePlane[0], 0, maxPenetrationVertexAbovePlane[2]);
        c.collision_point2 = plane->model_matrix * maxPenetrationVertexAbovePlane;
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
    }
    else {
        c.collision_normal = plane->inverse_model_matrix.getTranspose() * Vector(0, 1, 0);
        c.collision_point1 = plane->model_matrix * Vector(maxPenetrationVertexBelowPlane[0], 0, maxPenetrationVertexBelowPlane[2]);
        c.collision_point2 = plane->model_matrix * maxPenetrationVertexBelowPlane;
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
    }
    
    return true;
    
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
