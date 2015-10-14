#include "Game.h"

int Animation::CANVAS_WIDTH = 1024;
int Animation::CANVAS_HEIGHT = 1024;

ALCdevice* Sounds::device = nullptr;
ALCcontext* Sounds::context = nullptr;

double WorldSprite::cameraFocusX = 0.0;
double WorldSprite::cameraFocusY = 0.0;
float WorldSprite::mouseX = 0;
float WorldSprite::mouseY = 0;

float Mob::gravity = -0.002f;

float Planet::playerX = 0.0f;
float Planet::playerY = 0.0f;

unsigned int Shader::CURRENT_SHADER = -1;
float Shader::BLUR_STRENGTH = 1.0f;

Game::Game()
{
	Sounds::soundsInit();
	sound = new Sounds(0, 200, "res/sounds/trainloop.wav");
	sound->shouldLoop();
	//sound->play();

	shader[BASIC_SHADER] = new BasicShader();
	shader[BLUR_V_SHADER] = new Blur_V_Shader();
	shader[BLUR_H_SHADER] = new Blur_H_Shader();
	texture[SPRITE_SHEET] = new CTexture("bubblesprites.png");
	texture[SPRITE_SHEET]->bind(0);

	screenSprite = Sprite(0, 0, SCREEN_WIDTH * 2, -SCREEN_HEIGHT * 2, shader[BLUR_H_SHADER]);
	frontGround = Sprite(0, 0, SCREEN_WIDTH * 2, -SCREEN_HEIGHT * 2, shader[BASIC_SHADER]);
	frontGround.setAlpha(0.07f);
	frontGroundAnimation = Animation(0, 512, 600, 400, 1, 1000);

	player = Player(shader[BASIC_SHADER]);

	/*planets.push_back(std::shared_ptr<Planet>(new Planet(-200, 0, RED_GRASS_PLANET, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Planet(-700, 100, BlACK_PLANET, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Planet(400, 200, BlACK_PLANET, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Planet(-200, 500, BlACK_PLANET, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Planet(0, 0, BlACK_PLANET, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Gear(0, 1100, -360.0f, 180.0f, 1000, planets[4], BLACK_GEAR, shader[BASIC_SHADER])));
	planets.push_back(std::shared_ptr<Planet>(new Pully(0, 1100, 360.0f, 135.0f, -1000, true, planets[5], BLACK_GEAR, shader[BASIC_SHADER])));

	std::vector<std::shared_ptr<Asset>> assetList = {
		std::shared_ptr<Asset>(new Spike(0, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(10, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(20, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(30, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(40, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(50, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(60, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(70, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(80, planets[6], shader[BASIC_SHADER])),
		std::shared_ptr<Asset>(new Spike(0, planets[6], shader[BASIC_SHADER])),
	};
	planets[6]->setAssets(assetList);*/
	planets.push_back(std::shared_ptr<Planet>(new Planet(-200, 500, BlACK_PLANET, shader[BASIC_SHADER])));


	for (int i = 0; i < planets.size(); i++)
		if (planets[i]->getRope() != nullptr)
			ropes.push_back(std::shared_ptr<Rope>(planets[i]->getRope()));

	for (int i = 0; i < planets.size(); i++)
		if (planets[i]->getAssets() != nullptr)
			assets.insert(assets.end(), planets[i]->getAssets()->begin(), planets[i]->getAssets()->end());
}

Game::~Game()
{
	delete [] &shader;
	delete [] &texture;

	Sounds::soundsOver();
}

void Game::input(Input input)
{
	player.input(input);
	WorldSprite::setMouse(input);

	if (input.getMouseButtonDown(0))
	{
		planets.push_back(std::shared_ptr<Planet>(new Planet(WorldSprite::mouseX - 200, WorldSprite::mouseY - 200, BlACK_PLANET, shader[BASIC_SHADER])));
		std::cout << planets.size() << std::endl;
	}
}

void Game::update()
{
	for (unsigned int i = 0; i < planets.size(); i++)
		planets[i]->updatePlayerAngle();

	for (unsigned int i = 0; i < planets.size(); i++)
		planets[i]->move();

	for (unsigned int i = 0; i < assets.size(); i++)
		assets[i]->move();

	player.move();

	Planet::playerX = player.getImage()->getX() + (player.getImage()->getW() / 2);
	Planet::playerY = player.getImage()->getY() + (player.getImage()->getH() / 2);

	for (unsigned int i = 0; i < planets.size(); i++)
		planets[i]->playerCollision(&player);

	float oldFocusX = WorldSprite::cameraFocusX;
	float oldFocusY = WorldSprite::cameraFocusY;
	WorldSprite::cameraFocusY += ((player.getImage()->getY() * (player.getImage()->getY() > 0) - WorldSprite::cameraFocusY) / 200);
	WorldSprite::cameraFocusX += (player.getImage()->getX() *  (player.getImage()->getX() > 0) - WorldSprite::cameraFocusX) / 200;
	if (WorldSprite::cameraFocusX < 0)
		WorldSprite::cameraFocusX = 0;
	if (WorldSprite::cameraFocusY < 0)
		WorldSprite::cameraFocusY = 0;

	float cMoveX = WorldSprite::cameraFocusX - oldFocusX;
	float cMoveY = WorldSprite::cameraFocusY - oldFocusY;

	Shader::BLUR_STRENGTH = sqrt(cMoveX * cMoveX + cMoveY * cMoveY) / 20;
	if (Shader::BLUR_STRENGTH > 0.1)
		Shader::BLUR_STRENGTH = 0.1;

	soundUpdate();
}

void Game::soundUpdate()
{

	//WorldSounds::SOUND_FOCUS_X = player.getImage()->getX(); +player.getImage()->getW() / 2;
	//WorldSounds::SOUND_FOCUS_Y = player.getImage()->getY(); +player.getImage()->getH() / 2;
	WorldSounds::setListenerPos(player.getImage()->getX() + player.getImage()->getW() / 2, player.getImage()->getY() + player.getImage()->getH() / 2);
}

void Game::render()
{
	//bind texture for all sprites
	texture[SPRITE_SHEET]->bind(0);

	//bind specific fbo
	screenTexture.fbo_bind();

	//draw to fbo
	inGameDraw();
	frontGround.draw(&frontGroundAnimation);

	//unbind specific fbo
	screenTexture.fbo_unBind();




	//bind specific fbo
	screenTexture_2.fbo_bind();

	screenTexture.bind(0);
	screenSprite.draw();

	//unbind specific fbo
	screenTexture_2.fbo_unBind();

	texture[SPRITE_SHEET]->bind(0);

	screenTexture_2.bind(0);
	screenSprite.draw(shader[BLUR_V_SHADER]);
}

void Game::inGameDraw()
{
	for (int i = 0; i < ropes.size(); i++)
		ropes[i]->draw();

	for (unsigned int i = 0; i < planets.size(); i++)
		planets[i]->draw();

	player.draw();
}