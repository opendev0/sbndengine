#include "cScenes.hpp"
#include "sbndengine/iSbndEngine.hpp"

#define TEXTURE_PATH "src/textures/"


/**
 * helper macros to speed-up creation of world scenes
 *
 * see code below how to use these macros
 */

#define NEW_PLANE(plane_id, color, px, py, pz)							\
		iRef<iObject> plane_id = new iObject(#plane_id);				\
		plane_id->createFromFactory(*plane_factory);					\
		plane_id->translate(px, py, pz);														\
		iRef<iGraphicsObject> plane_id##_graphics_object = new iGraphicsObject(plane_id, materials.color);	\
		engine.graphics.addObject(plane_id##_graphics_object);			\
		engine.addObject(*plane_id);									\
		iRef<iPhysicsObject> plane_id##_physics_object = new iPhysicsObject(*plane_id);			\
		plane_id##_physics_object->setInverseMass(0);							\
		engine.physics.addObject(plane_id##_physics_object);


#define NEW_SPHERE(sphere_id, color, px, py, pz)						\
		iRef<iObject> sphere_id = new iObject(""#sphere_id);			\
		sphere_id->createFromFactory(*sphere_factory);					\
		sphere_id->translate(px, py, pz);								\
		iRef<iGraphicsObject> sphere_id##_graphics_object = new iGraphicsObject(sphere_id, materials.color);	\
		engine.graphics.addObject(sphere_id##_graphics_object);			\
		engine.addObject(*sphere_id);									\
		iRef<iPhysicsObject> sphere_id##_physics_object = new iPhysicsObject(*sphere_id);	\
		engine.physics.addObject(sphere_id##_physics_object);


#define NEW_BOX(box_id, color, px, py, pz)								\
		iRef<iObject> box_id = new iObject(""#box_id);					\
		box_id->createFromFactory(*box_factory);						\
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
void CScenes::setupMaterials()
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
	texture->createTextureFromFile(TEXTURE_PATH "boden_1.jpg");
	materials.boden_1 = new iGraphicsMaterial;
	materials.boden_1->setTexture(texture);
	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "boden_1Normal.jpg");
	materials.boden_1->setNormalTexture(texture);

	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "wand_18.jpg");
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
void CScenes::setupWorldBoxPlanes(float scale)
{
	/*
	 * setup objects
	 */
	iRef<iPhysicsObject> new_physics_object;

	iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(20.f*scale, 20.f*scale);
	plane_factory->setInverseMass(0);

	NEW_PLANE(plane0, boden_1, 0, -3*scale, 0);
	NEW_PLANE(plane1, wand_18, 0, 0, -10*scale);
	plane1->rotate(CVector<3,float>(1, 0, 0), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane2, wand_18, -10*scale, 0, 0);
	plane2->rotate(CVector<3,float>(0, 1, 0), -CMath<float>::PI()*0.5f);
	plane2->rotate(CVector<3,float>(0, 0, 1), CMath<float>::PI()*0.5f);
	NEW_PLANE(plane3, wand_18, 10*scale, 0, 0);
	plane3->rotate(CVector<3,float>(0, 1, 0), CMath<float>::PI()*0.5f);
	plane3->rotate(CVector<3,float>(0, 0, 1), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane4, wand_18, 0, 0, 10*scale);
	plane4->rotate(CVector<3,float>(0, 1, 0), -CMath<float>::PI());
	plane4->rotate(CVector<3,float>(1, 0, 0), CMath<float>::PI()*0.5f);
}

/**
 * different scenes are created here
 */

void CScenes::setupScene1()
{
	scene_description = "Standard test case";

	// setup planes
	setupWorldBoxPlanes();

	// create a new box factory
	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(4,2,1);

	// using the box_factory, a bunch of boxes are created by the macro
	// NEW_BOX. this automagically inserts the box into the physics, graphics
	// and engine classes
	NEW_BOX(box0, green, 0, 5, 0);
	// rotate box0
	box0->rotate(CVector<3,float>(0.5f, 0.5f, 0.5f).getNormalized(), -CMath<float>::PI()*0.1f);
	NEW_BOX(box1, red, -3.5f, 1, 4);
	box1->rotate(CVector<3,float>(0.1f, 0.2f, 0.5f).getNormalized(), CMath<float>::PI()*0.522f);
	NEW_BOX(box2, blue, -0.0f, 0, 0.9f);
	box2->rotate(CVector<3,float>(0.1f, -0.2f, 0.4f).getNormalized(), -CMath<float>::PI()*0.9522f);
	NEW_BOX(box3, red, 0, 0, 0);
	box3->rotate(CVector<3,float>(0, 1, 0).getNormalized(), -CMath<float>::PI()*0.25f);
	NEW_BOX(box4, green, 5, 1, 0);
	box4->rotate(CVector<3,float>(0, 0, 1).getNormalized(), -CMath<float>::PI()*0.25f);
	NEW_BOX(box5, green, 0.01f, 0, 0.01f);
	NEW_BOX(box6, red, 0, 4, 0);

	// create a sphere factory
	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(3);

	// create new sphere physics, graphics and engine instances
	NEW_SPHERE(sphere0, blue, -2, 3, 0);
	NEW_SPHERE(sphere1, white, -4, 3, -3.2f);
	NEW_SPHERE(sphere2, red, 3, 4, -0.7f);
	NEW_SPHERE(sphere3, green, 3.5f, 5.2f, -2.5f);

	// new box factory with different size
	box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(5,5,5);

	NEW_BOX(box10, yellow, 6, 5, -5);
	box10->rotate(CVector<3,float>(0.5f, 0.5f, 0.5f).getNormalized(), -CMath<float>::PI()*0.3f);

	NEW_BOX(box11, pink, -7, 6, -5);
	box11->rotate(CVector<3,float>(0.5f, 0.5f, 0.5f).getNormalized(), -CMath<float>::PI()*1.13f);

	// resolve interpenetrations to avoid any initial collisions
	engine.physics.detectAndResolveInterpenetrations();
}

void CScenes::setupScene2()
{
	scene_description = "Boxes with different sizes and rotation";

	setupWorldBoxPlanes();

	iRef<iPhysicsObject> new_physics_object;

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(2,2,2);

	NEW_BOX(box9, green, -6, -2, 0);
	NEW_BOX(box10, red, -6.5f, 0.5f, 0);
	box10->setRotation(CVector<3,float>(1,1,1).getNormalized(), 0.2f);

	NEW_BOX(box1, green, -3, -2, 0);
	NEW_BOX(box2, red, -3, 3, 0);

	NEW_BOX(box3, green, 0, -2, 0);
	NEW_BOX(box4, red, 0, 0, 0);

	NEW_BOX(box5, green, 3.01f, -2, 0.01f);
	NEW_BOX(box6, red, 3, 4, 0);

	NEW_BOX(box7, green, 6, -2, 0);
	box7->setRotation(CVector<3,float>(1,1,1).getNormalized(), -0.1f);
	NEW_BOX(box8, red, 6, 0.1f, 0);
	box8->setRotation(CVector<3,float>(1,-1,1).getNormalized(), -0.1f);

	engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
}


void CScenes::setupScene3()
{
	scene_description = "Fixed box with box attachted by a spring";

//		setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(3,3,3);
	iRef<iPhysicsSoftConstraint> spring;
///
	NEW_BOX(box_red_lu, red, -10, 10, 0);
	box_red_lu->rotate(CVector<3,float>(0, 1, 0).getNormalized(), -CMath<float>::PI()*0.25f);
	box_red_lu_physics_object->setInverseMass(0);

	NEW_BOX(box_green_lu, green, -6, 4, 0);
	box_green_lu->rotate(CVector<3,float>(0, 0, 1).getNormalized(), -CMath<float>::PI()*0.25f);
	box_green_lu_physics_object->setAngularSpeed(CVector<3,float>(0.914f, 0.12f, 1));

	spring = new cPhysicsSoftConstraintSpring(box_red_lu_physics_object, box_green_lu_physics_object, 0, 100, 0);
	engine.physics.addSoftConstraint(spring);
///
	NEW_BOX(box_red_middle, red, 0, 3, 0);
	box_red_middle->rotate(CVector<3,float>(0, 1, 0).getNormalized(), -CMath<float>::PI()*0.25f);
	box_red_middle_physics_object->setInverseMass(0);

	NEW_BOX(box_green_middle, green, 4, -4, 0);
	box_green_middle->rotate(CVector<3,float>(0, 0, 1).getNormalized(), -CMath<float>::PI()*2.25f);
	box_green_middle_physics_object->setAngularSpeed(CVector<3,float>(0.914f, 0.12f, 1));

	spring = new cPhysicsSoftConstraintSpring(box_red_middle_physics_object, box_green_middle_physics_object, 0, 200, 0);
	engine.physics.addSoftConstraint(spring);
///

	NEW_BOX(box_red_lb, red, -10, -3, 0);
	box_red_lb->rotate(CVector<3,float>(0, 1, 0).getNormalized(), -CMath<float>::PI()*1.25f);
	box_red_lb_physics_object->setInverseMass(0);

	NEW_BOX(box_green_lb, green, -4, 0, 0);
	box_green_lb->rotate(CVector<3,float>(0, 0, 1).getNormalized(), -CMath<float>::PI()*0.25f);
	box_green_lb_physics_object->setAngularSpeed(CVector<3,float>(0.914f, 0.12f, 1));

	spring = new cPhysicsSoftConstraintSpring(box_red_lb_physics_object, box_green_lb_physics_object, 0, 100, 0);
	engine.physics.addSoftConstraint(spring);
//
	NEW_BOX(box_red_ru, red, 12, 12, 0);
	box_red_ru->rotate(CVector<3,float>(0, 1, 0).getNormalized(), -CMath<float>::PI()*0.001f);

	NEW_BOX(box_green_ru, green, 6, 6, 0);
	box_green_ru->rotate(CVector<3,float>(0, 0, 1).getNormalized(), -CMath<float>::PI()*0.001f);

	spring = new cPhysicsSoftConstraintSpring(box_red_ru_physics_object, box_green_ru_physics_object, 1, 100, 0);
	engine.physics.addSoftConstraint(spring);
//
	NEW_BOX(box_red_rb, red, 12, -2, 0);
	NEW_BOX(box_green_rb, green, 6, -6, 0);

	spring = new cPhysicsSoftConstraintSpring(box_red_rb_physics_object, box_green_rb_physics_object, 1, 100, 0);
	engine.physics.addSoftConstraint(spring);

	engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
}

void CScenes::setupScene4()
{
	setupWorldBoxPlanes(2);

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(0.5,10,10);

	NEW_BOX(box_red_rb, yellow, -4, 2, 0);

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere;
	sphere_factory->resizeSphere(2);

	NEW_SPHERE(sphere0, pink, 5, 6, 0);
	sphere0_physics_object->setInverseMass(box_factory->inv_mass);

	engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
}


void CScenes::setupScene5()
{
	scene_description = "Newton pendulum - not working - explain";

	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(1);
/*	sphere_factory->mass = 10;
	sphere_factory->inv_mass = 0.1;*/

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(5,5,5);
	box_factory->setMass(1);
	NEW_BOX(box1, red, -5.5, -4, -5.5);
	NEW_BOX(box2, green, 5.5, -2, -5.5);

	NEW_SPHERE(anchor0, white, -4, 8, 0);
	anchor0_physics_object->setInverseMass(0);
	NEW_SPHERE(ball0, red, -4, 4, 0);
	NEW_ROPE(anchor0, ball0);

	NEW_SPHERE(anchor1, white, -2, 8, 0);
	anchor1_physics_object->setInverseMass(0);
	NEW_SPHERE(ball1, red, -2, 4, 0);
	NEW_ROPE(anchor1, ball1);

	NEW_SPHERE(anchor2, white, 0, 8, 0);
	anchor2_physics_object->setInverseMass(0);
	NEW_SPHERE(ball2, red, 0, 4, 0);
	NEW_ROPE(anchor2, ball2);

	NEW_SPHERE(anchor3, white, 2, 8, 0);
	anchor3_physics_object->setInverseMass(0);
	NEW_SPHERE(ball3, red, 2, 4, 0);
	NEW_ROPE(anchor3, ball3);

	NEW_SPHERE(anchor4, white, 4, 8, 0);
	anchor4_physics_object->setInverseMass(0);
	NEW_SPHERE(ball4, red, 8, 8, 0);
	NEW_ROPE(anchor4, ball4);
}


void CScenes::setupScene6()
{
	scene_description = "Box with a spring connected to a fixed ceiling box";

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(2,2,2);

	NEW_BOX(box3, red, 0, -4, 0);
	NEW_BOX(box4, green, 0, 4, 0);
	box4_physics_object->setInverseMass(0);

	iRef<iPhysicsSoftConstraint> spring;
	spring = new cPhysicsSoftConstraintSpring(box3_physics_object, box4_physics_object, 5, 500, 0.05f);
	engine.physics.addSoftConstraint(spring);
}


void CScenes::setupScene7()
{
	scene_description = "Bridge simulation with springs";

	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(0.5);

	NEW_SPHERE(anchor0, white, -10, 8, -3);
	anchor0_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor1, white, -10, 8, 3);
	anchor1_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor2, white, 10, 8, -3);
	anchor2_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor3, white, 10, 8, 3);
	anchor3_physics_object->setInverseMass(0);


	NEW_SPHERE(ball0, red, -6, 2, -2);
	NEW_SPHERE(ball1, red, -6, 2, 2);

	NEW_SPHERE(ball2, red, -2, 2, -2);
	NEW_SPHERE(ball3, red, -2, 2, 2);

	NEW_SPHERE(ball4, red, 2, 2, -2);
	NEW_SPHERE(ball5, red, 2, 2, 2);

	NEW_SPHERE(ball6, red, 6, 2, -2);
	NEW_SPHERE(ball7, red, 6, 2, 2);


	NEW_SPRING(anchor0, ball0);
	NEW_SPRING(anchor1, ball1);
	NEW_SPRING(anchor2, ball6);
	NEW_SPRING(anchor3, ball7);

	NEW_SPRING(ball0, ball1);
	NEW_SPRING(ball0, ball2);
	NEW_SPRING(ball1, ball3);
	NEW_SPRING(ball2, ball3);

	NEW_SPRING(ball2, ball4);
	NEW_SPRING(ball3, ball5);
	NEW_SPRING(ball4, ball5);

	NEW_SPRING(ball4, ball6);
	NEW_SPRING(ball5, ball7);
	NEW_SPRING(ball6, ball7);
}


void CScenes::setupScene8()
{
	scene_description = "Bridge simulation with ropes";

	setupWorldBoxPlanes();
	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(0.5);

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(5,5,5);
	box_factory->setMass(0.1);
	NEW_BOX(box1, red, -5.5, -4, -5.5);
	NEW_BOX(box2, green, 5.5, -2, -5.5);

	NEW_SPHERE(anchor0, white, -10, 8, -3);
	anchor0_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor1, white, -10, 8, 3);
	anchor1_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor2, white, 10, 8, -3);
	anchor2_physics_object->setInverseMass(0);
	NEW_SPHERE(anchor3, white, 10, 8, 3);
	anchor3_physics_object->setInverseMass(0);


	NEW_SPHERE(ball0, red, -6, 2, -2);
	NEW_SPHERE(ball1, red, -6, 2, 2);

	NEW_SPHERE(ball2, red, -2, 2, -2);
	NEW_SPHERE(ball3, red, -2, 2, 2);

	NEW_SPHERE(ball4, red, 2, 2, -2);
	NEW_SPHERE(ball5, red, 2, 2, 2);

	NEW_SPHERE(ball6, red, 6, 2, -2);
	NEW_SPHERE(ball7, red, 6, 2, 2);


	NEW_ROPE(anchor0, ball0);
	NEW_ROPE(anchor1, ball1);
	NEW_ROPE(anchor2, ball6);
	NEW_ROPE(anchor3, ball7);

	NEW_ROPE(ball0, ball1);
	NEW_ROPE(ball0, ball2);
	NEW_ROPE(ball1, ball3);
	NEW_ROPE(ball2, ball3);

	NEW_ROPE(ball2, ball4);
	NEW_ROPE(ball3, ball5);
	NEW_ROPE(ball4, ball5);

	NEW_ROPE(ball4, ball6);
	NEW_ROPE(ball5, ball7);
	NEW_ROPE(ball6, ball7);
}


