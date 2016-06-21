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

#include "sbndengine/engine/iEngine.hpp"
#include "sbndengine/graphics/iDraw3D.hpp"
#include <sstream>

class cPrivateEngine
{
public:
	// old mouse cursor absolute position in window coordinates
	int mouse_old_x, mouse_old_y;

	// accumulated mouse movement since last frame
	int mouse_rel_x, mouse_rel_y;

	cPrivateEngine()	:	mouse_old_x(-1), mouse_old_y(-1),
							mouse_rel_x(0), mouse_rel_y(0)
	{
	}
};

iEngine::iEngine()	:
		exit_program(false),
		reset_program(false),
		relativeMouseMovements(false),

		maximum_fps_for_physics_and_graphics(60.f)
{

	privateEngine = new cPrivateEngine;
}

void iEngine::run(class iApplication *p_application)
{
	application = p_application;

	// call this function to allow the application to run some
	// initialization code only once
	application->init();

	while (!exit_program)
	{
		window.init();
		window.create(640, 480, "SBND Engine Demo");

		graphics.setup();

		// setup the timer
		time.setup();

		// setup the event handler, e. g. to recognize the window close function
		events.setup(*this);

		// setup application (world, camera, event callbacks, etc.)
		application->setup();

		/*
		 * update all object model matrices before we start the rendering
		 *
		 * this is necessary due to the update of the physics which needs e. g.
		 * correct model view matrices
		 */
		updateObjectModelMatrices();

		// start main event loop
		events.startEventLoop();


		// shutdown application
		application->shutdown();

		window.destroy();
		window.shutdown();
	}
}

void iEngine::clear()
{
	graphics.clear();
	physics.reset();
	objectList.clear();
}

void iEngine::addObject(iObject &object)
{
	objectList.push_back(iRef<iObject>(object));
}

void iEngine::removeObject(iObject &object)
{
	objectList.remove(object);
}

void iEngine::updateObjectModelMatrices()
{
	for (std::list<iRef<iObject> >::iterator i = objectList.begin(); i != objectList.end(); i++)
		(**i).updateModelMatrix();
}


void iEngine::exit()
{
	events.leaveEventLoop();

	exit_program = true;
	// also set reset program to exit the "!reset_program" loop in run()
	reset_program = true;
}

void iEngine::reset()
{
	events.leaveEventLoop();

	reset_program = true;
}

iEngine::~iEngine()
{
	delete privateEngine;
}

/**
 * this method is executed previously to drawing a frame
 */
void iEngine::drawFramePrepare()
{
	/**
	 * update timeer
	 */
	time.update();

	/**
	 * limit elapsed seconds to 1/60
	 */
	double max_seconds_per_frame = 1.0/maximum_fps_for_physics_and_graphics;
	if (time.frame_elapsed_seconds > max_seconds_per_frame)
		time.frame_elapsed_seconds = max_seconds_per_frame;

	/**
	 * prepare rendering
	 */
	graphics.clearBuffers();
}


/**
 * this method is executed after the frame content was emitted
 */
void iEngine::drawFrameFinish()
{
	/**
	 * finish rendering
	 */
	window.swapBuffers();

	// if relativeMouseMovements is activated, move the curser to the windows center
	if (relativeMouseMovements)
	{
		window.setMousePosition(window.center_x, window.center_y);

		// set input state relative mouse motion to 0 for the case that no event is created
		inputState.relative_mouse_x = 0;
		inputState.relative_mouse_y = 0;

		// move mouse position to window center
		privateEngine->mouse_old_x = window.center_x;
		privateEngine->mouse_old_y = window.center_y;

		// set relative mouse motion accumulator to 0
		privateEngine->mouse_rel_x = 0;
		privateEngine->mouse_rel_y = 0;
	}


	/**
	 * limit to 60 FPS
	 */
	double max_seconds_per_frame = 1.0/maximum_fps_for_physics_and_graphics;
	static double lastTime = -1.0;
	double diff = max_seconds_per_frame - (time.elapsed_seconds - lastTime);

	if (diff > 0.0)
	{
		time.nanoSleep(diff);

		if (diff < max_seconds_per_frame)
		{
			lastTime += max_seconds_per_frame;
		}
		else
		{
			time.update();
			lastTime = time.elapsed_seconds;
		}
	}
	else
	{
		time.update();
		lastTime = time.elapsed_seconds;
	}
}


/**
 * called when debug mode is activated:
 *
 * output debug information about object laying under the mouse cursor
 */
void iEngine::physicsDebugInformation(iCamera &camera)
{
	if (physics.debug.active)
	{
		CVector<3,float> world_ray_start_pos, world_ray_direction;
		getRayAtCursorPosition(world_ray_start_pos, world_ray_direction, camera);

		iRef<iObjectRayIntersectionData> intersection = getObjectRayIntersection(world_ray_start_pos, world_ray_direction);

		if (intersection.isNotNull())
		{
			iPhysicsObject &po = *(iPhysicsObject*)intersection->collidingObject->physics_engine_ptr;
			iObject &o = *intersection->collidingObject.ref_class;

			float x = inputState.mouse_x+10.0f/(float)window.height;
			float y = inputState.mouse_y+50.0f/(float)window.height;
			std::ostringstream ss;
			ss << "Object: " << o.identifier_string << std::endl;
			ss << "Position: " << o.position << std::endl;
			ss << "Velocity: " << po.velocity << std::endl;
			ss << "Rotation: " << o.rotation << std::endl;
			ss << "Angular velocity: " << po.angular_velocity << std::endl;
			ss << "--------------------------------------" << std::endl;
			ss << "Friction disabled: " << po.friction_disabled << std::endl;
			ss << "Friction dynamic coefficient: " << po.friction_dynamic_coefficient << std::endl;
			ss << "Friction static coefficient: " << po.friction_static_coefficient << std::endl;
			ss << "Movable: " << po.movable << std::endl;
			text.printfScreenxy(x, y, ss.str().c_str());
		}
	}
}


