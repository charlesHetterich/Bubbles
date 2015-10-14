#pragma once
#include "globalVariablesAndIncludes.h"
#include "Sprite.h"
#include "Input.h"
#include "Sounds.h"

class Entity;
class Rope;
class Mob;
class Player;
class Planet;
class Bubble;

enum Planet_Animations
{
	GREEN_GRASS_PLANET,
	RED_GRASS_PLANET,
	BlACK_PLANET,
	BLACK_GEAR,

	NUM_PLANET_ANIMATIONS
};
static Animation planetAnimation[NUM_PLANET_ANIMATIONS] =
{
	Animation(5, 178, 64, 64),				//Green Grass Planet
	Animation(5, 246, 64, 64),				//Red Grass Planet
	Animation(5, 314, 64, 64),				//Black Planet
	Animation(5, 382, 64, 64),				//Black Gear
};

class Entity
{
public:
	Entity() : radius(0),
		planetAngle(0)
	{

	};
	
	virtual void draw() { image.draw(); }

	Sprite* getImage() { return &image; }

	float getRadius() { return radius; }
	void setRadius(float r) { radius = r; }\

	//dying
	virtual void reset() {}

	void setLandedHere(bool landed) { landedHere = landed; }
	bool getLandedHere() { return landedHere; }
	float getPlanetAngle() { return planetAngle; }
	virtual void updatePlanetAngle() {}

protected:

	WorldSprite image;
	float radius;

	float planetAngle;

	//flags
	bool landedHere;
};

class Asset : public Entity
{
public:
	Asset(float angle, std::shared_ptr<Entity> holder) : Entity(),
		holder(holder),
		anchoredAngle(angle)
	{
		planetAngle = angle;
	}

	virtual void move()
	{
		planetAngle = anchoredAngle + holder->getImage()->getRot();
		float x = holder->getImage()->getX() + holder->getImage()->getW() / 2 - image.getW() / 2 + cos((M_PI / 180) * planetAngle) * holder->getRadius();
		float y = holder->getImage()->getY() + holder->getImage()->getH() / 2 - image.getH() / 2 + sin((M_PI / 180) * planetAngle) * holder->getRadius();

		image.setX(x);
		image.setY(y);
	}
	virtual void collide(Entity* entity)
	{
		float holderR = holder->getRadius();

		float p1 = (M_PI * holderR * planetAngle) / 90.0f;
		float p2 = (M_PI * holderR * entity->getPlanetAngle()) / 90.0f;
		float c = 4 * M_PI * holderR;
		float d = abs(p1 - p2);

		if (d > c / 2.0f)
			d = c - d;

		if (d < radius + entity->getRadius())
			affectCollided(entity);
	}
	virtual void affectCollided(Entity* entity){}

protected:

	std::shared_ptr<Entity> holder;

	float anchoredAngle;

};

class Spike : public Asset
{
public:
	Spike(float angle, std::shared_ptr<Entity> holder, Shader* shader) : Asset(angle, holder)
	{
		float x = holder->getImage()->getX() + holder->getImage()->getW() / 2 - 10 + cos((M_PI / 180) * angle) * holder->getRadius();
		float y = holder->getImage()->getY() + holder->getImage()->getH() / 2 - 10 + sin((M_PI / 180) * angle) * holder->getRadius();
		
		anim = Animation(0, Animation::CANVAS_HEIGHT - 4, 1, 1, 1, 1000);
		image = WorldSprite(x, y, 20, 20, shader, &anim);
		image.setRot(angle + 45);
		radius = 8;
	}

	void move()
	{
		Asset::move();
		image.setRot(planetAngle + 45);
	}
	void affectCollided(Entity* entity)
	{
		entity->reset();
	}

private:

	Animation anim;

};

class Rope : public Entity
{
public:
	Rope(float x, float y, float length, float angle, Entity* holder, std::shared_ptr<Entity> entityHolding, Shader* shader) : Entity(),
		baseX(x),
		baseY(y),
		baseAngle(angle),
		maxStretch(2 * length),
		entityHolding(entityHolding)
	{
		image = WorldSprite(x, y - length, 4, length * 2, shader);
		image.setRot(angle);
		anim = Animation(0, Animation::CANVAS_HEIGHT - 2, 1, 2, 1, 1000);
	}
	Rope(){}

