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
#include "Player.hpp"
#include "Enemy.hpp"
#include "../sbndengine/physics/cPhysicsCollisionData.hpp"
#include "src/sbndengine/physics/cPhysicsIntersections.hpp"
#include <iostream>
#include <algorithm>

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
#include <vector>
#include <algorithm>

class GameApplication : public
		iApplication
{
	CGame *cGame;
	size_t level = 0;
	size_t points = 0;
	size_t time_end;
	size_t time_success;
	bool collisionSinceLastJump = true;

	// gravitation activated / deactivated
	bool gravitation_active;

	// use absolute mouse positions for motion
	bool mouse_absolute_motion;

	// output information about gui keystrokes
	bool output_gui_key_stroke_information;

	// main engine
	iEngine engine;

	std::vector<int> held_keys;

	enum Engine_state {GAME_SETUP, GAME_RUNNING, GAME_OVER, GAME_SUCCESS};

	Engine_state state;

	// storage for the character of the game
	struct
	{
		iRef<iObject> object;
		iRef<iPhysicsObject> physics_object;
		iRef<iGraphicsObject> graphics_object;
	} character;

	Player *player;
	cCamera3rdPerson player_camera;

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
		//iRef<iTexture> playerTexture = new iTexture;
		//playerTexture->createTextureFromFile("src/textures/wall.jpg");
		materials.playerMaterial = new iGraphicsMaterial;
		//materials.playerMaterial->setTexture(playerTexture);
		//playerTexture->createTextureFromFile("src/texture/wall_18Normal.jpg");
		//materials.playerMaterial->setNormalTexture(playerTexture);
		materials.playerMaterial->setColor(iColorRGBA(0.5, 0.5, 0.5));
	}

	/**
	 * create character object
	 */
	void setupCharacter()
	{
		iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(0.375, 0.75, 0.375);
		
		character.object = new iObject("character");
		character.object->createFromFactory(*box_factory);
		character.object->translate(-5, 0, 9);
		character.graphics_object = new iGraphicsObject(character.object, materials.playerMaterial);
		engine.graphics.addObject(character.graphics_object);
		engine.addObject(*character.object);
		character.physics_object = new iPhysicsObject(*character.object);
		character.physics_object->rotational_inertia = CMatrix3<float>(CMath<float>::max());;
		character.physics_object->rotational_inverse_inertia.setZero();
		engine.physics.addObject(character.physics_object);

		player = new Player(character.physics_object, player_camera, engine);
	}

	/*
	 * this method is called by the engine if a frame should be drawn
	 */
	void drawFrame()
	{
		handleHeldKeys(); // Handles player movement

		switch(state)
		{
			case GAME_SETUP:
			
				//reset engine
				engine.clear();
				cGame->collectables.clear();
				cGame->enemies.clear();
				cGame->untouchables.clear();
				held_keys.clear();
				points = 0;
				
				setupCharacter();
				player_camera.setup(player->getPosition(), CVector<3, float> (0, 2, 3.5));
				
				//load level
				loadLevel(level);
				
				engine.updateObjectModelMatrices();
				state = GAME_RUNNING;
				break;
				
			case GAME_RUNNING:
				checkGameEnd();

				player_camera.update(player->getPosition());
				player_camera.rotate(player->getAngularVelocity() * engine.time.frame_elapsed_seconds);
				player_camera.frustum(-1.5f, 1.5f, -1.5f * engine.window.aspect_ratio, 1.5f * engine.window.aspect_ratio, 1, 100);
				player_camera.computeMatrices();

				if (cGame->trigger1.isNotNull() && cGame->trigger1->inv_mass == 0 && points == 1) {
					cGame->trigger1->inv_mass = 1;
				}

				for (auto enemy : cGame->enemies) {
					enemy->move();
				}

				engine.window.setTitle("THIS GAME IS SO MUCH FUN!!1");

				/*
				 * PHYSICS: do one simulation step
				 */
				engine.physics.simulationTimestep(engine.time.elapsed_seconds);

				/*
				 * GRAPHICS: draw the objects
				 */
				engine.graphics.drawFrame(player_camera);

				/*
				 * output some help information
				 */
				if (output_gui_key_stroke_information)
				{
					engine.text.printfxy((float)10, (float)24, "%i Points", points);
					engine.text.printfxy((float)10, (float)34, "%f seconds left", getTimeLeft());
				}
				break;

			case GAME_OVER:
				player_camera.update(player->getPosition());
				player_camera.rotate(player->getAngularVelocity() * engine.time.frame_elapsed_seconds);
				player_camera.computeMatrices();
				engine.graphics.drawFrame(player_camera);

				engine.text.printfxy((float)10, (float)24, "GAME OVER");
				break;

			case GAME_SUCCESS:
				player_camera.update(player->getPosition());
				player_camera.rotate(0.25 * engine.time.frame_elapsed_seconds);
				player_camera.computeMatrices();
				engine.graphics.drawFrame(player_camera);

				engine.text.printfxy((float)10, (float)24, "YOU DID IT!");
				engine.text.printfxy((float)10, (float)34, "Time Left: %d", (time_end - time_success));
				engine.text.printfxy((float)10, (float)54, "Press enter to start the next level.");
		}
	}

	/**
	 * engine callback: setup the application
	 */
	void setup()
	{
		engine.physics.registerPreCollisionCallback(std::bind(&GameApplication::handleCollisions, this, std::placeholders::_1));
		cGame = new CGame(engine);
		
		setupMaterials();

		state = GAME_SETUP;
	}

	void loadLevel(int id)
	{
		switch (id) {
			case 0:
				cGame->level0();
				
				player->setJumpHeigth(7);
				character.object->position.setZero();
				
				setTimer(20);
				break;
				
			case 1:
				cGame->level1();
				
				player->setJumpHeigth(7);
				character.object->position.setZero();
				
				setTimer(20);
				break;

			case 2:
				cGame->level2();
				
				player->setJumpHeigth(7);
				character.object->position = CVector<3, float> (1, 0, 5);
				
				setTimer(20);
				break;
			
			case 3:
				cGame->level3();
				
				character.object->position.setZero();
				player->setJumpHeigth(5);
				
				setTimer(30);
				break;
		}
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
		held_keys.push_back(key);

		switch(key)
		{
			// General keys
			case 'q':	case 'Q':	engine.exit();	break;
			case 'R':
				level = 0;
			case 'r':
				state = GAME_SETUP;
				break;

			case ' ':
				if (collisionSinceLastJump) {
					player->jump();
					collisionSinceLastJump = false;
				}
				break;
				
			case 13:
				if (state = GAME_SUCCESS) {
					level = (level + 1) % 4;
					state = GAME_SETUP;
				}
		}
	}

	/*
	 * this method is called by the engine if a key was released
	 */
	void keyReleased(int key)
	{
		std::vector<int>::iterator element = std::find(held_keys.begin(), held_keys.end(), key);
		if (element != held_keys.end()) {
			held_keys.erase(element);
		}

		switch (key) {
			case 'a': case 'A':
			case SBND_EVENT_KEY_LEFT:
			case 'd': case 'D':
			case SBND_EVENT_KEY_RIGHT:
			case 'w': case 'W':
			case SBND_EVENT_KEY_UP:
			case 's': case 'S':
			case SBND_EVENT_KEY_DOWN:
				character.physics_object->velocity.setZero();
				character.physics_object->angular_velocity.setZero();
				break;
		}
	}

	/*
	 * this method is called every frame
	 */
	void handleHeldKeys()
	{
		for (std::vector<int>::iterator key = held_keys.begin(); key != held_keys.end(); key++) {
			switch(state)
			{
				case GAME_SETUP: break;
				case GAME_RUNNING:
					switch(*key)
					{
						case SBND_EVENT_KEY_UP:
						case 'w': case 'W':
							player->translate(CVector<3, float> (0, 0, -3) * player_camera.view_matrix * engine.time.frame_elapsed_seconds);
							break;
						case SBND_EVENT_KEY_DOWN:
						case 's': case 'S':
							player->translate(CVector<3, float> (0, 0, 3) * player_camera.view_matrix * engine.time.frame_elapsed_seconds);
							break;
						case SBND_EVENT_KEY_LEFT:
						case 'a': case 'A':
							player->rotate(CVector<3, float> (0, -2, 0) * engine.time.frame_elapsed_seconds);
							player_camera.rotate(2 * engine.time.frame_elapsed_seconds);
							break;
						case SBND_EVENT_KEY_RIGHT:
						case 'd': case 'D':
							player->rotate(CVector<3, float> (0, 2, 0) * engine.time.frame_elapsed_seconds);
							player_camera.rotate(-2 * engine.time.frame_elapsed_seconds);
							break;
					}
					break;

				case GAME_OVER:
				case GAME_SUCCESS:
					switch(*key)
					{
						case SBND_EVENT_KEY_LEFT:
						case 'a': case 'A':
							player->rotate(CVector<3, float> (0, -2, 0) * engine.time.frame_elapsed_seconds);
							player_camera.rotate(2 * engine.time.frame_elapsed_seconds);
							break;
						case SBND_EVENT_KEY_RIGHT:
						case 'd': case 'D':
							player->rotate(CVector<3, float> (0, 2, 0) * engine.time.frame_elapsed_seconds);
							player_camera.rotate(-2 * engine.time.frame_elapsed_seconds);
							break;
					}
			}
		}
	}

	/*
	 * this method is called each frame
	 */
	void handleCollisions(std::list<CPhysicsCollisionData> &collisions)
	{
		for (std::list<CPhysicsCollisionData>::iterator it = collisions.begin(); it != collisions.end(); it++) {
			bool deleteCollision;
			if (it->physics_object1->object == character.object)
				deleteCollision = handlePlayerTouch(it->physics_object2, *it);
			else if (it->physics_object2->object == character.object)
				deleteCollision = handlePlayerTouch(it->physics_object1, *it);

			if (deleteCollision) {
				collisions.erase(it);
				it--;
			}
		}
	}

	bool handlePlayerTouch(const iRef<iPhysicsObject> &physics_object, CPhysicsCollisionData &collision)
	{
		std::vector<iRef<iObject>>::iterator it;

		if (std::find(cGame->untouchables.begin(), cGame->untouchables.end(), physics_object->object) != cGame->untouchables.end()) {
			state = GAME_OVER;
			return true;
		}
		else if ((it = std::find(cGame->collectables.begin(), cGame->collectables.end(), physics_object->object)) != cGame->collectables.end()) {
			engine.physics.removeObject(physics_object);
			engine.graphics.removeObject(physics_object->object);
			engine.removeObject(*physics_object->object);
			++points;
			return true;
		}
		else {
			for (auto enemy : cGame->enemies) {
				if (*enemy != physics_object) continue;

				if (enemy->defeated(collision)) {
					engine.physics.removeObject(physics_object);
					engine.graphics.removeObject(physics_object->object);
					engine.removeObject(*physics_object->object);
					++points;
					return true;
				}
				else {
					state = GAME_OVER;
					return true;
				}
			}
		}

		if (fabs(collision.collision_normal[1]) == 1) {
			collisionSinceLastJump = true;
		}

		return false;
	}

	void setTimer(float seconds)
	{
		time_end = engine.time.elapsed_seconds + seconds;
	}

	float getTimeLeft() const
	{
		return (time_end - engine.time.elapsed_seconds);
	}

	void checkGameEnd()
	{
		if (getTimeLeft() < 0) state = GAME_OVER;
		else if (cGame->collectables.size() + cGame->enemies.size() == points) {
			time_success = engine.time.elapsed_seconds;
			state = GAME_SUCCESS;
		}
	}
};
