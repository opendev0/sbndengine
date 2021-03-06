#ifndef _C_GAME_HPP__
#define _C_GAME_HPP__

#include "sbndengine/iSbndEngine.hpp"

class CGame
{
public:
	iEngine &engine;


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

	void setupGameScene();
};

#endif
