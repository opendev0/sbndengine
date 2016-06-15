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
#include "../cGame.hpp"
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

class GameApplication : public
		iApplication
{
	CGame *cGame;

	// gravitation activated / deactivated
	bool gravitation_active;

	// use absolute mouse positions for motion
	bool mouse_absolute_motion;

	// output information about gui keystrokes
	bool output_gui_key_stroke_information;

	// main engine
	iEngine engine;

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
	 * vector containing the movement velocity of the player relative to the current view
	 */
	CVector<3,float> playerVelocity;


public:
	GameApplication()	:
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
		game_camera.setup(character.object->position, CVector<3, float> (0, 2, 3));
		game_camera.computeMatrices();
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

	/*
	 * Sets up gravity, the character and the game scene
	 */
	void setupWorld()
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
	}

	/*
	 * this method is called by the engine if a frame should be drawn
	 */
	void drawFrame()
	{
		/*
		 * CAMERA MOVEMENTS
		 */
		character.object->translate(playerVelocity * game_camera.view_matrix * engine.time.frame_elapsed_seconds);
		game_camera.update(character.object->position);
		game_camera.rotate(character.physics_object->angular_velocity[1] * engine.time.frame_elapsed_seconds);
		game_camera.frustum(-1.5f,1.5f,-1.5f*engine.window.aspect_ratio,1.5f*engine.window.aspect_ratio,1,100);
		game_camera.computeMatrices();

		engine.window.setTitle("THIS GAME IS SO MUCH FUN!!1");

		/*
		 * PHYSICS: do one simulation step
		 */
		engine.physics.simulationTimestep(engine.time.elapsed_seconds);

		/*
		 * GRAPHICS: draw the objects
		 */
		engine.graphics.drawFrame(game_camera);

		/*
		 * output some help information
		 */
		if (output_gui_key_stroke_information)
		{
			int pos_y = 10+14;
			engine.text.printfxy((float)10, (float)pos_y, "Press [h] to hide this information");	pos_y += 14;
			pos_y += 14;
			engine.text.printfxy((float)10, (float)pos_y, "[q]: quit"); pos_y += 14;
			engine.text.printfxy((float)10, (float)pos_y, "[<-/->]: rotate camera"); pos_y += 14;
		}
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
		cGame = new CGame(engine);

		setupWorld();
		resetPlayer();
	}


	/**
	 * simply delete the allocated scenes
	 */
	void shutdown()
	{
		delete cGame;
	}

	/*
	 * this method is executed by the engine if a key was pressed
	 */
	void keyPressed(int key)
	{
		switch(key)
		{
			// General keys
			case 'q':	case 'Q':	engine.exit();	break;
			case 'h':	output_gui_key_stroke_information = !output_gui_key_stroke_information;	break;

			// Movement control keys
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
				character.physics_object->angular_velocity[1] = 2;
				break;
			case SBND_EVENT_KEY_RIGHT:
			case 'd': case 'D':
				character.physics_object->angular_velocity[1] = -2;
				break;
			case ' ':
				character.physics_object->velocity[1] = 10;
				break;
		}
	}

	/*
	 * this method is called by the engine if a key was released
	 */
	void keyReleased(int key)
	{
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

	/*
	 * this method is called by the engine if mouse button 'button' was pressed
	 */
	void mouseButtonDown(int button)
	{
	}
};