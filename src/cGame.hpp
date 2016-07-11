#ifndef _C_GAME_HPP__
#define _C_GAME_HPP__

#include <vector>
#include "sbndengine/iSbndEngine.hpp"
#include "include/gamemath.hpp"
#include "game/PathEnemy.hpp"

typedef CVector<3, gmfloat> vec3f;

class CGame
{
public:
	iEngine &engine;
	std::vector<iRef<iObject> > collectables;
	std::vector<iRef<iObject> > untouchables;
	std::vector<Enemy *> enemies;
	iRef<iPhysicsObject> trigger1;

	/**
	 * different kinds of materials
	 */
	struct
	{
		iRef<iGraphicsMaterial> white;
		iRef<iGraphicsMaterial> red;
		iRef<iGraphicsMaterial> yellow;
		iRef<iGraphicsMaterial> blue;
		iRef<iGraphicsMaterial> cyan;
		iRef<iGraphicsMaterial> green;
		iRef<iGraphicsMaterial> pink;
		iRef<iGraphicsMaterial> black;

		iRef<iGraphicsMaterial> white_noise;
		iRef<iGraphicsMaterial> red_noise;
		iRef<iGraphicsMaterial> yellow_noise;
		iRef<iGraphicsMaterial> blue_noise;
		iRef<iGraphicsMaterial> cyan_noise;
		iRef<iGraphicsMaterial> green_noise;
		iRef<iGraphicsMaterial> pink_noise;
		iRef<iGraphicsMaterial> black_noise;

		iRef<iGraphicsMaterial> grey_noise;
		iRef<iGraphicsMaterial> brown_noise;

		iRef<iGraphicsMaterial> boden_1;
		iRef<iGraphicsMaterial> wand_18;

		iRef<iGraphicsMaterial> player;
	} materials;


	void setupWorldBoxPlanes(float scale = 1.0);

	CGame(iEngine &io_engine);

	~CGame();

	void setupMaterials();

	void reset();

	void level0();
	void level1();
	void level2();
	void level3();

private:
	iRef<iPhysicsObject> addObjectToEngine(iObjectFactory::Type type, vec3f pos, vec3f size, vec3f rotation, iRef<iGraphicsMaterial> &material, float invMass = 0);
	iRef<iPhysicsObject> addBox(const vec3f &pos, const vec3f &size, const vec3f &rotation, iRef<iGraphicsMaterial> &material, float invMass = 0);
	iRef<iPhysicsObject> addPlane(const vec3f &pos, const vec2f &size, const vec3f &rotation, iRef<iGraphicsMaterial> &material, float invMass = 0);
	iRef<iPhysicsObject> addSphere(const vec3f &pos, float radius, iRef<iGraphicsMaterial> &material, float invMass = 0);
	void addRope(iRef<iPhysicsObject> o1, iRef<iPhysicsObject> o2);
	void addSpring(iRef<iPhysicsObject> o1, iRef<iPhysicsObject> o2);
};

#endif