void CScenes::setupScene9()
{
	scene_description = "2 Balls connected by a rope at their center of mass";

	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(2);

	NEW_SPHERE(ball0, red, -5, 0, -4);
	NEW_SPHERE(ball1, green, 5, 0, -4);

	NEW_SPHERE(ball2, red, 5, 5, 3);
	NEW_SPHERE(ball3, green, 5, 0, 3);
	ball3_physics_object->setInverseMass(0);

	NEW_ROPE(ball0, ball1);
}


void CScenes::setupScene10()
{
	scene_description = "2 Boxes connected by a rope";

	setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(1,4,1);

	NEW_BOX(box0, red, -0.75f, 0, 0);
	NEW_BOX(box1, green, 0.75f, 0, 0);

	NEW_ROPE_ANGULAR(box0, (CVector<3,float>(0,2,0)), box1, (CVector<3,float>(0,2,0)), 0.0f);
}

void CScenes::setupScene11()
{
	scene_description = "Mass ball simulation";

	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(1.5);

	NEW_SPHERE(ball000, red,	-8, 0, -4);
	NEW_SPHERE(ball100, green,	-4, 0, -4);
	NEW_SPHERE(ball200, red,	0, 0, -4);
	NEW_SPHERE(ball300, green,	4, 0, -4);
	NEW_SPHERE(ball400, red,	8, 0, -4);

	NEW_SPHERE(ball001, red,	-8, 0, 0);
	NEW_SPHERE(ball101, green,	-4, 0, 0);
	NEW_SPHERE(ball201, red,	0, 0, 0);
	NEW_SPHERE(ball301, green,	4, 0, 0);
	NEW_SPHERE(ball401, red,	8, 0, 0);

	NEW_SPHERE(ball002, red,	-8, 0, 4);
	NEW_SPHERE(ball102, green,	-4, 0, 4);
	NEW_SPHERE(ball202, red,	0, 0, 4);
	NEW_SPHERE(ball302, green,	4, 0, 4);
	NEW_SPHERE(ball402, red,	8, 0, 4);
//
	NEW_SPHERE(ball010, red,	-8, 4, -4);
	NEW_SPHERE(ball110, green,	-4, 4, -4);
	NEW_SPHERE(ball210, red,	0, 4, -4);
	NEW_SPHERE(ball310, green,	4, 4, -4);
	NEW_SPHERE(ball410, red,	8, 4, -4);

	NEW_SPHERE(ball011, red,	-8, 4, 0);
	NEW_SPHERE(ball111, green,	-4, 4, 0);
	NEW_SPHERE(ball211, red,	0, 4, 0);
	NEW_SPHERE(ball311, green,	4, 4, 0);
	NEW_SPHERE(ball411, red,	8, 4, 0);

	NEW_SPHERE(ball012, red,	-8, 4, 4);
	NEW_SPHERE(ball112, green,	-4, 4, 4);
	NEW_SPHERE(ball212, red,	0, 4, 4);
	NEW_SPHERE(ball312, green,	4, 4, 4);
	NEW_SPHERE(ball412, red,	8, 4, 4);
//
	NEW_SPHERE(ball020, red,	-8, 8, -4);
	NEW_SPHERE(ball120, green,	-4, 8, -4);
	NEW_SPHERE(ball220, red,	0, 8, -4);
	NEW_SPHERE(ball320, green,	4, 8, -4);
	NEW_SPHERE(ball420, red,	8, 8, -4);

	NEW_SPHERE(ball021, red,	-8, 8, 0);
	NEW_SPHERE(ball121, green,	-4, 8, 0);
	NEW_SPHERE(ball221, red,	0, 8, 0);
	NEW_SPHERE(ball321, green,	4, 8, 0);
	NEW_SPHERE(ball421, red,	8, 8, 0);

	NEW_SPHERE(ball022, red,	-8, 8, 4);
	NEW_SPHERE(ball122, green,	-4, 8, 4);
	NEW_SPHERE(ball222, red,	0, 8, 4);
	NEW_SPHERE(ball322, green,	4, 8, 4);
	NEW_SPHERE(ball422, red,	8, 8, 4);
}