	void draw() { image.draw(&anim); }
	virtual void move(float d, float x, float y)
	{
		baseX = x;
		baseY = y;

		image.setH(maxStretch + d);
		image.setX(baseX);
		image.setY(baseY - image.getH() / 2);

		entityHolding->getImage()->setX(baseX - entityHolding->getImage()->getW() / 2 + cos((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2));
		entityHolding->getImage()->setY(baseY - entityHolding->getImage()->getH() / 2 + sin((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2));
	}

	float getMaxStretch() { return maxStretch; }
	float getBaseAngle() { return baseAngle; }

protected:

	Animation anim;
	float maxStretch;
	float baseX;
	float baseY;
	float baseAngle;

	std::shared_ptr<Entity> entityHolding;

};

class PullyRope : public Rope
{
public:
	PullyRope(float x, float y, float length, float angle, Entity* holder, bool inverted, std::shared_ptr<Entity> entityHolding, Shader* shader) : Rope(x, y, length, angle, holder, entityHolding, shader),
		inverted(inverted),
		holder(holder)
	{

	}

	void move(float d, float x, float y)
	{

		baseX = x;
		baseY = y;

		image.setX(baseX);
		image.setY(baseY - image.getH() / 2);

		if (inverted)
		{
			entityHolding->getImage()->setX(baseX - entityHolding->getImage()->getW() / 2 + cos((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2) - cos((M_PI / 180) * (baseAngle + 180)) * entityHolding->getRadius());
			entityHolding->getImage()->setY(baseY - entityHolding->getImage()->getH() / 2 + sin((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2) - sin((M_PI / 180) * (baseAngle + 180)) * entityHolding->getRadius());

			if (!entityHolding->getLandedHere())
				entityHolding->getImage()->setRot(-holder->getImage()->getRot());
			else
				holder->getImage()->setRot(-entityHolding->getImage()->getRot());
		}
		else
		{
			entityHolding->getImage()->setX(baseX - entityHolding->getImage()->getW() / 2 + cos((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2) - cos((M_PI / 180) * (baseAngle)) * entityHolding->getRadius());
			entityHolding->getImage()->setY(baseY - entityHolding->getImage()->getH() / 2 + sin((M_PI / 180) * (image.getRot() + 90)) * (image.getH() / 2) - sin((M_PI / 180) * (baseAngle)) * entityHolding->getRadius());
			
			if (!entityHolding->getLandedHere())
				entityHolding->getImage()->setRot(holder->getImage()->getRot());
			else
				holder->getImage()->setRot(entityHolding->getImage()->getRot());
		}
	}

private:

	std::shared_ptr<Entity> holder;
	bool inverted;
};

class Mob : public Entity
{
public:
	Mob() : Entity(),
		velocityX(0),
		velocityY(0),
		effectOfGravity(0),
		terminalVelocity(0)
	{

	};

	//statics
	static float gravity;

	//movement
	virtual void move()
	{
		velocityY += (gravity * effectOfGravity) * (velocityY - terminalVelocity);

		image.setX(image.getX() + velocityX);
		image.setY(image.getY() + velocityY);
	}
	inline float getVelocityX() { return velocityX; }
	inline void setVelocityX(float velocity) { velocityX = velocity; }
	inline float getVelocityY() { return velocityY; }
	inline void setVelocityY(float velocity) { velocityY = velocity; }

protected:

	float terminalVelocity;
	float effectOfGravity;
	float velocityX;
	float velocityY;
};

class Player : public Mob
{
public:
	Player(Shader* shader) : Mob(),
			landed(false),
			currentAnim(IDLE),
			vXMax(2.0f),
			vXAccel(0.08)
	{
		image = WorldSprite(-600, 700, 100, 100, shader);

		anim[IDLE] = Animation(0, 30, 15, 15, 1, 10);
		anim[RUNNING] = Animation(0, 0, 15, 15, 4, 100);
		anim[JUMPING] = Animation(0, 45, 15, 15, 6, 1000);
		anim[SLIDING] = Animation(0, 15, 15, 15, 2, 1000);

		step[0] = new Sounds(0, 600, "res/sounds/Randomize87.wav");
		step[1] = new Sounds(0, 600, "res/sounds/Randomize87.wav");

		effectOfGravity = 2;
		terminalVelocity = -8.0f;
		radius = 26.0f;
	}
	Player(){};

	void input(Input input);
	void move();
	void draw()
	{
		image.draw(&anim[currentAnim]);
		image.setRot(0.0f);
	}

	//player animation
	enum
	{
		IDLE,
		RUNNING,
		JUMPING,
		SLIDING,

		NUM_ANIMATIONS
	};
	void setCurrentAnimation(unsigned int animNum) { currentAnim = animNum; }
	unsigned int getCurrentAnimation(){ return currentAnim; }
	Animation* getAnimation(unsigned int animNum){ return &anim[animNum]; }

	//movment
	float getVXAccel() { return vXAccel; }

	void updatePlanetAngle();
	void setPlanetLandedOn(Planet* planet) { planetLandedOn = planet; }
	Planet* getPlanetLandedOn() { return planetLandedOn; }
	Planet comparePlanetLandedOn();

	//die
	void reset();

	//flags
	inline void toggleLanded() { landed = !landed; }
	inline bool getLanded() { return landed; }

private:

	//movement
	float vXMax;
	float vXAccel;

	//player sounds
	Sounds* step[2];
	bool stepTaken;

	//player animation
	Animation anim[NUM_ANIMATIONS];
	unsigned int currentAnim;

	Planet* planetLandedOn;

	//flags
	bool landed;
};

class Planet : public Mob
{
public:
	Planet(float x, float y, unsigned int animNumber, Shader* shader, std::vector<std::shared_ptr<Asset>> assets = std::vector<std::shared_ptr<Asset>>()) : Mob(),
		animNumber(animNumber),
		assets(assets)
	{
		image = WorldSprite(x, y, 400, 400, shader, &planetAnimation[animNumber]);
		radius = 200.0f;
	}
	Planet(){}

	//statics
	static float playerX;
	static float playerY;

	//drawing
	virtual void draw()
	{
		for (unsigned int i = 0; i < assets.size(); i++)
			assets[i]->draw();

		image.draw(&planetAnimation[animNumber]);
	}

	//assets
	void setAssets(std::vector<std::shared_ptr<Asset>> assets) { this->assets = assets; }
	void interactWithAssets(Entity* entity)
	{
		for (unsigned int i = 0; i < assets.size(); i++)
			assets[i]->collide(entity);
	}
	std::vector<std::shared_ptr<Asset>>* getAssets() { return &assets; }

	//player collision
	virtual void playerCollision(Player* player)
	{
		Sprite* pImage = player->getImage();
		Rect pRect = pImage->getRect();

		if (!player->getLanded())
		{
			if (circleCollision(image.getX() + image.getW() / 2, image.getY() + image.getH() / 2, radius, pRect.x + pRect.w / 2, pRect.y + pRect.h / 2, player->getRadius()))
			{
				//land player
				player->toggleLanded();
				player->setPlanetLandedOn(this);
				landedHere = true;

				float pVX = player->getVelocityX();
				float pVY = player->getVelocityY();
				float pSpeed = sqrt(pVX * pVX + pVY * pVY);

				float pTravelAngle = (180.0f / M_PI) * acosf(pVX / pSpeed);

				//fix angle
				if (pVY < 0)
					pTravelAngle = 360.0f - pTravelAngle;;

				player->setVelocityX(sin((playerAngle - pTravelAngle) * (M_PI / 180)) * pSpeed);
			}
		}
		else
		{
			if (circleCollision(image.getX() + image.getW() / 2, image.getY() + image.getH() / 2, radius, pRect.x + pRect.w / 2, pRect.y + pRect.h / 2, player->getRadius() - 5))
			{
				//land player
				player->getPlanetLandedOn()->setLandedHere(false);
				player->setPlanetLandedOn(this);
				landedHere = true;
				std::cout << "me";
			}
		}

	}
	virtual void movePlayer(Player* player)
	{
		playerAngle -= 360.0f / ((2 * M_PI * radius) / player->getVelocityX());

		int controlledAngle = (int)playerAngle % 360;

		//stop player
		player->setVelocityY(0.0f);
		player->getImage()->setRot(playerAngle - 90);
		player->setRadius(26.0f);

		//animation
		if (player->getVelocityX() == 0.0f)
			player->setCurrentAnimation(player->IDLE);
		else
		{
			player->setCurrentAnimation(player->RUNNING);
		}

		//set players pos
		player->getImage()->setX((image.getX() + image.getW() / 2 - player->getImage()->getW() / 2) + (cos((M_PI / 180) * playerAngle)) * (radius + player->getRadius()));
		player->getImage()->setY((image.getY() + image.getH() / 2 - player->getImage()->getH() / 2) + (sin((M_PI / 180) * playerAngle)) * (radius + player->getRadius()));
	}
	virtual void dismount(float vX, float vY){ landedHere = false; }
	void setPlayerAngle(float angle) { playerAngle = angle; }
	float getPlayerAngle() { return playerAngle; }
	void updatePlayerAngle()
	{
		//x & y distances
		float x = (playerX - (image.getX() + image.getW() / 2));
		float y = (playerY - (image.getY() + image.getH() / 2));

		//find distane
		playerDistance = sqrt(x * x + y * y);

		//find angle
		playerAngle = (180.0f / M_PI) * acosf(x / playerDistance);

		//fix angle
		if (y < 0)
			playerAngle = 360.0f - playerAngle;
	}

	virtual std::shared_ptr<Rope> getRope() { return nullptr; }

protected:

	float playerDistance = 0;
	float playerAngle = 0;

	std::vector<std::shared_ptr<Asset>> assets;

	unsigned int animNumber;
};

class Gear : public Planet
{
public:

	Gear(float x, float y, float maxRot, float ropeAngle, float ropeLength, std::shared_ptr<Entity> entityHolding, unsigned int animNumber, Shader* shader) : Planet(x, y, animNumber, shader),
		maxRot(maxRot)
	{
		image = WorldSprite(x, y, 400, 400, shader, &planetAnimation[animNumber]);
		rope.reset(new Rope(x + radius + cos((M_PI / 180) * ropeAngle) * (radius - 10), y + radius + sin((M_PI / 180) * ropeAngle) * (radius - 10), ropeLength, ropeAngle, this, entityHolding, shader));
	}

	virtual void movePlayer(Player* player)
	{
		float oldRot = image.getRot();
		image.setRot(oldRot + (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2);
		playerAngle -= (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2;

		if (maxRot > 0)
		{
			if (oldRot == image.getRot() && oldRot != 0)
			{
				image.setRot(image.getRot() - (360.0f / (2 * M_PI * radius)) / 2);
				playerAngle -= (360.0f / (2 * M_PI * radius)) / 2;
			}

			if (image.getRot() > maxRot)
			{
				image.setRot(maxRot);
				playerAngle -= (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2;
			}
			else if (image.getRot() < 0)
			{
				image.setRot(0);
				playerAngle -= (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2;
			}
		}
		else
		{
			if (oldRot == image.getRot() && oldRot != 0)
			{
				image.setRot(image.getRot() + (360.0f / (2 * M_PI * radius)) / 2);
				playerAngle += (360.0f / (2 * M_PI * radius)) / 2;
			}

			if (image.getRot() < maxRot)
			{
				image.setRot(maxRot);
				playerAngle -= (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2;
			}
			else if (image.getRot() > 0)
			{
				image.setRot(0);
				playerAngle -= (360.0f / ((2 * M_PI * radius) / player->getVelocityX())) / 2;
			}
		}


		int controlledAngle = (int)playerAngle % 360;

		//stop player
		player->setVelocityY(0.0f);
		player->getImage()->setRot(playerAngle - 90);
		player->setRadius(26.0f);

		//animation
		if (player->getVelocityX() == 0.0f)
			player->setCurrentAnimation(player->IDLE);
		else
		{
			player->setCurrentAnimation(player->RUNNING);
		}

		//set players pos
		player->getImage()->setX((image.getX() + image.getW() / 2 - player->getImage()->getW() / 2) + (cos((M_PI / 180) * playerAngle)) * (radius + player->getRadius()));
		player->getImage()->setY((image.getY() + image.getH() / 2 - player->getImage()->getW() / 2) + (sin((M_PI / 180) * playerAngle)) * (radius + player->getRadius()));
	}
	void move()
	{
		if (!landedHere)
		{
			if (maxRot > 0 && image.getRot() > 0)
			{
				image.setRot(image.getRot() - (360.0f / (2 * M_PI * radius)) / 2);
			}
			else if (image.getRot() < 0)
			{
				image.setRot(image.getRot() + (360.0f / (2 * M_PI * radius)) / 2);
			}
		}

		rope->move(2 * (image.getRot() * M_PI * radius) / 180, image.getX() + radius + cos((M_PI / 180) * rope->getBaseAngle()) * (radius - 10), image.getY() + radius + sin((M_PI / 180) * rope->getBaseAngle()) * (radius - 10));
	}
	void draw()
	{
		Planet::draw();
	}

	virtual std::shared_ptr<Rope> getRope() { return rope; }

protected:

	std::shared_ptr<Rope> rope;

	float maxRot;
	float minRot;

};

class Pully : public Gear
{
public:
	Pully(float x, float y, float maxRot, float ropeAngle, float ropeLength, bool inverted, std::shared_ptr<Entity> entityHolding, unsigned int animNumber, Shader* shader) : Gear(x, y, maxRot, ropeAngle, ropeLength, entityHolding, animNumber, shader)
	{
		rope.reset(new PullyRope(x + radius + cos((M_PI / 180) * ropeAngle) * (radius - 10), y + radius + sin((M_PI / 180) * ropeAngle) * (radius - 10), ropeLength, ropeAngle, this, inverted, entityHolding, shader));
	}

private:

};