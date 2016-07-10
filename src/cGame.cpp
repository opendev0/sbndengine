#include "cGame.hpp"

#define TEXTURE_PATH "src/textures/"

/**
 * helper macros to speed-up creation of world scenes
 *
 * see code below how to use these macros
 */

#define NEW_PLANE(plane_id, color, px, py, pz, factory)							\
		iRef<iObject> plane_id = new iObject(#plane_id);				\
		plane_id->createFromFactory(*factory);					\
		plane_id->translate(px, py, pz);														\
		iRef<iGraphicsObject> plane_id##_graphics_object = new iGraphicsObject(plane_id, materials.color);	\
		engine.graphics.addObject(plane_id##_graphics_object);			\
		engine.addObject(*plane_id);									\
		iRef<iPhysicsObject> plane_id##_physics_object = new iPhysicsObject(*plane_id);			\
		plane_id##_physics_object->setInverseMass(0);							\
		engine.physics.addObject(plane_id##_physics_object);


#define NEW_SPHERE(sphere_id, color, px, py, pz, factory)						\
		iRef<iObject> sphere_id = new iObject(""#sphere_id);			\
		sphere_id->createFromFactory(*factory);					\
		sphere_id->translate(px, py, pz);								\
		iRef<iGraphicsObject> sphere_id##_graphics_object = new iGraphicsObject(sphere_id, materials.color);	\
		engine.graphics.addObject(sphere_id##_graphics_object);			\
		engine.addObject(*sphere_id);									\
		iRef<iPhysicsObject> sphere_id##_physics_object = new iPhysicsObject(*sphere_id);	\
		engine.physics.addObject(sphere_id##_physics_object);


#define NEW_BOX(box_id, color, px, py, pz, factory)								\
		iRef<iObject> box_id = new iObject(""#box_id);					\
		box_id->createFromFactory(*factory);						\
		box_id->translate(px, py, pz);									\
		iRef<iGraphicsObject> box_id##_graphics_object = new iGraphicsObject(box_id, materials.color);	\
		engine.graphics.addObject(box_id##_graphics_object);			\
		engine.addObject(*box_id);										\
		iRef<iPhysicsObject> box_id##_physics_object = new iPhysicsObject(*box_id);			\
		engine.physics.addObject(box_id##_physics_object);

#define NEW_ROPE(o1, o2)						\
		iRef<iPhysicsHardConstraint> o1##_##o2##_hard_constraint_rope = new cPhysicsHardConstraintRope(o1##_physics_object, o2##_physics_object, o1->position.dist(o2->position));	\
		o1##_physics_object->setDisableCollisionRotationAndFrictionFlag(true);	o2##_physics_object->setDisableCollisionRotationAndFrictionFlag(true);	\
		engine.physics.addHardConstraint(o1##_##o2##_hard_constraint_rope);		\
		engine.graphics.addObjectConnector(new cGraphicsObjectConnectorCenter(o1, o2, materials.white));

#define NEW_ROPE_ANGULAR(o1, a1, o2, a2, extra_distance)						\
		iRef<iPhysicsHardConstraint> o1##_##o2##_hard_constraint_rope = new cPhysicsHardConstraintRopeAngular(o1##_physics_object, (a1), o2##_physics_object, (a2), ((o1)->position+(a1)).dist((o2)->position+(a2))+extra_distance);	\
		engine.physics.addHardConstraint(o1##_##o2##_hard_constraint_rope);		\
		engine.graphics.addObjectConnector(new cGraphicsObjectConnectorAngular(o1, a1, o2, a2, materials.white));

#define NEW_SPRING(o1, o2)						\
		iRef<iPhysicsSoftConstraint> o1##_##o2##_soft_constraint_rope = new cPhysicsSoftConstraintSpring(o1##_physics_object, o2##_physics_object, o1->position.dist(o2->position), 50);	\
		o1##_physics_object->setDisableCollisionRotationAndFrictionFlag(true);	o2##_physics_object->setDisableCollisionRotationAndFrictionFlag(true);	\
		engine.physics.addSoftConstraint(o1##_##o2##_soft_constraint_rope);		\
		engine.graphics.addObjectConnector(new cGraphicsObjectConnectorCenter(o1, o2, materials.white));


#define NEW_SPRING_ANGULAR(o1, a1, o2, a2, extra_distance)                  \
		iRef<iPhysicsSoftConstraint> o1##_##o2##_soft_constraint_spring = new cPhysicsSoftConstraintSpringAngular(o1##_physics_object, (a1), o2##_physics_object, (a2), ((o1)->position+(a1)).dist((o2)->position+(a2))+extra_distance);  \
		engine.physics.addSoftConstraint(o1##_##o2##_soft_constraint_spring); \
		engine.graphics.addObjectConnector(new cGraphicsObjectConnectorAngular(o1, a1, o2, a2, materials.white));

/**
 * setup materials
 */
void CGame::setupMaterials()
{
	/*
	 * setup materials
	 */

	iRef<iTexture> texture;

	// noise textures
	texture = new iTexture;
	texture->createRandomizedTexture(0.7f, 0.2f, 0.0f, 0.5f);
	materials.red_noise = new iGraphicsMaterial;
	materials.red_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.1f, 0.8f, 0.2f, 0.3f);
	materials.green_noise = new iGraphicsMaterial;
	materials.green_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.9f, 0.9f, 0.1f, 0.6f);
	materials.yellow_noise = new iGraphicsMaterial;
	materials.yellow_noise->setTexture(texture);


	texture = new iTexture;
	texture->createRandomizedTexture(0.2f, 0.3f, 0.9f, 0.3f);
	materials.blue_noise = new iGraphicsMaterial;
	materials.blue_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(1.0f, 1.0f, 1.0f, 0.3f);
	materials.white_noise = new iGraphicsMaterial;
	materials.white_noise->setTexture(texture);


	texture = new iTexture;
	texture->createRandomizedTexture(0.0f, 0.0f, 0.0f, 0.4f);

	materials.black_noise = new iGraphicsMaterial;
	materials.black_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.1f, 0.9f, 0.8f, 0.4f);

	materials.cyan_noise = new iGraphicsMaterial;
	materials.cyan_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.8f, 0.1f, 0.9f, 0.3f);

	materials.cyan_noise = new iGraphicsMaterial;
	materials.cyan_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.4f, 0.2f, 0.0f, 0.3f);
	materials.brown_noise = new iGraphicsMaterial;
	materials.brown_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.8f, 0.83f, 0.76f, 0.4f);
	materials.grey_noise = new iGraphicsMaterial;
	materials.grey_noise->setTexture(texture);

	texture = new iTexture;
	texture->createRandomizedTexture(0.8f, 0.1f, 0.9f, 0.3f);
	materials.pink_noise = new iGraphicsMaterial;
	materials.pink_noise->setTexture(texture);

	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "boden_12.jpg");
	materials.boden_1 = new iGraphicsMaterial;
	materials.boden_1->setTexture(texture);
	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "wand_18Normal.jpg");
	materials.boden_1->setNormalTexture(texture);

	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "wall.jpg");
	materials.wand_18 = new iGraphicsMaterial;
	materials.wand_18->setTexture(texture);
	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "wand_18Normal.jpg");
	materials.wand_18->setNormalTexture(texture);

	// colored surfaces only
#if 0
	materials.red = new iGraphicsMaterial;
	materials.red->setColor(iColorRGBA(1.0f, 0.0f, 0.0f));

	materials.green = new iGraphicsMaterial;
	materials.green->setColor(iColorRGBA(0.0f, 1.0f, 0.0f));

	materials.yellow = new iGraphicsMaterial;
	materials.yellow->setColor(iColorRGBA(1.0f, 1.0f, 0.0f));

	materials.blue = new iGraphicsMaterial;
	materials.blue->setColor(iColorRGBA(0.0f, 0.0f, 1.0f));

	materials.white = new iGraphicsMaterial;
	materials.white->setColor(iColorRGBA(1.0f, 1.0f, 1.0f));

	materials.black = new iGraphicsMaterial;
	materials.black->setColor(iColorRGBA(0.0f, 0.0f, 0.0f));

	materials.cyan = new iGraphicsMaterial;
	materials.cyan->setColor(iColorRGBA(0.0f, 1.0f, 1.0f));

	materials.pink = new iGraphicsMaterial;
	materials.pink->setColor(iColorRGBA(1.0f, 0.0f, 1.0f));
#else
	materials.red = materials.red_noise;
	materials.green = materials.green_noise;
	materials.yellow = materials.yellow_noise;
	materials.blue = materials.blue_noise;
	materials.white = materials.white_noise;
	materials.black = materials.black_noise;
	materials.cyan = materials.cyan_noise;
	materials.pink = materials.pink_noise;
#endif
}

/**
 * setup planes (a box without the top plane)
 */
void CGame::setupWorldBoxPlanes(float scale)
{
	/*
	 * setup objects
	 */
	iRef<iPhysicsObject> new_physics_object;

	iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(25.f*scale, 25.f*scale);
	plane_factory->setInverseMass(0);

	NEW_PLANE(plane0, boden_1, 0, -3*scale, 0, plane_factory);
	NEW_PLANE(plane1, wand_18, 0, 0, -10*scale, plane_factory);
	plane1->rotate(CVector<3,float>(1, 0, 0), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane2, wand_18, -10*scale, 0, 0, plane_factory);
	plane2->rotate(CVector<3,float>(0, 1, 0), -CMath<float>::PI()*0.5f);
	plane2->rotate(CVector<3,float>(0, 0, 1), CMath<float>::PI()*0.5f);
	NEW_PLANE(plane3, wand_18, 10*scale, 0, 0, plane_factory);
	plane3->rotate(CVector<3,float>(0, 1, 0), CMath<float>::PI()*0.5f);
	plane3->rotate(CVector<3,float>(0, 0, 1), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane4, wand_18, 0, 0, 10*scale, plane_factory);

	plane4->rotate(CVector<3,float>(0, 1, 0), -CMath<float>::PI());
	plane4->rotate(CVector<3,float>(1, 0, 0), CMath<float>::PI()*0.5f);
}

void CGame::level0() {
	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> coin_factory = new cObjectFactorySphere(0.2);
	
	//stairs
	float stairX = 0;
	float stairZ = -1.5;
	
	//-------------------------------------------------------------------------
	
	iRef<cObjectFactoryBox> stair_factory = new cObjectFactoryBox(0.75, 2, 0.75);
	stair_factory->setInverseMass(0);
	
	NEW_BOX(platform1, grey_noise, stairX, -2, stairZ, stair_factory);
	
	NEW_BOX(platform2_1, grey_noise, stairX, -2, stairZ - 2, stair_factory);	
	NEW_BOX(platform2_2, grey_noise, stairX, 0, stairZ - 2, stair_factory);
	
	NEW_BOX(platform3_1, grey_noise, stairX, -2, stairZ - 5, stair_factory);
	NEW_BOX(platform3_2, grey_noise, stairX, 0, stairZ - 5, stair_factory);	
	NEW_BOX(platform3_3, grey_noise, stairX, 2, stairZ - 5, stair_factory);
	
	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------
	
	NEW_SPHERE(coin1, green, stairX, -0.9, stairZ, coin_factory);
	collectables.push_back(coin1);
	
	NEW_SPHERE(coin2, green, stairX, 1.1, stairZ - 2, coin_factory);
	collectables.push_back(coin2);
	
	NEW_SPHERE(coin3, green, stairX, 3.1, stairZ - 5, coin_factory);
	collectables.push_back(coin3);
}

void CGame::level1() {
	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> coin_factory = new cObjectFactorySphere(0.1);
	
	//ramp
	float rampX = -10;
	float rampZ = -10;
	
	iRef<cObjectFactoryBox> ramp_factory = new cObjectFactoryBox(5, 5, 6);
	NEW_BOX(ramp, white_noise, rampX, -3, rampZ + 5, ramp_factory);
	ramp_physics_object->setInverseMass(0);
	ramp->rotate(CVector<3, float> (0, 0, 1), CMath<float>::PI()*0.25f);
	
	iRef<cObjectFactoryBox> ramp_side_factory = new cObjectFactoryBox(8.5, 7.5, 0.25);
	NEW_BOX(ramp_side1, white_noise, rampX, -3, rampZ + 8, ramp_side_factory);
	ramp_side1_physics_object->setInverseMass(0);
	
	NEW_BOX(ramp_side2, white_noise, rampX, -3, rampZ + 2, ramp_side_factory);
	ramp_side2_physics_object->setInverseMass(0);
	
	NEW_BOX(safety_step, white_noise, rampX + 4, -6, rampZ - 2.2, ramp_side_factory);
	safety_step->rotate(CVector<3, float> (0, 1, 0), CMath<float>::PI()*0.5f);
	safety_step_physics_object->setInverseMass(0);

	iRef<cObjectFactoryBox> switch_factory = new cObjectFactoryBox(0.5, 5, 8);
	
	NEW_BOX(coin4, green, rampX + 3, -2, rampZ + 4, switch_factory);
	coin4->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.01f);
	coin4_physics_object->setInverseMass(0);
	collectables.push_back(coin4);
	
	
	iRef<cObjectFactorySphere> ball_factory = new cObjectFactorySphere(0.2);
	iRef<cObjectFactoryBox> spike_factory = new cObjectFactoryBox(0.4, 0.4, 0.4);
	
	iRef<iObject> balls[10];
	iRef<iGraphicsObject> gballs[10];
	iRef<iPhysicsObject> pballs[10];

	for (int i = 5; i < 10; i++) {
		balls[i] = new iObject("ball" + i);
		balls[i]->createFromFactory(*ball_factory);
		balls[i]->translate(rampX + 1, 5 + i, rampZ + i%5 + 3);
		gballs[i] = new iGraphicsObject(balls[i], materials.red);
		engine.graphics.addObject(gballs[i]);
		engine.addObject(*balls[i]);
		pballs[i] = new iPhysicsObject(*balls[i]);
		engine.physics.addObject(pballs[i]);
		
		untouchables.push_back(balls[i]);
	}
	for (int i = 0; i < 5; i++) {
		balls[i] = new iObject("spike" + i);
		balls[i]->createFromFactory(*spike_factory);
		balls[i]->translate(rampX + 1, 5 + i, rampZ + i%5 + 3);
		gballs[i] = new iGraphicsObject(balls[i], materials.red);
		engine.graphics.addObject(gballs[i]);
		engine.addObject(*balls[i]);
		pballs[i] = new iPhysicsObject(*balls[i]);
		engine.physics.addObject(pballs[i]);
		
		untouchables.push_back(balls[i]);
	}
}

void CGame::level2() {
	
	setupWorldBoxPlanes(0.5);
	iRef<cObjectFactorySphere> coin_factory = new cObjectFactorySphere(0.3);

	
	iRef<cObjectFactoryBox> stair_factory = new cObjectFactoryBox(0.5, 2, 1);
	stair_factory->setInverseMass(0);
	
	iRef<cObjectFactoryBox> path_factory = new cObjectFactoryBox(0.5, 4, 2);
	path_factory->setInverseMass(0);
	
	
	NEW_BOX(front_step1, pink, -4.5, -2, -3.5, stair_factory);
	NEW_BOX(front_step2, pink, -4.5, -2, -4, stair_factory);
	NEW_BOX(front_step3, pink, -4.5, -2, -4.5, stair_factory);
	
	
	NEW_BOX(path1, pink, -4.5, -1, -2, path_factory);
	
	NEW_BOX(path2, pink, -3.75, -1, -1, path_factory);
	path2->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path3, pink, -1.75, -1, -1, path_factory);
	path3->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path4, pink, 0.25, -1, -1, path_factory);
	path4->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path5, pink, 2.25, -1, -1, path_factory);
	path5->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	
	NEW_BOX(path6, pink, 3, -1, 0, path_factory);
	NEW_BOX(path7, pink, 3, -1, 2, path_factory);
	
	
	NEW_BOX(end_step1, pink, 3, -2, 3, stair_factory);
	NEW_BOX(end_step2, pink, 3, -2, 4, stair_factory);
	NEW_BOX(end_step3, pink, 3, -2, 5, stair_factory);
	
	NEW_BOX(end_step4, pink, 3.5, -2, 3, stair_factory);
	end_step4->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(end_step5, pink, 4.5, -2, 3, stair_factory);
	end_step5->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	
	//-------------------------------------------------------------------------
	
	iRef<cObjectFactoryBox> lava_factory1 = new cObjectFactoryBox(7.75, 2, 5.8);
	
	NEW_BOX(lava1, red, -1.125, -2.2, 2.175, lava_factory1);
	lava1_physics_object->setInverseMass(0);
	untouchables.push_back(lava1);
	
	iRef<cObjectFactoryBox> lava_factory2 = new cObjectFactoryBox(7.5, 2, 4);
	
	NEW_BOX(lava2, red, -0.5, -2.2, -3, lava_factory2);
	lava2_physics_object->setInverseMass(0);
	untouchables.push_back(lava2);
	
	iRef<cObjectFactoryBox> lava_factory3 = new cObjectFactoryBox(8, 2, 2);
	
	NEW_BOX(lava3, red, 4, -2.2, -1, lava_factory3);
	lava3->rotate(CVector<3, float> (0, 1, 0), CMath<float>::PI()*0.5f);
	lava3_physics_object->setInverseMass(0);
	untouchables.push_back(lava3);
	
	
	iRef<cObjectFactoryBox> pendulum_factory = new cObjectFactoryBox(0.1, 0.5, 0.5);
	iRef<cObjectFactoryBox> pendulum_connector_factory = new cObjectFactoryBox(1, 1, 1);
	
	float pendulum1X = -2;
	float pendulum1Z = -1;
	
	NEW_SPHERE(pendulum1, red, pendulum1X, 2.5, pendulum1Z - 3, pendulum_factory);
	
	NEW_BOX(pendulum_connector1, white_noise, pendulum1X, 7, pendulum1Z, pendulum_connector_factory);
	pendulum_connector1_physics_object->setInverseMass(0);
	
	NEW_SPRING(pendulum1, pendulum_connector1);
	untouchables.push_back(pendulum1);
	
	
	float pendulum2X = 0;
	float pendulum2Z = -1;
	
	NEW_SPHERE(pendulum2, red, pendulum2X, 2.5, pendulum2Z + 3, pendulum_factory);
	
	NEW_BOX(pendulum_connector2, white_noise, pendulum2X, 7, pendulum2Z, pendulum_connector_factory);
	pendulum_connector2_physics_object->setInverseMass(0);
	
	NEW_SPRING(pendulum2, pendulum_connector2);
	untouchables.push_back(pendulum2);
	
	
	float pendulum3X = 2;
	float pendulum3Z = -1;
	
	NEW_SPHERE(pendulum3, red, pendulum3X, 2.5, pendulum3Z - 3, pendulum_factory);
	
	NEW_BOX(pendulum_connector3, white_noise, pendulum3X, 7, pendulum3Z, pendulum_connector_factory);
	pendulum_connector3_physics_object->setInverseMass(0);
	
	NEW_SPRING(pendulum3, pendulum_connector3);
	untouchables.push_back(pendulum3);
	
	//-------------------------------------------------------------------------
	
	NEW_SPHERE(coin4, green, -1, 2.5, -1, coin_factory);
	collectables.push_back(coin4);
	NEW_SPHERE(coin5, green, 1, 2.5, -1, coin_factory);
	collectables.push_back(coin5);
	NEW_SPHERE(coin6, green, -4.5, 0.5, -4.5, coin_factory);
	collectables.push_back(coin6);
}