void CScenes::setupScene12()
{
	scene_description = "Mass box simulation";

	setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(2, 2, 2.5);

	NEW_BOX(box000, red,	-8, 0, -4);
	NEW_BOX(box100, green,	-4, 0, -4);
	NEW_BOX(box200, red,	0, 0, -4);
	NEW_BOX(box300, green,	4, 0, -4);
	NEW_BOX(box400, red,	8, 0, -4);

	NEW_BOX(box001, red,	-8, 0, 0);
	NEW_BOX(box101, green,	-4, 0, 0);
	NEW_BOX(box201, red,	0, 0, 0);
	NEW_BOX(box301, green,	4, 0, 0);
	NEW_BOX(box401, red,	8, 0, 0);

	NEW_BOX(box002, red,	-8, 0, 4);
	NEW_BOX(box102, green,	-4, 0, 4);
	NEW_BOX(box202, red,	0, 0, 4);
	NEW_BOX(box302, green,	4, 0, 4);
	NEW_BOX(box402, red,	8, 0, 4);
//
	NEW_BOX(box010, red,	-8, 4, -4);
	NEW_BOX(box110, green,	-4, 4, -4);
	NEW_BOX(box210, red,	0, 4, -4);
	NEW_BOX(box310, green,	4, 4, -4);
	NEW_BOX(box410, red,	8, 4, -4);

	NEW_BOX(box011, red,	-8, 4, 0);
	NEW_BOX(box111, green,	-4, 4, 0);
	NEW_BOX(box211, red,	0, 4, 0);
	NEW_BOX(box311, green,	4, 4, 0);
	NEW_BOX(box411, red,	8, 4, 0);

	NEW_BOX(box012, red,	-8, 4, 4);
	NEW_BOX(box112, green,	-4, 4, 4);
	NEW_BOX(box212, red,	0, 4, 4);
	NEW_BOX(box312, green,	4, 4, 4);
	NEW_BOX(box412, red,	8, 4, 4);
//
	NEW_BOX(box020, red,	-8, 8, -4);
	NEW_BOX(box120, green,	-4, 8, -4);
	NEW_BOX(box220, red,	0, 8, -4);
	NEW_BOX(box320, green,	4, 8, -4);
	NEW_BOX(box420, red,	8, 8, -4);

	NEW_BOX(box021, red,	-8, 8, 0);
	NEW_BOX(box121, green,	-4, 8, 0);
	NEW_BOX(box221, red,	0, 8, 0);
	NEW_BOX(box321, green,	4, 8, 0);
	NEW_BOX(box421, red,	8, 8, 0);

	NEW_BOX(box022, red,	-8, 8, 4);
	NEW_BOX(box122, green,	-4, 8, 4);
	NEW_BOX(box222, red,	0, 8, 4);
	NEW_BOX(box322, green,	4, 8, 4);
	NEW_BOX(box422, red,	8, 8, 4);
}

