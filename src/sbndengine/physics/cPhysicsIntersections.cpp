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
    Vector maxBelowPlane, maxAbovePlane;
    
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
            if (current[1] < maxBelowPlane[1]) {
                maxBelowPlane = current;
            }
        }
        //vertex is above plane
        else {
            sideOfPlane++;
            //vertex is the farthest above plane
            if (current[1] > maxAbovePlane[1]) {
                maxAbovePlane = current;
            }
        }
    }

    //no collision when all points are on one side of plane or outside of plane
    if (abs(sideOfPlane) == 8 || vertecesOutsidePlane.size() == 8) {
        return false;
    }


    c.physics_object1 = &physics_object_plane;
    c.physics_object2 = &physics_object_box;
   
 
    //check for edge/edge collisions
    if (!vertecesOutsidePlane.empty()) {
        
        //for each vertex outside we check all edges connecting it to its neighbours
        Vector calculateNeighbors[3] = {Vector(-1, 1, 1), Vector(1, -1, 1), Vector(1, 1, -1)};
        
        Vector largest = Vector();
        Vector largestNeighbour = Vector();
        
        for (std::list<Vector>::iterator it = vertecesOutsidePlane.begin(); it != vertecesOutsidePlane.end(); ++it) {
            
            Vector current = plane->inverse_model_matrix * box->model_matrix * *it;
            
            if (sideOfPlane < 0) {
                //the current vertex is above the plane
                if (current[1] >= maxAbovePlane[1] && current[1] >= largest[1]) {

                    largest = current;
                    
                    for (Vector* factor = calculateNeighbors; factor != calculateNeighbors + 3; factor++) {
                        
                        Vector neighbour = plane->inverse_model_matrix * box->model_matrix * (current * *factor);
                        //the current neighbour is below the plane and inside the plane
                        if (!(fabs(neighbour[0]) > planeFactory.size_x / 2 || fabs(neighbour[2]) > planeFactory.size_z / 2) && neighbour[1] < 0 && neighbour[1] < largestNeighbour[1]) {
                            largestNeighbour = neighbour;
                        }
                    }
                }
            }            
            
            else { 
                //the current vertex is below the plane
                if (current[1] < maxBelowPlane[1] && current[1] < largest[1]) {
                    
                    largest = current;
                    
                    for (Vector* factor = calculateNeighbors; factor != calculateNeighbors + 3; factor++) {
                        
                        Vector neighbour = plane->inverse_model_matrix * box->model_matrix * (current * *factor);
                        //the current neighbour is above the plane and inside the plane
                        if (!(fabs(neighbour[0]) > planeFactory.size_x / 2 || fabs(neighbour[2]) > planeFactory.size_z / 2) && neighbour[1] >= 0 && neighbour[1] >= largestNeighbour[1]) {
                            largestNeighbour = neighbour;
                        }
                    }
                }
            }
        }
            

    
        //calculate collision with edges parallel to z axis
        LinePP boxEdge = LinePP(largest, largestNeighbour);
        LinePP planeEdge = LinePP(Vector(planeFactory.size_x/2, 0, -planeFactory.size_z/2), Vector(planeFactory.size_x/2, 0, planeFactory.size_z/2));
        Vector boxClosestVertex, planeClosestVertex;
        
        IntLinePPLinePP::closestPoints(boxEdge, planeEdge, boxClosestVertex, planeClosestVertex);
        
        
        //check if collision occured
        if (IntLinePPLinePP::distance(boxEdge, planeEdge) <= 0.1) {
            c.collision_normal = plane->inverse_model_matrix.getTranspose() * (boxClosestVertex - planeClosestVertex).getNormalized();
            c.collision_point1 = plane->model_matrix * planeClosestVertex;
            c.collision_point2 = plane->model_matrix * boxClosestVertex;
            c.interpenetration_depth = (c.collision_point1 - c.collision_point2).getLength();
            return true;
        }
        
 
 
        //calculate collisions parallel to x axis
        planeEdge = LinePP(Vector(-planeFactory.size_x/2, 0, planeFactory.size_z/2), Vector(planeFactory.size_x/2, 0, planeFactory.size_z/2));
        
        IntLinePPLinePP::closestPoints(boxEdge, planeEdge, boxClosestVertex, planeClosestVertex);
        
        
        //check if collision occured
        if (IntLinePPLinePP::distance(boxEdge, planeEdge) <= 0.1) {
            c.collision_normal = plane->inverse_model_matrix.getTranspose() * (planeClosestVertex - boxClosestVertex).getNormalized();
            c.collision_point1 = plane->model_matrix * planeClosestVertex;
            c.collision_point2 = plane->model_matrix * boxClosestVertex;
            c.interpenetration_depth = (c.collision_point1 - c.collision_point2).getLength();
            return true;
        }
        
        return false;
    }
    
    
    
    if (sideOfPlane < 0) {
        c.collision_normal = plane->inverse_model_matrix.getTranspose() * Vector(0, -1, 0);
        c.collision_point1 = plane->model_matrix * Vector(maxAbovePlane[0], 0, maxAbovePlane[2]);
        c.collision_point2 = plane->model_matrix * maxAbovePlane;
        c.interpenetration_depth = (c.collision_point2 - c.collision_point1).getLength();
    }
    else {
        c.collision_normal = plane->inverse_model_matrix.getTranspose() * Vector(0, 1, 0);
        c.collision_point1 = plane->model_matrix * Vector(maxBelowPlane[0], 0, maxBelowPlane[2]);
        c.collision_point2 = plane->model_matrix * maxBelowPlane;
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
 * compute the intersection between a box and a box
 */
vec2d CPhysicsIntersections::getProjection(iPhysicsObject const &boxObject, Vector const &axis)
{
	iRef<iObject> box = boxObject.object;
	Vector boxHalfSize = static_cast<cObjectFactoryBox *>(&box->objectFactory.getClass())->half_size;
	
	Vector vertexList[8] = {
		Vector(-boxHalfSize[0], -boxHalfSize[1], -boxHalfSize[2]),
		Vector(-boxHalfSize[0], -boxHalfSize[1], boxHalfSize[2]),
		Vector(-boxHalfSize[0], boxHalfSize[1], -boxHalfSize[2]),
		Vector(-boxHalfSize[0], boxHalfSize[1], boxHalfSize[2]),
		Vector(boxHalfSize[0], -boxHalfSize[1], -boxHalfSize[2]),
		Vector(boxHalfSize[0], -boxHalfSize[1], boxHalfSize[2]),
		Vector(boxHalfSize[0], boxHalfSize[1], -boxHalfSize[2]),
		Vector(boxHalfSize[0], boxHalfSize[1], boxHalfSize[2])
	};
	
	float min = axis.dotProd(boxObject.object->model_matrix * vertexList[0]);
	float max = min;
	for (int i = 1; i < 8; ++i) {
		// Vertices are in model space. We need to transform them to world space.
		Vector vertexPos = boxObject.object->model_matrix * vertexList[i];
		float projLength = axis.dotProd(vertexPos);
		if (projLength > max) max = projLength;
		else if (projLength < min) min = projLength;
	}

	return vec2d(min, max);
}

bool CPhysicsIntersections::boxBox(iPhysicsObject &physics_object_box1, iPhysicsObject &physics_object_box2, CPhysicsCollisionData &c)
{
#if WORKSHEET_5
	// Determine axis that need to be checked for overlapping of object projections (separating axis theorem)
	CMatrix4<float> modelMatrix1 = physics_object_box1.object->model_matrix;
	CMatrix4<float> modelMatrix2 = physics_object_box2.object->model_matrix;

	Vector seperatingAxes[15] = {
		// Normals of object1's surfaces
		Vector(modelMatrix1[0][0], modelMatrix1[1][0], modelMatrix1[2][0]), 
		Vector(modelMatrix1[0][1], modelMatrix1[1][1], modelMatrix1[2][1]), 
		Vector(-modelMatrix1[0][2], -modelMatrix1[1][2], -modelMatrix1[2][2]),
		// Normals of object2's surfaces
		Vector(modelMatrix2[0][0], modelMatrix2[1][0], modelMatrix2[2][0]),
		Vector(modelMatrix2[0][1], modelMatrix2[1][1], modelMatrix2[2][1]),
		Vector(-modelMatrix2[0][2], -modelMatrix2[1][2], -modelMatrix2[2][2])
	};

	int x = 6;
	for (int i = 0; i < 3; i++) {
		for (int j = 3; j < 6; j++) {
			seperatingAxes[x] = seperatingAxes[i] % seperatingAxes[j];
			x++;
		}
	}

	// Separating axis algorithm
	c.interpenetration_depth = CMath<float>::max();
	for (Vector* axis = seperatingAxes; axis != seperatingAxes + 15; axis++) {
		*axis = (*axis).getNormalized();

		vec2d proj1 = getProjection(physics_object_box1, *axis);
		vec2d proj2 = getProjection(physics_object_box2, *axis);
		
		if (proj1[1] <= proj2[0] || proj2[1] <= proj1[0]) {
			return false; // Separating axis found -> no collision
		}

		// Current axis is not separating objects -> calculate overlap
		float overlap = CMath<float>::min(proj1[1], proj2[1]) - CMath<float>::max(proj1[0], proj2[0]);
		if (overlap < c.interpenetration_depth) {
			c.interpenetration_depth = overlap;
			c.collision_normal = *axis;
			Vector sgn = (physics_object_box2.object->position - physics_object_box1.object->position);
			if (sgn.dotProd(c.collision_normal) < 0) c.collision_normal = -c.collision_normal;
            
            if (axis - seperatingAxes < 3) {
                //used principal axis of box1
                c.collision_point1 = physics_object_box1.object->position + c.collision_normal * fabs(proj1[1] - proj1[0]) * 0.5f;
                c.collision_point2 = c.collision_point1 + c.collision_normal * c.interpenetration_depth;
            }
            else if (axis - seperatingAxes < 6) {
                //used principal axis of box2
                c.collision_point2 = physics_object_box2.object->position + c.collision_normal * fabs(proj2[1] - proj2[0]) * 0.5f;
                c.collision_point1 = c.collision_point2 + c.collision_normal * c.interpenetration_depth;
            }
            else {
                //used axis created by cross-product
                
                sgn[0] = (sgn[0] >= 0) - (sgn[0] < 0);
                sgn[1] = (sgn[1] >= 0) - (sgn[1] < 0);
                sgn[2] = (sgn[2] >= 0) - (sgn[2] < 0);
                


                Vector boxHalfSize1 = static_cast<cObjectFactoryBox *>(&physics_object_box1.object->objectFactory.getClass())->half_size;
                
                //calculate which principle axis of box1 was used for creation of the cross-product, this is the direction of the edge
                int i = (axis - seperatingAxes - 6)/3;
                
                //calculate a point on this edge using the other two principle axis
                Vector point1 = physics_object_box1.object->position 
                                    + seperatingAxes[(i+1)%3] * boxHalfSize1[(i+1)%3] * sgn[(i+1)%3]
                                    + seperatingAxes[(i+2)%3] * boxHalfSize1[(i+2)%3] * sgn[(i+2)%3];
                LinePP edge1 = LinePP(point1, point1 + seperatingAxes[i]);
                
                
                Vector boxHalfSize2 = static_cast<cObjectFactoryBox *>(&physics_object_box2.object->objectFactory.getClass())->half_size;
                
                //calculate which principle axis of box2 was used for creation of the cross-product, this is the direction of the edge
                int j = (axis - seperatingAxes)%3;
                //calculate a point on this edge using the other two principle axis
                Vector point2 = physics_object_box2.object->position 
                                    + seperatingAxes[(j+1)%3 + 3] * boxHalfSize2[(j+1)%3] * -sgn[(j+1)%3]
                                    + seperatingAxes[(j+2)%3 + 3] * boxHalfSize2[(j+2)%3] * -sgn[(j+2)%3];

                LinePP edge2 = LinePP(point2, point2 + seperatingAxes[j + 3]);
                
                //find the two closest points on the lines
                IntLinePPLinePP::closestPoints(edge1, edge2, c.collision_point1, c.collision_point2);
            }
		}
	}

    
	std::cout << "Box-Box-Collision detected" << std::endl;
	std::cout << "Object1: " << physics_object_box1.object->identifier_string << std::endl;
	std::cout << "Object2: " << physics_object_box2.object->identifier_string << std::endl;
	std::cout << "Collision normal: " << c.collision_normal << std::endl;
	std::cout << "Interpenetration depth: " << c.interpenetration_depth << std::endl;
	std::cout << "Collision point distance: " << (c.collision_point2 - c.collision_point1).getLength() << std::endl << std::endl;
    
	c.physics_object1 = &physics_object_box1;
	c.physics_object2 = &physics_object_box2;
    
	return true;
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