void CGame::level3() {
	const float h = CMath<float>::PI(); // (H)alf a rotation
	const float q = h / 2; // A (q)uarter of a rotation
	const vec3f nv; // Null vector

	// Setup room borders (wall, floor, ceiling)
	addPlane(vec3f(0, 0, 0), vec2f(30, 50), nv, materials.boden_1); // Floor
	addPlane(vec3f(0, 12, 0), vec2f(30, 50), vec3f(h, 0, 0), materials.boden_1); // Ceiling
	addPlane(vec3f(0, 9, 5), vec2f(30, 20), vec3f(q, 0, 0), materials.wand_18); // South wall
	addPlane(vec3f(0, 9, -17), vec2f(30, 20), vec3f(-q, 0, 0), materials.wand_18); // North wall
	addPlane(vec3f(-10, 5, 0), vec2f(50, 30), vec3f(0, q, q), materials.wand_18); // West wall
	addPlane(vec3f(10, 5, 0), vec2f(50, 30), vec3f(0, -q, -q), materials.wand_18); // East wall

	// UPPER FLOOR AND STAIRS
	addBox(vec3f(0, 5, -10), vec3f(20, 0.2, 5), nv, materials.boden_1); // Pendulum floor

	// Stairs to upper floor
	addBox(vec3f(2, 1, -3), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(3, 2, -3), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(4, 3, -4), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(2, 3.5, -5), vec3f(1, 0.2, 1), nv, materials.white); // Collectable
	addBox(vec3f(2, 4.5, -2), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(4, 4, -6), vec3f(1, 0.2, 1), nv, materials.white);

	// Pendulum
	{
		auto box1 = addBox(vec3f(0, 11.5, -10), vec3f(0.01, 0.01, 0.01), nv, materials.red);
		auto box2 = addSphere(vec3f(0, 6.1, -10), 0.8, materials.red, 1);
		addSpring(box1, box2);
		auto length = (box1->object->position - box2->object->position).getLength();
		auto direction = vec3f(0, -1, -10).getNormalized();
		box2->object->position = box1->object->position + (direction * length);

		untouchables.push_back(box2->object);
		untouchables.push_back(addSphere(vec3f(0, 5.5, -10), 0.5, materials.red, 100)->object);

		auto enemy1PObject = addSphere(vec3f(-10, 5.6, -7.5), 0.5, materials.blue);
		auto enemy1 = new PathEnemy(enemy1PObject, engine);
		enemy1->addCheckpoint(CVector<3, float>(-2, 5.6, -7.5));
		enemy1->addCheckpoint(CVector<3, float>(-2, 5.6, -12.5));
		enemies.push_back(enemy1);
	}

	// Triggered pendulum
	{
		auto box1 = addBox(vec3f(2, 11.5, -10), vec3f(0.01, 0.01, 0.01), nv, materials.red);
		auto box2 = addSphere(vec3f(2, 6.1, -10), 0.8, materials.red);
		addSpring(box1, box2);
		auto length = (box1->object->position - box2->object->position).getLength();
		auto direction = vec3f(0, -1, -10).getNormalized();
		box2->object->position = box1->object->position + (direction * length);

		trigger1 = box2;
		untouchables.push_back(box2->object);
		untouchables.push_back(addSphere(vec3f(2, 5.5, -10), 0.5, materials.red, 10000)->object);
	}

	addBox(vec3f(-6.5, 5, 0), vec3f(7, 0.2, 10), nv, materials.boden_1); // Enemy floor

	auto enemy2PObject = addBox(vec3f(-6, 0.5, -3), vec3f(1, 1, 1), vec3f(0, 1, 1) * 0.4, materials.blue);
	enemies.push_back(new PathEnemy(enemy2PObject, engine));

	// Setup collectables
	collectables.push_back(addSphere(vec3f(-5, 5.3, -10), 0.2, materials.green, 1)->object);
	collectables.push_back(addSphere(vec3f(2, 3.8, -5), 0.2, materials.green, 1)->object);
}