void CScenes::setupScene13()
{
	scene_description = "Mass Ball/Box simulation";

	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(1.5);
	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(3, 3, 3);

	NEW_SPHERE(ball000, red,	-8, 0, -4);
	NEW_SPHERE(ball100, green,	-4, 0, -4);
	NEW_SPHERE(ball200, red,	0, 0, -4);
	NEW_SPHERE(ball300, green,	4, 0, -4);
	NEW_SPHERE(ball400, red,	8, 0, -4);

	NEW_BOX(ball001, red,	-8, 0, 0);
	NEW_BOX(ball101, green,	-4, 0, 0);
	NEW_BOX(ball201, red,	0, 0, 0);
	NEW_BOX(ball301, green,	4, 0, 0);
	NEW_BOX(ball401, red,	8, 0, 0);

	NEW_SPHERE(ball002, red,	-8, 0, 4);
	NEW_SPHERE(ball102, green,	-4, 0, 4);
	NEW_SPHERE(ball202, red,	0, 0, 4);
	NEW_SPHERE(ball302, green,	4, 0, 4);
	NEW_SPHERE(ball402, red,	8, 0, 4);
//
	NEW_BOX(ball010, red,	-8, 4, -4);
	NEW_BOX(ball110, green,	-4, 4, -4);
	NEW_BOX(ball210, red,	0, 4, -4);
	NEW_BOX(ball310, green,	4, 4, -4);
	NEW_BOX(ball410, red,	8, 4, -4);

	NEW_SPHERE(ball011, red,	-8, 4, 0);
	NEW_SPHERE(ball111, green,	-4, 4, 0);
	NEW_SPHERE(ball211, red,	0, 4, 0);
	NEW_SPHERE(ball311, green,	4, 4, 0);
	NEW_SPHERE(ball411, red,	8, 4, 0);

	NEW_SPHERE(ball012, red,	-8, 4, 4);
	NEW_SPHERE(ball112, green,	-4, 4, 4);
	NEW_SPHERE(ball212, red,	0, 4, 4);
	NEW_SPHERE(ball312, green,	4, 4, 4);
	NEW_SPHERE(ball412, red,	8, 4, 4);
//
	NEW_BOX(ball020, red,	-8, 8, -4);
	NEW_BOX(ball120, green,	-4, 8, -4);
	NEW_BOX(ball220, red,	0, 8, -4);
	NEW_BOX(ball320, green,	4, 8, -4);
	NEW_BOX(ball420, red,	8, 8, -4);

	NEW_SPHERE(ball021, red,	-8, 8, 0);
	NEW_SPHERE(ball121, green,	-4, 8, 0);
	NEW_SPHERE(ball221, red,	0, 8, 0);
	NEW_SPHERE(ball321, green,	4, 8, 0);
	NEW_SPHERE(ball421, red,	8, 8, 0);

	NEW_SPHERE(ball022, red,	-8, 8, 4);
	NEW_SPHERE(ball122, green,	-4, 8, 4);
	NEW_SPHERE(ball222, red,	0, 8, 4);
	NEW_SPHERE(ball322, green,	4, 8, 4);
	NEW_SPHERE(ball422, red,	8, 8, 4);
}


