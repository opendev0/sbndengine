#ifndef _C_SCENES_HPP__
#define _C_SCENES_HPP__

#include "sbndengine/iSbndEngine.hpp"

class CScenes
{
public:
	iEngine &engine;

	const char *scene_description;

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
	} materials;


	void setupWorldBoxPlanes(float scale = 1.0);

	CScenes(iEngine &io_engine);

	void setupScene(int scene_id);

	~CScenes();

	void setupMaterials();

	void setupScene1();
	void setupScene2();
	void setupScene3();
	void setupScene4();
	void setupScene5();
	void setupScene6();
	void setupScene7();
	void setupScene8();
	void setupScene9();
	void setupScene10();
	void setupScene11();
	void setupScene12();
	void setupScene13();
	void setupScene14();
	void setupScene15();
	void setupScene16();
	void setupScene17();
	void setupScene18();
	void setupScene19();
	void setupScene20();
	void setupScene21();
	void setupScene22();
    void setupScene23();
    void setupScene24();
    void setupScene25();
    void setupScene26();
    void setupScene27();
    void setupScene28();
    void setupScene29();
};

#endif
