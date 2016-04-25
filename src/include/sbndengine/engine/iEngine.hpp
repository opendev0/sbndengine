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

#ifndef __I_ENGINE_HPP__
#define __I_ENGINE_HPP__

#include "sbndengine/iApplication.hpp"
#include "sbndengine/engine/iObject.hpp"
#include "sbndengine/engine/iObjectRayIntersection.hpp"
#include "sbndengine/iRef.hpp"
#include "sbndengine/iEvents.hpp"
#include "sbndengine/iEventHandlers.hpp"
#include "sbndengine/iInputState.hpp"
#include "sbndengine/camera/iCamera.hpp"
#include "sbndengine/iTime.hpp"
#include "sbndengine/iWindow.hpp"
#include "sbndengine/physics/iPhysics.hpp"
#include "sbndengine/graphics/iGraphics.hpp"
#include "sbndengine/iText.hpp"
#include <list>

/**
 * this is the root of the whole SBND engine.
 *
 * the engine is intended to initialize everything:
 *   * graphics
 *   * physics
 *   * keyboard/mouse input
 */
class iEngine	: public iEventHandlers
{
private:
	class cPrivateEngine *privateEngine;

	// interface to user application
	class iApplication *application;

	// list with objects to draw and for physics
	std::list<iRef<iObject> > objectList;

	bool exit_program;
	bool reset_program;

	bool relativeMouseMovements;

	float maximum_fps_for_physics_and_graphics;

public:
	iWindow window;		///< handler to the rendering window
	iTime time;			///< handler to the time data
	iEvents events;		///< handler to the event system
	iInputState inputState;	///< handler to information about the current input state

	iPhysics physics;	///< handler to the physics engine
	iGraphics graphics;	///< handler to draw something

	iText text;	///< text handler to draw some 2D string on screen

	iEngine();
	virtual ~iEngine();

	/**
	 * clear all datasets (e. g. object list, etc.)
	 */
	void clear();

	/**
	 * run the engine with p_application having the callback functions
	 */
	void run(class iApplication *p_application);

	/**
	 * add an object to be rendered / simulated by the engine
	 */
	void addObject(iObject &object);

	/**
	 * update all object model matrix
	 */
	void updateObjectModelMatrices();

	/**
	 * draw a frame with objects viewed through camera
	 */
	void drawFrame(iCamera &p_camera);

	/**
	 * call this function to exit the program
	 */
	void exit();

	/**
	 * call this function to reset the program
	 */
	void reset();

	/**
	 * prepare drawing a frame. E. g. call the functions to clear the color and depth buffer, etc.
	 */
	void drawFramePrepare();


	/**
	 * the work drawing a frame. E. g. swapping front and back buffer
	 */
	void drawFrameFinish();

	/**
	 * compute intersection point of ray with all objects
	 */
	iRef<iObjectRayIntersectionData> getObjectRayIntersection(
				const CVector<3,float> &ray_start_pos,
				const CVector<3,float> &ray_direction
			);

//	iObject *getObjectByIdentifier(const char* p_identifier_string);

	/**
	 * call this function if the relative mouse movements should be enabled.
	 * in this mode, the mouse cursor is hidden.
	 *
	 * if the relative mouse movements is activated, a camera control like
	 * in a 1st person can be implemented.
	 */
	void activateRelativeMouseMovements();

	/**
	 * call this function to activate absolute mouse movements. since the mouse
	 * cursor is visible in this mode, the cursor can be used to select objects.
	 */
	void activateAbsoluteMouseMovements();

	/**
	 * get the ray start position and ray direction which represents a line
	 * through the current camera position and the mouse cursor.
	 */
	void getRayAtCursorPosition(
			CVector<3,float> &world_ray_start_pos,
			CVector<3,float> &world_ray_direction,
			iCamera &camera
		);

	/**
	 * output some physics information about the object at the current cursors position
	 */
	void physicsDebugInformation(iCamera &camera);


private:
	/**
	 * IMPLEMENTED EVENT HANDLERS
	 */
	void windowResized(int width, int height);
	void drawFrame();
	void windowClose();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mouseMoveRelative(int x, int y);
	void mouseButtonDown(int button_nr);
	void mouseButtonUp(int button_nr);
};

#endif //__I_OBJECT_HPP__