void CScenes::setupScene14()
{
	scene_description = "2 spheres - left: velocity based collision resolution, right: impulse based collision resolution";

	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(2);

	NEW_SPHERE(sphere0, red,	-3, 8, 0);
	sphere0_physics_object->setDisableCollisionRotationAndFrictionFlag(true);

	NEW_SPHERE(sphere1, red,	3, 8, 0);

	engine.physics.setGravitation(CVector<3,float>(0, -100, 0));
}

void CScenes::setupScene15()
{
	scene_description = "boxes: just some boxes";

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);

	setupWorldBoxPlanes();
#if 0
	NEW_BOX(box_green, green,	-5, 5, 0);
	box_green_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,0,1).getNormalized(), CMath<float>::PI()*0.1));
	box_green_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(1,0,0).getNormalized(), CMath<float>::PI()*0.1));
	box_green_physics_object->setInverseMass(0);

	NEW_BOX(box_red, red,	5, 5, 0);
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,0,1).getNormalized(), CMath<float>::PI()*-0.1));
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(1,0,0).getNormalized(), CMath<float>::PI()*-0.1));
#elif 0
	NEW_BOX(box_red, red,	5, 5, 0);
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,0,1).getNormalized(), CMath<float>::PI()*-0.1));
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(1,0,0).getNormalized(), CMath<float>::PI()*-0.1));

	NEW_BOX(box_green, green,	-5, 5, 0);
	box_green_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,0,1).getNormalized(), CMath<float>::PI()*0.1f));
	box_green_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(1,0,0).getNormalized(), CMath<float>::PI()*0.1f));
	box_green_physics_object->setInverseMass(0);