iRef<iPhysicsObject> CGame::addObjectToEngine(iObjectFactory::Type type, vec3f pos, vec3f size, vec3f rotation, iRef<iGraphicsMaterial> &material, float invMass)
{
	iRef<iObjectFactory> factory;
	switch (type) {
		case iObjectFactory::TYPE_BOX:
			factory = new cObjectFactoryBox(size[0], size[1], size[2]);
			break;

		case iObjectFactory::TYPE_PLANE:
			factory = new cObjectFactoryPlane(size[0], size[2]);
			break;

		case iObjectFactory::TYPE_SPHERE:
			factory = new cObjectFactorySphere(size[0]);
			break;
	}

	iRef<iObject> object = new iObject();
	object->createFromFactory(*factory);
	object->translate(pos);
	object->rotate(vec3f(1, 0, 0), rotation[0]);
	object->rotate(vec3f(0, 1, 0), rotation[1]);
	object->rotate(vec3f(0, 0, 1), rotation[2]);
	iRef<iGraphicsObject> graphicsObject = new iGraphicsObject(object, material);
	engine.graphics.addObject(graphicsObject);
	engine.addObject(*object);
	iRef<iPhysicsObject> physicsObject = new iPhysicsObject(*object);
	physicsObject->setInverseMass(invMass);
	engine.physics.addObject(physicsObject);

	return physicsObject;
}

