#include "cGame.hpp"
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
void CGame::setupWorldBoxPlanes(float scale)
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


void CGame::setupGameScene() {
	setupWorldBoxPlanes();
	
	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(1.0);
	
	NEW_SPHERE(sphere, red, 0, 0, -3);
	
	sphere->setTouchable(false);
}


CGame::CGame(iEngine &p_engine)	:
		engine(p_engine)
{
	setupMaterials();
}


CGame::~CGame()
{

}