#else
	NEW_BOX(box_red, red,	3, 0, 0);
//		box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,1,1).getNormalized(), CMath<float>::PI()*-0.1f));
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,0,1).getNormalized(), -CMath<float>::PI()*-0.25f));
	box_red_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,1,0).getNormalized(), -CMath<float>::PI()*-0.25f));

	NEW_BOX(box_green, green,	-2, 0, 0);
	box_green_physics_object->setInverseMass(0);

#endif
}


void CScenes::setupScene16()
{
	scene_description = "BOX unrotated";

	/*
	 * setup objects
	 */
	setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);

	NEW_BOX(box0, red,	0, -2, 0);
	NEW_BOX(box1, green, -5, -2, 0);


	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(2);
	NEW_SPHERE(sphere0, yellow, 5, -2, 0);

//		engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
}


void CScenes::setupScene17()
{
	scene_description = "BOX rotated";

	setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);

	NEW_BOX(box, red,	0, 5, 0);
	box_physics_object->object->rotate(CQuaternion<float>(CVector<3,float>(0,1,0).getNormalized(), -CMath<float>::PI()));

	box_physics_object->object->rotation.normalize();
	box_physics_object->object->updateModelMatrix();

	engine.physics.setGravitation(CVector<3,float>(0, 0, 0));
}