iRef<iPhysicsObject> CGame::addBox(const vec3f &pos, const vec3f &size, const vec3f &rotation, iRef<iGraphicsMaterial> &material, float invMass)
{
	return CGame::addObjectToEngine(iObjectFactory::TYPE_BOX, pos, size, rotation, material, invMass);
}

iRef<iPhysicsObject> CGame::addPlane(const vec3f &pos, const vec2f &size, const vec3f &rotation, iRef<iGraphicsMaterial> &material, float invMass)
{
	return CGame::addObjectToEngine(iObjectFactory::TYPE_PLANE, pos, vec3f(size[0], 0, size[1]), rotation, material, invMass);
}

iRef<iPhysicsObject> CGame::addSphere(const vec3f &pos, float radius, iRef<iGraphicsMaterial> &material, float invMass)
{
	return CGame::addObjectToEngine(iObjectFactory::TYPE_SPHERE, pos, vec3f(radius, 0, 0), vec3f(0, 0, 0), material, invMass);
}

void CGame::addRope(iRef<iPhysicsObject> o1, iRef<iPhysicsObject> o2)
{
	iRef<iPhysicsHardConstraint> rope = new cPhysicsHardConstraintRope(o1, o2, o1->object->position.dist(o2->object->position));
	o1->setDisableCollisionRotationAndFrictionFlag(true);
	o2->setDisableCollisionRotationAndFrictionFlag(true);
	engine.physics.addHardConstraint(rope);
	engine.graphics.addObjectConnector(new cGraphicsObjectConnectorCenter(o1->object, o2->object, materials.white));
}

void CGame::addSpring(iRef<iPhysicsObject> o1, iRef<iPhysicsObject> o2)
{
	iRef<iPhysicsSoftConstraint> spring = new cPhysicsSoftConstraintSpring(o1, o2, o1->object->position.dist(o2->object->position), 50);
	o1->setDisableCollisionRotationAndFrictionFlag(true);
	o2->setDisableCollisionRotationAndFrictionFlag(true);
	engine.physics.addSoftConstraint(spring);
	engine.graphics.addObjectConnector(new cGraphicsObjectConnectorCenter(o1->object, o2->object, materials.white));
}

CGame::CGame(iEngine &p_engine)	:
		engine(p_engine)
{
	setupMaterials();
}

CGame::~CGame()
{

}