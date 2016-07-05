#include "cGame.hpp"
#include "sbndengine/iSbndEngine.hpp"

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


void CGame::fourRooms()
{
	iRef<cObjectFactoryBox> wall_factory = new cObjectFactoryBox(0.5, 18, 20);
	NEW_PLANE(wall1, grey_noise, 0, 0, 0, wall_factory);
	wall1->rotate(CVector<3, float>(0, 1, 0), -CMath<float>::PI()*0.5f);
	wall1->rotate(CVector<3, float>(0, 0, 1), CMath<float>::PI()*0.5f);
	wall1_physics_object->setInverseMass(0);
			
			
	NEW_PLANE(wall3, grey_noise, 0, 0, 0, wall_factory);
	wall3->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI());
	wall3->rotate(CVector<3, float>(1, 0, 0), CMath<float>::PI()*0.5f);
	wall3_physics_object->setInverseMass(0);
}


void CGame::setupGameScene() {
	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> coin_factory = new cObjectFactorySphere(0.1);
	
	fourRooms();
	
//stairs
{
	float stairX = 4;
	float stairZ = -4;
	
	//-------------------------------------------------------------------------
	
	iRef<cObjectFactoryBox> stair_factory = new cObjectFactoryBox(0.5, 2, 0.5);
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

//ramp
{
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

//sen's fortress
{
	float sens_fortressX = -5;
	float sens_fortressZ = 5;
	
	iRef<cObjectFactoryBox> stair_factory = new cObjectFactoryBox(0.5, 2, 0.5);
	stair_factory->setInverseMass(0);
	
	iRef<cObjectFactoryBox> path_factory = new cObjectFactoryBox(0.5, 4, 2);
	path_factory->setInverseMass(0);
	
	
	NEW_BOX(front_step1, pink, sens_fortressX - 4.5, -2, sens_fortressZ - 3.75, stair_factory);
	NEW_BOX(front_step2, pink, sens_fortressX - 4.5, -2, sens_fortressZ - 3.25, stair_factory);
	
	
	NEW_BOX(path1, pink, sens_fortressX - 4.5, -1, sens_fortressZ - 2, path_factory);
	
	NEW_BOX(path2, pink, sens_fortressX - 3.75, -1, sens_fortressZ - 1, path_factory);
	path2->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path3, pink, sens_fortressX - 1.75, -1, sens_fortressZ - 1, path_factory);
	path3->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path4, pink, sens_fortressX + 0.25, -1, sens_fortressZ - 1, path_factory);
	path4->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	NEW_BOX(path5, pink, sens_fortressX + 2.25, -1, sens_fortressZ - 1, path_factory);
	path5->rotate(CVector<3, float>(0, 1, 0), CMath<float>::PI()*0.5f);
	
	NEW_BOX(path6, pink, sens_fortressX + 3, -1, sens_fortressZ, path_factory);
	NEW_BOX(path7, pink, sens_fortressX + 3, -1, sens_fortressZ + 2, path_factory);
	
	
	NEW_BOX(end_step1, pink, sens_fortressX + 3, -2, sens_fortressZ + 3, stair_factory);
	NEW_BOX(end_step2, pink, sens_fortressX + 3, -2, sens_fortressZ + 3.5, stair_factory);
	NEW_BOX(end_step3, pink, sens_fortressX + 3, -2, sens_fortressZ + 4, stair_factory);
	
	//-------------------------------------------------------------------------
	
	iRef<cObjectFactoryBox> lava_factory1 = new cObjectFactoryBox(7.75, 2, 5.8);
	
	NEW_BOX(lava1, red, sens_fortressX - 1.125, -3, sens_fortressZ + 2.175, lava_factory1);
	lava1_physics_object->setInverseMass(0);
	
	iRef<cObjectFactoryBox> lava_factory2 = new cObjectFactoryBox(7.5, 2, 4);
	
	NEW_BOX(lava2, red, sens_fortressX - 0.5, -3, sens_fortressZ - 3, lava_factory2);
	lava2_physics_object->setInverseMass(0);
	
	iRef<cObjectFactoryBox> lava_factory3 = new cObjectFactoryBox(7.5, 2, 4);
	
	NEW_BOX(lava3, red, sens_fortressX - 0.5, -3, sens_fortressZ - 3, lava_factory3);
	lava3_physics_object->setInverseMass(0);
}
}


CGame::CGame(iEngine &p_engine)	:
		engine(p_engine)
{
	setupMaterials();
}


CGame::~CGame()
{

}