void CScenes::setupScene18()
{
	scene_description = "BOX rotated";

	setupWorldBoxPlanes();

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);

	NEW_BOX(box, red,	0, 5, 0);
	box_physics_object->setAngularSpeed(CVector<3,float>(0,1,-1)*0.3f);

	engine.physics.setGravitation(CVector<3,float>(0, -9.81f, 0));
}


void CScenes::setupScene19()
{
	scene_description = "BOX rotated";

	/*
	 * setup objects
	 */
	iRef<iPhysicsObject> new_physics_object;

	iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(25.f, 25.f);
	plane_factory->setInverseMass(0);

	NEW_PLANE(plane0, red, 0, -3, 0);
	plane0->rotate(CVector<3,float>(2.4f, -0.3f, -2.4f).getNormalized(), CMath<float>::PI()*0.1f);
	NEW_PLANE(plane1, yellow, 0, 0, -10);
	plane1->rotate(CVector<3,float>(1, 0, 0), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane2, cyan, -10, 0, 0);
	plane2->rotate(CVector<3,float>(0, 0, 1), CMath<float>::PI()*0.5f);
	NEW_PLANE(plane3, cyan, 10, 0, 0);
	plane3->rotate(CVector<3,float>(0, 0, 1), -CMath<float>::PI()*0.5f);
	NEW_PLANE(plane4, yellow, 0, 0, 10);
	plane4->rotate(CVector<3,float>(1, 0, 0), CMath<float>::PI()*0.5f);

	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);
	NEW_BOX(box, red,	-4, 5, 0);
	box_physics_object->setAngularSpeed(CVector<3,float>(1,0,-1)*0.9f);


	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(4);
	NEW_SPHERE(sphere, pink,	4, 5, 0);

	engine.physics.setGravitation(CVector<3,float>(0, -9.81f, 0));
}


void CScenes::setupScene20()
{
	scene_description = "Sphere";

	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(4);

	NEW_SPHERE(sphere, pink,	0, 5, 0);

	engine.physics.setGravitation(CVector<3,float>(0, -9.81f, 0));
}


/**
 * LAB WORKSHEET 4, ASSIGNMENT 3
 *
 * setup a scene by yourself
 */
void CScenes::setupScene21()
{
	scene_description = "Boxes fixed to spheres using springs";

	setupWorldBoxPlanes();
    
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(3, 3, 3);
    
    NEW_BOX(box0, green, 0, 3, 0);
    box0_physics_object->setInverseMass(0);
    box0->rotate(CVector <3, float> (1, 0, 0), CMath<float>::PI()*0.25f);
    box0->rotate(CVector <3, float> (0, 0, 1), CMath<float>::PI()*0.25f);
    
    NEW_BOX(box1, red, 0, 9, 0);
    
    engine.physics.setGravitation(CVector <3, float> (0, -4, 0));
}


/**
 * LAB WORKSHEET ?, ASSIGNMENT ?
 *
 */
void CScenes::setupScene22()
{
    //tests for sphere/box collisions
    setupWorldBoxPlanes();
        
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
    box_factory->resizeBox(3,3,3);
    iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(2);
    
    NEW_BOX(box, green, 0, 3, 0);
    box_physics_object->setInverseMass(0);
    box->rotate(CVector <3, float> (1, 0, 0), CMath<float>::PI()*0.25f);
    box->rotate(CVector <3, float> (0, 0, 1), CMath<float>::PI()*0.25f);
        
        
    NEW_SPHERE(sphere0, red, 0, 9, 0);
        
    engine.physics.setGravitation(CVector <3, float> (0, -4, 0));
}


void CScenes::setupScene23()
{
    //tests for box/plane collisions
    setupWorldBoxPlanes();
    
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
    box_factory->resizeBox(3, 3, 3);
    
    NEW_BOX(box, red, 0, 3, 0);
    box->rotate(CVector <3, float> (1, 0, 0), CMath<float>::PI()*0.25f);
    box->rotate(CVector <3, float> (0, 0, 1), CMath<float>::PI()*0.25f);
    
    engine.physics.setGravitation(CVector <3, float> (0, -4, 0));
}

   
void CScenes::setupScene24()
{     
    //tests for box/plane collisions edge to edge
    iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(9.f, 9.f);
    plane_factory->setInverseMass(0);

    NEW_PLANE(plane1, red, 0, 0, 0);
    plane1->rotate(CVector<3,float>(0, 0, 0), -CMath<float>::PI()*0.5f);
            
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(3, 3, 3);
    NEW_BOX(box, green, 5.5, 10, 0);
    box->rotate(CVector<3, float>(1, 0, 0), CMath<float>::PI()*0.25f);
    box->rotate(CVector<3, float>(0, 0, 1), CMath<float>::PI()*0.25f);
}


