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

#include "sbndengine/iSbndEngine.hpp"
#include "cScenes.hpp"
#include "cGame.hpp"
#include <iostream>

/*
#if defined(WIN32) || defined(_WIN32)
	#include <stdlib.h>
	#include <io.h>
#else
	#include <unistd.h>
#endif
*/

#include <stdio.h>
#include <sstream>



class keyReleased;
int *global_pargc;
char **global_argv;

class cApplicationImplementation : public
		iApplication
{
	// application in game mode
	bool game_mode;

	// scenes
	int scene_id;
	CScenes *cScenes;
	CGame *cGame;

	// gravitation activated / deactivated
	bool gravitation_active;

	// use absolute mouse positions for motion
	bool mouse_absolute_motion;

	// output information about gui keystrokes
	bool output_gui_key_stroke_information;

	// main engine
	iEngine engine;

	// camera to see something
	cCamera1stPerson camera;

	// game camera
	cCamera3rdPerson game_camera;

	// storage for the character of the game
	struct
	{
		iRef<iObject> object;
		iRef<iPhysicsObject> physics_object;
		iRef<iGraphicsObject> graphics_object;
	} character;

	/**
	 * different kinds of materials (so far only colors are supported)
	 */
	struct
	{
		iRef<iGraphicsMaterial> whiteMaterial;
		iRef<iGraphicsMaterial> blackMaterial;
		iRef<iGraphicsMaterial> playerMaterial;
	} materials;

	/**
	 * storage for engine objects used for control
	 */
	struct
	{
		iRef<iObject> sphere_mouse_start;
		iRef<iObject> sphere_mouse_end;

		iRef<iObject> sphere_mouse_start_spring;
		iRef<iObject> sphere_mouse_end_spring;
	} engineObjects;

	/**
	 *storage for graphics objects
	 */
	struct
	{
		iRef<iGraphicsObject> sphere_mouse_start;
		iRef<iGraphicsObject> sphere_mouse_end;
		iRef<iGraphicsObjectConnector> sphere_connector;

		iRef<iGraphicsObject> sphere_mouse_start_spring;
		iRef<iGraphicsObject> sphere_mouse_end_spring;
		iRef<iGraphicsObjectConnector> sphere_connector_spring;
	} graphicsObjects;

	/**
	 * storage for physics objects used for physical interactions
	 */
	struct
	{
		iRef<iPhysicsObject> sphere_mouse_start;
		iRef<iPhysicsObject> sphere_mouse_end;

		iRef<iPhysicsObject> sphere_mouse_start_spring;
		iRef<iPhysicsObject> sphere_mouse_end_spring;
	} physicObjects;

	/**
	 * vector containing the movement velocity of the player relative to the current view
	 */
	CVector<3,float> playerVelocity;


public:
	cApplicationImplementation()	:
		game_mode(false),
		scene_id(1),
		gravitation_active(true),
		mouse_absolute_motion(false),
		output_gui_key_stroke_information(true)

	{
		engine.run(this);
	}

	/**
	 * reset the players state
	 */
	void resetPlayer()
	{
		camera.reset();
		camera.moveRelative(CVector<3,float>(0, 4, 12));
		camera.computeMatrices();
	}

	/**
	 * setup materials
	 */
	void setupMaterials()
	{
		/*
		 * setup white & black materials for drag&drop functionality
		 */
		materials.whiteMaterial = new iGraphicsMaterial;
		materials.whiteMaterial->setColor(iColorRGBA(1.0, 1.0, 1.0));

		materials.blackMaterial = new iGraphicsMaterial;
		materials.blackMaterial->setColor(iColorRGBA(0.0, 0.0, 0.0));


		/*
		 * setup player material
		 */
		materials.playerMaterial = new iGraphicsMaterial;
		materials.playerMaterial->setColor(iColorRGBA(0.0, 0.0, 0.0));
	}


	/**
	 * setup the scene with the mouse specific objects (mouse ball, object attached ball, etc.)
	 */
	void setupSceneMouse()
	{
		iRef<cObjectFactorySphere> sphere_small_factory = new cObjectFactorySphere(0.2f);

		/**
		 * standard mouse balls
		 */
		engineObjects.sphere_mouse_start = new iObject("SphereMouseStart");
		engineObjects.sphere_mouse_start->setIntersectionsComputable(false);
		engineObjects.sphere_mouse_start->createFromFactory(*sphere_small_factory);
		engine.addObject(*engineObjects.sphere_mouse_start);

		graphicsObjects.sphere_mouse_start = new iGraphicsObject(engineObjects.sphere_mouse_start, *materials.whiteMaterial);
		engine.graphics.addObject(graphicsObjects.sphere_mouse_start);
		graphicsObjects.sphere_mouse_start->unsetVisible();

		// don't add to physics engine
		physicObjects.sphere_mouse_start = new iPhysicsObject(engineObjects.sphere_mouse_start);


		engineObjects.sphere_mouse_end = new iObject("SphereMouseEnd");
		engineObjects.sphere_mouse_end->setIntersectionsComputable(false);
		engineObjects.sphere_mouse_end->createFromFactory(*sphere_small_factory);
		engine.addObject(*engineObjects.sphere_mouse_end);

		graphicsObjects.sphere_mouse_end = new iGraphicsObject(engineObjects.sphere_mouse_end, *materials.blackMaterial);
		engine.graphics.addObject(graphicsObjects.sphere_mouse_end);
		graphicsObjects.sphere_mouse_end->unsetVisible();

		physicObjects.sphere_mouse_end = new iPhysicsObject(engineObjects.sphere_mouse_end);
		physicObjects.sphere_mouse_end->setInverseMass(0);

		graphicsObjects.sphere_connector = new cGraphicsObjectConnectorCenter(engineObjects.sphere_mouse_start, engineObjects.sphere_mouse_end, materials.whiteMaterial, false);
		engine.graphics.addObjectConnector(graphicsObjects.sphere_connector);

		/**
		 * mouse balls for spring
		 */
		engineObjects.sphere_mouse_start_spring = new iObject("SphereMouseStart");
		engineObjects.sphere_mouse_start_spring->setIntersectionsComputable(false);
		engineObjects.sphere_mouse_start_spring->createFromFactory(*sphere_small_factory);
		engine.addObject(*engineObjects.sphere_mouse_start_spring);

		graphicsObjects.sphere_mouse_start_spring = new iGraphicsObject(engineObjects.sphere_mouse_start_spring, *materials.whiteMaterial);
		engine.graphics.addObject(graphicsObjects.sphere_mouse_start_spring);
		graphicsObjects.sphere_mouse_start_spring->unsetVisible();

		// don't add to physics engine
		physicObjects.sphere_mouse_start_spring = new iPhysicsObject(engineObjects.sphere_mouse_start_spring);


		engineObjects.sphere_mouse_end_spring = new iObject("SphereMouseEnd");
		engineObjects.sphere_mouse_end_spring->setIntersectionsComputable(false);
		engineObjects.sphere_mouse_end_spring->createFromFactory(*sphere_small_factory);
		engine.addObject(*engineObjects.sphere_mouse_end_spring);

		graphicsObjects.sphere_mouse_end_spring = new iGraphicsObject(engineObjects.sphere_mouse_end_spring, *materials.blackMaterial);
		engine.graphics.addObject(graphicsObjects.sphere_mouse_end_spring);
		graphicsObjects.sphere_mouse_end_spring->unsetVisible();

		physicObjects.sphere_mouse_end_spring = new iPhysicsObject(engineObjects.sphere_mouse_end_spring);
		physicObjects.sphere_mouse_end_spring->setInverseMass(0);

		graphicsObjects.sphere_connector_spring = new cGraphicsObjectConnectorCenter(engineObjects.sphere_mouse_start_spring, engineObjects.sphere_mouse_end_spring, materials.whiteMaterial, false);
		engine.graphics.addObjectConnector(graphicsObjects.sphere_connector_spring);
	}


	/**
	 * enable gravitation in physics engine
	 */
	void enableGravitation()
	{
		engine.physics.setGravitation(CVector<3,float>(0, -9.81f, 0));
	}

	/**
	 * disable gravitation in physics engine
	 */
	void disableGravitation()
	{
		engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
	}


	/**
	 * this method is called if a new scene has to be created
	 */
	void setupWorld()
	{
		// clear existing data in case that this is a reset
		engine.clear();

		// set the appropriate mouse mode
		if (mouse_absolute_motion)	engine.activateAbsoluteMouseMovements();
		else						engine.activateRelativeMouseMovements();

		// setup gravitation
		if (gravitation_active)	enableGravitation();
		else					disableGravitation();

		// setup materials
		setupMaterials();

		// setup scene
		cScenes->setupScene(scene_id);

		// setup mouse balls, physic spring objects for mouse interactions, etc...
		setupSceneMouse();

		// update all object model matrices
		engine.updateObjectModelMatrices();
	}


	/*
	 * sets up gravity, the character and the game scene
	 */
	void setupGameWorld()
	{
		// clear existing data
		engine.clear();

		// enable gravity
		enableGravitation();

		setupMaterials();

		// setup the game scene
		cGame->setupGameScene();
		setupCharacter();

		engine.updateObjectModelMatrices();
	}

	void setupCharacter()
	{
		iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(1, 1, 1);

		character.object = new iObject("character");
		character.object->createFromFactory(*box_factory);
		character.graphics_object = new iGraphicsObject(character.object, materials.playerMaterial);
		engine.graphics.addObject(character.graphics_object);
		engine.addObject(*character.object);
		character.physics_object = new iPhysicsObject(*character.object);
		engine.physics.addObject(character.physics_object);
	}


	/**
	 * this method cares about the mouse interactivity
	 */
	void interactiveMouseActions(std::ostringstream &title_text)
	{
		static bool allow_impulse = true;

		// configuration variable
		bool frame_based_impulse = true;

		if (engine.inputState.mouse_buttons[2])	// right mouse button
		{
			if (allow_impulse || frame_based_impulse)
			{
				CVector<3,float> world_ray_start_pos, world_ray_direction;
				engine.getRayAtCursorPosition(world_ray_start_pos, world_ray_direction, camera);

				iRef<iObjectRayIntersectionData> intersection = engine.getObjectRayIntersection(world_ray_start_pos, world_ray_direction);

				if (intersection.isNotNull())
				{
					iPhysicsObject &po = *(iPhysicsObject*)intersection->collidingObject->physics_engine_ptr;
					if (po.isMovable())
					{
						engine.physics.addImpulseToObjectAtPoint(
								po,
								world_ray_start_pos + world_ray_direction*intersection->t,
								world_ray_direction*(frame_based_impulse ? (float)engine.time.frame_elapsed_seconds*5.0f : 1.0f) / po.inv_mass
								);
					}
				}

				allow_impulse = false;
			}
		}
		else
		{
			allow_impulse = true;
		}

		/**
		 * MIDDLE MOUSE BUTTON
		 */
		{
			static iRef<iObject> selected_object;
			static float selected_eye_point_distance = -1.0;	// distance between eye and object
			static iRef<iPhysicsSoftConstraint> spring;

			if (engine.inputState.mouse_buttons[1])	// middle mouse button
			{
				CVector<3,float> world_ray_start_pos, world_ray_direction;
				engine.getRayAtCursorPosition(world_ray_start_pos, world_ray_direction, camera);

				static CVector<3,float> drag_start_object_space_coordinate;

				if (selected_object.isNotNull())
				{
					CVector<3,float> old_cursor_world_pos = selected_object->model_matrix*drag_start_object_space_coordinate;
					engineObjects.sphere_mouse_start_spring->setPosition(old_cursor_world_pos);
					engineObjects.sphere_mouse_start_spring->updateModelMatrix();
					graphicsObjects.sphere_mouse_start_spring->setVisible();

					// move the destination object position
					CVector<3,float> new_cursor_world_pos = world_ray_start_pos + world_ray_direction*selected_eye_point_distance;
					engineObjects.sphere_mouse_end_spring->setPosition(new_cursor_world_pos);
					engineObjects.sphere_mouse_end_spring->updateModelMatrix();
					graphicsObjects.sphere_mouse_end_spring->setVisible();

					graphicsObjects.sphere_connector_spring->setVisibility(true);

					title_text << "   Selected Object '" << selected_object->identifier_string << "' with a spring force";
				}
				else
				{
					iRef<iObjectRayIntersectionData> intersection = engine.getObjectRayIntersection(world_ray_start_pos, world_ray_direction);

					if (intersection.isNotNull())
					{
						/*
						 * select the object
						 */
						selected_object = intersection->collidingObject.ref_class;

						selected_eye_point_distance = intersection->t;

						CVector<3,float> intersection_point = world_ray_start_pos + world_ray_direction * intersection->t;

						drag_start_object_space_coordinate = selected_object->model_matrix.getInverse()*intersection_point;

						engineObjects.sphere_mouse_start_spring->setPosition(selected_object->model_matrix*drag_start_object_space_coordinate);
						engineObjects.sphere_mouse_start_spring->updateModelMatrix();
						graphicsObjects.sphere_mouse_start_spring->setVisible();

						// set the mass of the object mouse end point to the same as the object
						physicObjects.sphere_mouse_end_spring->setInverseMass(((iPhysicsObject*)selected_object->physics_engine_ptr)->inv_mass);

						engineObjects.sphere_mouse_end_spring->setPosition(intersection_point);
						engineObjects.sphere_mouse_end_spring->updateModelMatrix();
						graphicsObjects.sphere_mouse_end_spring->setVisible();

						graphicsObjects.sphere_connector_spring->setVisibility(true);

						title_text << "   Selected Object: " << intersection->collidingObject->identifier_string;

						/*
						 * create spring
						 */
						spring = new cPhysicsSoftConstraintSpringAngular(
								*(iPhysicsObject*)selected_object->physics_engine_ptr,
								drag_start_object_space_coordinate,
								*(physicObjects.sphere_mouse_end_spring),
								CVector<3,float>(0,0,0),
								(intersection_point - selected_object->position).getLength(),
								50.0f/(*(iPhysicsObject*)selected_object->physics_engine_ptr).inv_mass,
								0.9f,
								true
							);
						engine.physics.addSoftConstraint(spring);

					}
				}
			}
			else
			{
				if (selected_object.isNotNull())
				{
					graphicsObjects.sphere_mouse_start_spring->unsetVisible();
					graphicsObjects.sphere_mouse_end_spring->unsetVisible();
					graphicsObjects.sphere_connector_spring->setVisibility(false);
					selected_object.release();

					engine.physics.removeSoftConstraint(spring);
					spring.release();
				}
			}
		}


		/**
		 * LEFT MOUSE BUTTON
		 */
		{
			static iRef<iObject> selected_object;
			static float right_selected_eye_point_distance = -1.0;	// distance between eye and object

			if (engine.inputState.mouse_buttons[0])	// left mouse button
			{
				/*
				 * first we compute the ray start and direction in world space
				 */
				CVector<3,float> world_ray_start_pos, world_ray_direction;
				engine.getRayAtCursorPosition(world_ray_start_pos, world_ray_direction, camera);

				static CVector<3,float> drag_start_object_space_coordinate;

				if (selected_object.isNotNull())
				{
					/*
					 * an object is already selected -> move the object to the current cursor position
					 */

					CVector<3,float> old_cursor_world_pos = selected_object->model_matrix*drag_start_object_space_coordinate;
					engineObjects.sphere_mouse_start->setPosition(old_cursor_world_pos);
					engineObjects.sphere_mouse_start->updateModelMatrix();
					graphicsObjects.sphere_mouse_start->setVisible();

					// move the destination object position
					CVector<3,float> new_cursor_world_pos = world_ray_start_pos + world_ray_direction*right_selected_eye_point_distance;
					engineObjects.sphere_mouse_end->setPosition(new_cursor_world_pos);
					engineObjects.sphere_mouse_end->updateModelMatrix();
					graphicsObjects.sphere_mouse_end->setVisible();
					graphicsObjects.sphere_connector->setVisibility(true);

					iPhysicsObject *po = (iPhysicsObject*)selected_object->physics_engine_ptr;
					CVector<3,float> object_move_speed = (new_cursor_world_pos - old_cursor_world_pos)*5.0;

					if (po->isMovable())
					{
						po->setSpeed(object_move_speed);
					}
					else
					{
						selected_object->translate(object_move_speed*(float)engine.time.frame_elapsed_seconds);
						selected_object->updateModelMatrix();
					}

					title_text << "   Selected Object: " << selected_object->identifier_string;
				}
				else
				{
					iRef<iObjectRayIntersectionData> intersection = engine.getObjectRayIntersection(world_ray_start_pos, world_ray_direction);

					if (intersection.isNotNull())
					{
						/*
						 * select the object
						 */
						selected_object = intersection->collidingObject.ref_class;

						right_selected_eye_point_distance = intersection->t;

						CVector<3,float> intersection_point = world_ray_start_pos + world_ray_direction * intersection->t;

						drag_start_object_space_coordinate = selected_object->model_matrix.getInverse()*intersection_point;

						engineObjects.sphere_mouse_start->setPosition(selected_object->model_matrix*drag_start_object_space_coordinate);
						engineObjects.sphere_mouse_start->updateModelMatrix();
						graphicsObjects.sphere_mouse_start->setVisible();

						// set the mass of the object mouse cube to the same as the object
						physicObjects.sphere_mouse_start->setInverseMass(((iPhysicsObject*)selected_object->physics_engine_ptr)->inv_mass);

						engineObjects.sphere_mouse_end->setPosition(intersection_point);
						engineObjects.sphere_mouse_end->updateModelMatrix();
						graphicsObjects.sphere_mouse_end->setVisible();
						graphicsObjects.sphere_connector->setVisibility(true);

						title_text << "   Selected Object: " << intersection->collidingObject->identifier_string;
					}
				}

			}
			else
			{
				if (selected_object.isNotNull())
				{
					if (mouse_absolute_motion)
						graphicsObjects.sphere_mouse_start->unsetVisible();
					graphicsObjects.sphere_mouse_end->unsetVisible();
					graphicsObjects.sphere_connector->setVisibility(false);
					selected_object.release();
				}

				if (!mouse_absolute_motion)
				{
					CVector<3,float> world_ray_start_pos, world_ray_direction;
					engine.getRayAtCursorPosition(world_ray_start_pos, world_ray_direction, camera);

					iRef<iObjectRayIntersectionData> intersection = engine.getObjectRayIntersection(world_ray_start_pos, world_ray_direction);
					if (intersection.isNotNull())
					{
						CVector<3,float> intersection_point = world_ray_start_pos + world_ray_direction * intersection->t;
						graphicsObjects.sphere_mouse_start->object->setPosition(intersection_point);
						graphicsObjects.sphere_mouse_start->object->updateModelMatrix();
						graphicsObjects.sphere_mouse_start->setVisible();
					}
					else
					{
						CVector<3,float> intersection_point = world_ray_start_pos + world_ray_direction * 20;

						engineObjects.sphere_mouse_start->setPosition(intersection_point);
						engineObjects.sphere_mouse_start->updateModelMatrix();
						graphicsObjects.sphere_mouse_start->setVisible();
					}
				}
			}
		}
	}

	/*
	 * this method is called by the engine if a frame should be drawn
	 */
	void drawFrame()
	{
		// ostringstream to set the window title
		std::ostringstream title_text;
		if (!game_mode) {
			/*
			 * CAMERA MOVEMENTS
			 */
			camera.moveRelative(playerVelocity*(float)engine.time.frame_elapsed_seconds*15.0f);
			camera.rotate(engine.inputState.relative_mouse_y, -engine.inputState.relative_mouse_x, 0);
			camera.frustum(-1.5f,1.5f,-1.5f*engine.window.aspect_ratio,1.5f*engine.window.aspect_ratio,1,100);
			camera.computeMatrices();

			/*
			 * mouse actions
			 */
			interactiveMouseActions(title_text);

			/*
			 * setup window title
			 */
			title_text << "   Scene: '" << cScenes->scene_description << "'";

			title_text << "   FPS: '" << engine.time.fps << "'";
			engine.window.setTitle(title_text.str().c_str());
		}
		else {
			/*
			 * CAMERA MOVEMENTS
			 */
            character.object->translate(playerVelocity * game_camera.view_matrix * engine.time.frame_elapsed_seconds);
			game_camera.update(character.object->position);
			game_camera.rotate(character.physics_object->angular_velocity[1] * engine.time.frame_elapsed_seconds);
			game_camera.frustum(-1.5f,1.5f,-1.5f*engine.window.aspect_ratio,1.5f*engine.window.aspect_ratio,1,100);
			game_camera.computeMatrices();


			engine.window.setTitle("THIS GAME IS SO MUCH FUN!!1");
		}

		/*
		 * PHYSICS: do one simulation step
		 */
		engine.physics.simulationTimestep(engine.time.elapsed_seconds);

		/*
		 * GRAPHICS: draw the objects
		 */
		if (!game_mode) {
			engine.graphics.drawFrame(camera);
		}
		else {
			engine.graphics.drawFrame(game_camera);
		}

		/*
		 * output some help information
		 */
		if (output_gui_key_stroke_information)
		{
			if (!game_mode) {
				int pos_y = 10+14;
				engine.text.printfxy((float)10, (float)pos_y, "Press [h] to hide this information");	pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[q]: quit");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[r]: reset application");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[e]: reset scene");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[1,2,...,0]: switch to scenes 1 to 10");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[F1-F12]: switch to scenes 11 to 24");	pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[a,s,d,w]: move left, back, right, forward");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[space]: game mode on/off");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[g]: enable gravitation");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[G]: disable gravitation");	pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[backspace]: activate/deactivate DEBUG mode, record each timestep");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "    + [enter]: pause/continue simulation");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "    + [cursor]: move your cursor over objects for debug information");	pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "    + [mouse wheel up/down]: forward/backward in simulation history during debug mode");	pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[tab]: switch to game mode"); pos_y += 14;
			}
			else {
				int pos_y = 10+14;
				engine.text.printfxy((float)10, (float)pos_y, "Press [h] to hide this information");	pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[q]: quit"); pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[<-/->]: rotate camera"); pos_y += 14;
				pos_y += 14;
				engine.text.printfxy((float)10, (float)pos_y, "[tab]: switch to physics mode"); pos_y += 14;

			}
		}

		/*
		 * this function has to be called to output some debug informations
		 * about the object state of the physic engine
		 */
		engine.physicsDebugInformation(camera);
	}




	/**
	 * engine callback: init is called only once during the whole program runtime
	 */
	void init()
	{
	}


	/**
	 * engine callback: setup the application
	 */
	void setup()
	{
		cScenes = new CScenes(engine);
		cGame = new CGame(engine);

		setupWorld();

		resetPlayer();
	}


	/**
	 * simply delete the allocated scenes
	 */
	void shutdown()
	{
		delete cScenes;
		delete cGame;
	}


	void setupGameMode()
	{
		game_mode = true;
		setupGameWorld();
		game_camera.setup(character.object->position, CVector<3, float> (0, 2, 3));
		game_camera.computeMatrices();
	}


	void shutdownGameMode()
	{
		game_mode = false;
		scene_id = 1;
		setupWorld();
		resetPlayer();
	}

	/*
	 * this method is executed by the engine if a key was pressed
	 */
	void keyPressed(int key)
	{
		if (!game_mode) {
			switch(key)
			{
				case '\t':	setupGameMode();	break;
				case 'q':	case 'Q':	engine.exit();	break;

				case 'w':	case 'W':	playerVelocity[2] = CMath<float>::max(playerVelocity[2]-1.0f, -1.0f);	break;
				case 's':	case 'S':	playerVelocity[2] = CMath<float>::min(playerVelocity[2]+1.0f, +1.0f);	break;
				case 'd':	case 'D':	playerVelocity[0] = CMath<float>::min(playerVelocity[0]+1.0f, +1.0f);	break;
				case 'a':	case 'A':	playerVelocity[0] = CMath<float>::max(playerVelocity[0]-1.0f, -1.0f);	break;

				case 'h':	output_gui_key_stroke_information = !output_gui_key_stroke_information;	break;

				case 'r':		shutdown(); setup();		break;
				case 'e':		setupWorld();	break;
				case ' ':
					mouse_absolute_motion = !mouse_absolute_motion;
					if (mouse_absolute_motion)	engine.activateAbsoluteMouseMovements();
					else						engine.activateRelativeMouseMovements();
					break;

				case 'g':		gravitation_active = true;	enableGravitation();	break;
				case 'G':		gravitation_active = false;	disableGravitation();	break;

				case '1':		scene_id = 1;	setupWorld();	break;
				case '2':		scene_id = 2;	setupWorld();	break;
				case '3':		scene_id = 3;	setupWorld();	break;
				case '4':		scene_id = 4;	setupWorld();	break;
				case '5':		scene_id = 5;	setupWorld();	break;
				case '6':		scene_id = 6;	setupWorld();	break;
				case '7':		scene_id = 7;	setupWorld();	break;
				case '8':		scene_id = 8;	setupWorld();	break;
				case '9':		scene_id = 9;	setupWorld();	break;
				case '0':		scene_id = 10;	setupWorld();	break;

				case SBND_EVENT_KEY_F1:		scene_id = 11;	setupWorld();	break;
				case SBND_EVENT_KEY_F2:		scene_id = 12;	setupWorld();	break;
				case SBND_EVENT_KEY_F3:		scene_id = 13;	setupWorld();	break;
				case SBND_EVENT_KEY_F4:		scene_id = 14;	setupWorld();	break;
				case SBND_EVENT_KEY_F5:		scene_id = 15;	setupWorld();	break;
				case SBND_EVENT_KEY_F6:		scene_id = 16;	setupWorld();	break;
				case SBND_EVENT_KEY_F7:		scene_id = 17;	setupWorld();	break;
				case SBND_EVENT_KEY_F8:		scene_id = 18;	setupWorld();	break;
				case SBND_EVENT_KEY_F9:		scene_id = 19;	setupWorld();	break;
				case SBND_EVENT_KEY_F10:	scene_id = 20;	setupWorld();	break;
				case SBND_EVENT_KEY_F11:	scene_id = 21;	setupWorld();	break;
				case SBND_EVENT_KEY_F12:	scene_id = 22;	setupWorld();	break;

				case 'y':       scene_id = 23;    setupWorld();    break;
				case 'x':       scene_id = 24;    setupWorld();    break;
				case 'c':       scene_id = 25;    setupWorld();    break;
				case 'v':       scene_id = 26;    setupWorld();    break;
				case 'b':       scene_id = 27;    setupWorld();    break;
				case 'n':       scene_id = 28;    setupWorld();    break;
				case 'm':       scene_id = 29;    setupWorld();    break;

				/*
				 * physics engine debug stuff
				 */
				case SBND_EVENT_KEY_BACKSPACE:	if (engine.physics.debug.active)	engine.physics.debug.stop();	else	engine.physics.debug.start();	break;
				case SBND_EVENT_KEY_ENTER:		if (engine.physics.debug.paused)	engine.physics.debug.play();	else	engine.physics.debug.pause();	break;
				case '+':	engine.physics.debug.forwardFrame();	break;
				case '-':	engine.physics.debug.backwardFrame();	break;
			}
		}
		else {
			switch(key)
			{
				case '\t':	shutdownGameMode();	break;
				case 'q':	case 'Q':	engine.exit();	break;
				case SBND_EVENT_KEY_UP:
                case 'w': case 'W':
                    playerVelocity[2] = -3;
                    break;
				case SBND_EVENT_KEY_DOWN:
                case 's': case 'S':
                    playerVelocity[2] = 3;
                    break;
				case SBND_EVENT_KEY_LEFT:
                case 'a': case 'A':
                    character.physics_object->angular_velocity[1] = 1.5;
                    break;
				case SBND_EVENT_KEY_RIGHT:
                case 'd': case 'D':
                    character.physics_object->angular_velocity[1] = -1.5;
					break;
				case ' ':
					character.physics_object->velocity[1] = 10;
					break;
			}
		}
	}

	/*
	 * this method is called by the engine if a key was released
	 */
	void keyReleased(int key)
	{
		if (!game_mode) {
			switch(key)
			{
				case 'w':	case 'W':	playerVelocity[2] = CMath<float>::min(playerVelocity[2]+1.0f, +1.0f);	break;
				case 's':	case 'S':	playerVelocity[2] = CMath<float>::max(playerVelocity[2]-1.0f, -1.0f);	break;
				case 'd':	case 'D':	playerVelocity[0] = CMath<float>::max(playerVelocity[0]-1.0f, -1.0f);	break;
				case 'a':	case 'A':	playerVelocity[0] = CMath<float>::min(playerVelocity[0]+1.0f, +1.0f);	break;
			}
		}
		else {
			switch(key)
			{
                case SBND_EVENT_KEY_UP:
                case 'w': case 'W':
				case SBND_EVENT_KEY_DOWN:
                case 's': case 'S':
                    playerVelocity[2] = 0;
                    break;
				case SBND_EVENT_KEY_LEFT:
                case 'a': case 'A':
				case SBND_EVENT_KEY_RIGHT:
                case 'd': case 'D':
					character.physics_object->angular_velocity[1] = 0;
					break;
			}
		}
	}

	/*
	 * this method is called by the engine if mouse button 'button' was pressed
	 */
	void mouseButtonDown(int button)
	{
		switch(button)
		{
			case 3:	engine.physics.debug.forwardFrame();	break;
			case 4:	engine.physics.debug.backwardFrame();	break;
		}
	}
};

int main(int argc, char **argv)
{
	global_pargc = &argc;
	global_argv = argv;

	{
		cApplicationImplementation *cA = new cApplicationImplementation;

#ifdef DEBUG
		if (!debug_ibase_list.empty())
			std::cout << "MEMORY LEAK: iBase class missing in action" << std::endl;
#endif
		delete cA;
	}
	return 0;
}