/**
 * implement basic event handlers and forward to application class
 */
void iEngine::windowResized(int width, int height)
{
	graphics.windowResized(width, height);
	window.windowResized(width, height);
}

void iEngine::drawFrame()
{
	drawFramePrepare();

	application->drawFrame();

	drawFrameFinish();
}

void iEngine::keyPressed(int key)
{
	application->keyPressed(key);
}


void iEngine::keyReleased(int key)
{
	application->keyReleased(key);
}


void iEngine::windowClose()
{
	exit();
}

void iEngine::mouseMove(int x, int y)
{
	int half_x = window.center_x;
	int half_y = window.center_y;

	if (relativeMouseMovements)
	{
		if (x != half_x || y != half_y)
		{
			privateEngine->mouse_rel_x += x - privateEngine->mouse_old_x;
			privateEngine->mouse_rel_y += y - privateEngine->mouse_old_y;

			inputState.relative_mouse_x = (float)privateEngine->mouse_rel_x / (float)window.width;
			inputState.relative_mouse_y = -(float)privateEngine->mouse_rel_y / (float)window.height;

			// always put to center
			inputState.mouse_x = 0;
			inputState.mouse_y = 0;
		}
	}
	else
	{
		inputState.mouse_x = (float)x / (float)window.width * 2.0f - 1.0f;
		inputState.mouse_y = 1.0f - (float)y / (float)window.height * 2.0f;
	}

	application->mouseMove(x, y);

	privateEngine->mouse_old_x = x;
	privateEngine->mouse_old_y = y;
}

void iEngine::mouseButtonDown(int button_nr)
{
	if (button_nr < 5)
		inputState.mouse_buttons[button_nr] = true;

	application->mouseButtonDown(button_nr);
}


void iEngine::mouseButtonUp(int button_nr)
{
	if (button_nr < 5)
		inputState.mouse_buttons[button_nr] = false;

	application->mouseButtonUp(button_nr);
}


iRef<iObjectRayIntersectionData> iEngine::getObjectRayIntersection(
			const CVector<3,float> &ray_start_pos,
			const CVector<3,float> &ray_direction
		)
{
	iObjectRayIntersection objectRayIntersection;
	objectRayIntersection.setup(ray_start_pos, ray_direction);

	for (std::list<iRef<iObject> >::iterator i = objectList.begin(); i != objectList.end(); i++)
	{
		iObject &o = **i;
		if (o.intersections_computable)
			objectRayIntersection.computeRayIntersection(o);
	}

	if (!objectRayIntersection.intersectionData.isNotNull())
		return iRef<iObjectRayIntersectionData>();

	return iRef<iObjectRayIntersectionData>(objectRayIntersection.intersectionData.getClass());
}


void iEngine::getRayAtCursorPosition(
		CVector<3,float> &world_ray_start_pos,
		CVector<3,float> &world_ray_direction,
		iCamera &camera
	)
{
	/*
	 * first we compute the ray start and direction in world space
	 */
	static CVector<3,float> drag_start_object_space_coordinate;

	// use camera projection and view matrix to project back the mouse coordinate to world space
	CVector<4,float> view_space_coord = camera.projection_matrix.getInverse()*CVector<4,float>(inputState.mouse_x, inputState.mouse_y, -1, 1);
	view_space_coord /= view_space_coord[3];

	world_ray_start_pos = CVector<3,float>(camera.getPosition());
	/*
	 * vectors (such as the viewspace coordinate since this is a direction from the origin) are
	 * projected to another basis with matrix M by using the inverse tranpose of M.
	 *
	 * since we have to project from viewspace back to the world space, we use the inverse matrix M as
	 * the projection matrix. then for vectors we use the inverse transpose inverse, which is simply the
	 * transpose matrix.
	 *
	 * if a is a vector, we can use the formula $$ a*M^T = M*a $$ to reduce our computation to a simple vector-matrix multiplication
	 */
	world_ray_direction = CVector<3,float>(view_space_coord) * CMatrix3<float>(camera.view_matrix);
	world_ray_direction.normalize();
}


void iEngine::activateRelativeMouseMovements()
{
	window.hideMouse();
	window.setMousePosition(window.center_x, window.center_y);
	inputState.mouse_x = 0;
	inputState.mouse_y = 0;
	relativeMouseMovements = true;
}


void iEngine::activateAbsoluteMouseMovements()
{
	window.showMouse();
	relativeMouseMovements = false;
	inputState.relative_mouse_x = 0;
	inputState.relative_mouse_y = 0;
}