void CScenes::setupScene25()
{
	setupWorldBoxPlanes();
	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(4,2,1);
	NEW_BOX(box5, green, 0.01f, -2, 2);
    box5->rotate(CVector<3, float>(1, 1, 1).getNormalized(), CMath<float>::PI()*0.35f);
	NEW_BOX(box6, red, 0, -2, -2);
    box6->rotate(CVector<3, float>(1, 1, 1).getNormalized(), CMath<float>::PI()*0.35f);
	box5_physics_object->addSpeed(CVector<3, float>(0, 0, -3));
}


void CScenes::setupScene26()
{
    setupWorldBoxPlanes();
	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox;
	box_factory->resizeBox(3, 3, 3);
	NEW_BOX(box5, green, 0, 3, 0);
    box5->rotate(CVector<3, float>(1, 0, 0).getNormalized(), CMath<float>::PI()*-0.25f);
    box5_physics_object->setInverseMass(0);
           
	NEW_BOX(box6, red, 0, 9, 0);
    box6->rotate(CVector<3, float>(0, 0, 1).getNormalized(), CMath<float>::PI()*-0.25f);			
    engine.physics.setGravitation(CVector<3, float> (0, -4, 0));
}


void CScenes::setupScene27()
{        
    scene_description = "2 Boxes connected by a spring";

    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4,4,4);
    
    NEW_BOX(box0, red, -4, 1, 0);
    NEW_BOX(box1, green, 4, 1, 0);

    NEW_SPRING_ANGULAR(box0, (CVector<3,float>(0,2,0)), box1, (CVector<3,float>(0,2,0)), 100.0f);
            
    engine.physics.setGravitation(CVector<3, float>());
}


void CScenes::setupScene28()
{        

    setupWorldBoxPlanes();
    
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(2, 2, 2);
    iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(1);
            
    NEW_BOX(box, red, 0, 10, 0);
    
    NEW_SPHERE(sphere1, pink, -1, 6, 0);
    NEW_SPHERE(sphere2, pink, 1, 4, 0);
    NEW_SPHERE(sphere3, pink, 0, 0, 0);
    
    sphere1_physics_object->setInverseMass(0);
    sphere2_physics_object->setInverseMass(0);
    sphere3_physics_object->setInverseMass(0);
            
    engine.physics.setGravitation(CVector <3, float> (0, -9.81f, 0));
}


void CScenes::setupScene29() 
{
    setupWorldBoxPlanes();
    
    iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(4, 4, 4);
    
    iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(10, 10);
    iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(0.1);
    
    NEW_BOX(box, red, 0, 4, 0);
    box_factory = new cObjectFactoryBox(10, 1, 10);
    NEW_BOX(floor, green, 0, -1, 0);
    floor_physics_object->setInverseMass(0);
    
    
    NEW_SPHERE(test, pink, 1.07201, -1.07802, -1.23486);
    test_physics_object->setInverseMass(0);
    
    
    box->rotate(CVector<3, float> (1, 0, 0), CMath<float>::PI()*0.1f);
    box->rotate(CVector<3, float> (0, 0, 1), CMath<float>::PI()*0.1f);

    
    engine.physics.setGravitation(CVector <3, float> (0, -9.81f, 0));
}

CScenes::CScenes(iEngine &p_engine)	:
		engine(p_engine)
{
	setupMaterials();
	scene_description = NULL;
}


CScenes::~CScenes()
{

}

void CScenes::setupScene(int scene_id)
{
	// setup scene
	switch(scene_id)
	{
	case 1:		setupScene1();		break;
	case 2:		setupScene2();		break;
	case 3:		setupScene3();		break;
	case 4:		setupScene4();		break;
	case 5:		setupScene5();		break;
	case 6:		setupScene6();		break;
	case 7:		setupScene7();		break;
	case 8:		setupScene8();		break;
	case 9:		setupScene9();		break;
	case 10:	setupScene10();		break;
	case 11:	setupScene11();		break;
	case 12:	setupScene12();		break;
	case 13:	setupScene13();		break;
	case 14:	setupScene14();		break;
	case 15:	setupScene15();		break;
	case 16:	setupScene16();		break;
	case 17:	setupScene17();		break;
	case 18:	setupScene18();		break;
	case 19:	setupScene19();		break;
	case 20:	setupScene20();		break;
	case 21:	setupScene21();		break;
	case 22:	setupScene22();		break;
    case 23:    setupScene23();     break;
    case 24:    setupScene24();     break;
    case 25:    setupScene25();     break;
    case 26:    setupScene26();     break;
    case 27:    setupScene27();     break;
    case 28:    setupScene28();     break;
    case 29:    setupScene29();     break;

	default:
		setupScene1();
	}
}
