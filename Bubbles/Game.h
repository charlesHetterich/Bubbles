#pragma once
#include "globalVariablesAndIncludes.h"
#include "Input.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Sprite.h"
#include "Entities.h"
#include "Sounds.h"

class Game
{
public:
	Game();
	~Game();

	void input(Input input);
	void update();
	void soundUpdate();
	void render();
	void inGameDraw();

private:

	Sounds* sound;


	Sprite screenSprite;
	Sprite frontGround;
	Animation frontGroundAnimation;

	Player player;

	std::vector<std::shared_ptr<Planet>> planets;
	std::vector<std::shared_ptr<Rope>> ropes;
	std::vector<std::shared_ptr<Asset>> assets;

	//---------------------------------------------------------------------------
	//CORE
	//---------------------------------------------------------------------------
	//shaders
	enum
	{
		BASIC_SHADER,
		BLUR_V_SHADER,
		BLUR_H_SHADER,

		NUM_SHADERS
	};
	Shader* shader[NUM_SHADERS];

	//textures
	enum
	{
		NO_TEXTURE,
		SPRITE_SHEET,

		NUM_TEXTURES
	};
	CTexture* texture[NUM_TEXTURES];
	RenderTexture screenTexture = RenderTexture((unsigned int)ACTUAL_SCREEN_WIDTH, (unsigned int)ACTUAL_SCREEN_HEIGHT);
	RenderTexture screenTexture_2 = RenderTexture((unsigned int)ACTUAL_SCREEN_WIDTH, (unsigned int)ACTUAL_SCREEN_HEIGHT);
	int currentTexure = 0;
};

