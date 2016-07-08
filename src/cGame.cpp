#include "cGame.hpp"

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
	texture->createTextureFromFile(TEXTURE_PATH "boden_12.jpg");
	materials.boden_1 = new iGraphicsMaterial;
	materials.boden_1->setTexture(texture);
	texture = new iTexture;
	texture->createTextureFromFile(TEXTURE_PATH "boden_1Normal.jpg");
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
	iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(20.f*scale, 20.f*scale);
	plane_factory->setInverseMass(0);

	// Setup floor
	NEW_PLANE(plane0, boden_1, 0, 0, 0);

	// Setup walls
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

void CGame::setupTestLevel()
{
	setupWorldBoxPlanes();

	iRef<cObjectFactorySphere> sphere_factory = new cObjectFactorySphere(0.5);
	iRef<cObjectFactoryBox> box_factory = new cObjectFactoryBox(1, 1, 1);
	iRef<cObjectFactoryPlane> plane_factory = new cObjectFactoryPlane(1.471, 1.275);

	NEW_SPHERE(sphere, red, 0, 0, -3);
	untouchables.push_back(sphere);

	NEW_SPHERE(sphere2, green, 3, 0, -3);
	collectables.push_back(sphere2);
	
	NEW_SPHERE(sphere3, blue, -3, 0, -3);
	PathEnemy *boxEnemy = new PathEnemy(sphere3_physics_object, engine);
	boxEnemy->addCheckpoint(CVector<3, float>(-3, -3, 3));
	boxEnemy->addCheckpoint(CVector<3, float>(0, -3, -3));
	boxEnemy->addCheckpoint(CVector<3, float>(0, -3, 0));
	enemies.push_back(boxEnemy);
	
	NEW_BOX(box, blue, -6, 0, -3);
	box_physics_object->setInverseMass(0);
	box->rotate(CVector<3, float> (0, 1, 1).getNormalized(), 0.4);
	enemies.push_back(new PathEnemy(box_physics_object, engine));
}

void CGame::level1() {
	const float h = CMath<float>::PI(); // (H)alf a rotation
	const float q = h / 2; // A (q)uarter of a rotation
	const vec3f nv; // Null vector

	// Setup room borders (wall, floor, ceiling)
	addPlane(vec3f(0, 0, 0), vec2f(30, 50), nv, materials.boden_1);
	addPlane(vec3f(0, 12, 0), vec2f(30, 50), vec3f(h, 0, 0), materials.boden_1);
	addPlane(vec3f(0, 9, 5), vec2f(30, 20), vec3f(q, 0, 0), materials.wand_18);
	addPlane(vec3f(0, 9, -20), vec2f(30, 20), vec3f(-q, 0, 0), materials.wand_18);
	addPlane(vec3f(-10, 5, 0), vec2f(50, 30), vec3f(0, q, q), materials.wand_18);
	addPlane(vec3f(10, 5, 0), vec2f(50, 30), vec3f(0, -q, -q), materials.wand_18);

	// SETUP OBSTACLES
	// Upper floor and stairs
	addBox(vec3f(0, 5, -10), vec3f(20, 0.2, 5), nv, materials.boden_1);
	addBox(vec3f(2, 1, -3), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(3, 2, -3), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(4, 3, -4), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(2, 3.5, -5), vec3f(1, 0.2, 1), nv, materials.white); // Collectable
	addBox(vec3f(2, 4.5, -2), vec3f(1, 0.2, 1), nv, materials.white);
	addBox(vec3f(4, 4, -6), vec3f(1, 0.2, 1), nv, materials.white);
	//addBox(vec3f(2, 4.5, -4), vec3f(1, 0.2, 1), nv, materials.white);

	// Pendulum
	{
		auto box1 = addBox(vec3f(0, 11.5, -10), vec3f(0.01, 0.01, 0.01), nv, materials.red);
		auto box2 = addSphere(vec3f(0, 6.1, -10), 0.8, materials.red, 1);
		addRope(box1, box2);
		auto length = (box1->object->position - box2->object->position).getLength();
		auto direction = vec3f(0, -1, -10).getNormalized();
		box2->object->position = box1->object->position + (direction * length);

		untouchables.push_back(box2->object);
	}

	// Triggered pendulum
	{
		auto box1 = addBox(vec3f(2, 11.5, -10), vec3f(0.01, 0.01, 0.01), nv, materials.red);
		auto box2 = addSphere(vec3f(2, 6.1, -10), 0.8, materials.red);
		addRope(box1, box2);
		auto length = (box1->object->position - box2->object->position).getLength();
		auto direction = vec3f(0, -1, -10).getNormalized();
		box2->object->position = box1->object->position + (direction * length);

		trigger1 = box2;
		untouchables.push_back(box2->object);
	}

	// Setup untouchables
	untouchables.push_back(addSphere(vec3f(0, 5.5, -10), 0.5, materials.red, 100)->object);
	untouchables.push_back(addSphere(vec3f(2, 5.5, -10), 0.5, materials.red, 10000)->object);

	// Setup enemies
	auto enemy1PObject = addSphere(vec3f(-10, 5.6, -7.5), 0.5, materials.blue);
	auto enemy1 = new PathEnemy(enemy1PObject, engine);
	enemy1->addCheckpoint(CVector<3, float>(-2, 5.6, -7.5));
	enemy1->addCheckpoint(CVector<3, float>(-2, 5.6, -12.5));
	enemies.push_back(enemy1);
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
	iRef<iPhysicsSoftConstraint> rope = new cPhysicsSoftConstraintSpring(o1, o2, o1->object->position.dist(o2->object->position), 50);
	o1->setDisableCollisionRotationAndFrictionFlag(true);
	o2->setDisableCollisionRotationAndFrictionFlag(true);
	engine.physics.addSoftConstraint(rope);
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