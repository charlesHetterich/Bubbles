#include "Entities.h"

void Player::move()
{
	if (!landed)
	{
		currentAnim = JUMPING;

		//velocityY is positive
		if (velocityY >= 0.1f)
			anim[currentAnim].setCurrentFrame(0);
		else if (velocityY >= 0.0f)
			anim[currentAnim].setCurrentFrame(1);
		else if (velocityY >= -0.02f)
			anim[currentAnim].setCurrentFrame(2);
		//velocityY is negative
		else if (velocityY <= -0.1f)
			anim[currentAnim].setCurrentFrame(4);
		else if (velocityY <= -0.07f)
			anim[currentAnim].setCurrentFrame(4);
		else if (velocityY <= -0.02f)
			anim[currentAnim].setCurrentFrame(3);

		Mob::move();
	}
	else
	{
		planetLandedOn->movePlayer(this);
		updatePlanetAngle();
		planetLandedOn->interactWithAssets(this);
	}

	//set horizontal flip
	if (!landed || currentAnim == RUNNING)
	{
		if (velocityX < 0.0f)
			image.setHorizontalFlip(true);
		else if (velocityX > 0.0f)
			image.setHorizontalFlip(false);
	}

	if (currentAnim == RUNNING && (anim[currentAnim].getCurrentFrame() == 0  || anim[currentAnim].getCurrentFrame() == 2) && !stepTaken)
	{
		step[rand() % 2]->play();
		stepTaken = true;
	}
	else if (anim[RUNNING].getCurrentFrame() != 0 && anim[RUNNING].getCurrentFrame() != 2)
		stepTaken = false;
}

void Player::input(Input input)
{
	if (input.getKey(SDL_SCANCODE_A) || input.getGamePadAxis(GAMEPAD_LS_X) < -input.deadZone)
	{
		if (velocityX > -vXMax + vXAccel)
			velocityX -= vXAccel;
		else if (velocityX > - vXMax)
			velocityX = -vXMax;
	}
	else if (input.getKey(SDL_SCANCODE_D) || input.getGamePadAxis(GAMEPAD_LS_X) > input.deadZone)
	{
		if (velocityX < vXMax - vXAccel)
			velocityX += vXAccel;
		else if (velocityX < vXMax)
			velocityX = vXMax;
	}
	else if (landed)
	{
		if (velocityX < 0.0f)
			velocityX += vXAccel;
		else if (velocityX > 0.0f)
			velocityX -= vXAccel;

		if (abs(velocityX) <= vXAccel)
			velocityX = 0.0f;
	}

	if (landed)
	{
		if (velocityX < -vXMax)
			velocityX += vXAccel;
		else if (velocityX > vXMax)
			velocityX -= vXAccel;
	}
	

	if (landed && (input.getKeyDown(SDL_SCANCODE_SPACE) || input.getGamePadButtonDown(GAMEPAD_A)))
	{
		velocityX = cos((M_PI / 180) * planetLandedOn->getPlayerAngle()) * 5;
		velocityY = sin((M_PI / 180) * planetLandedOn->getPlayerAngle()) * 5;
		landed = false;


		planetLandedOn->dismount(velocityX, velocityY);
	}


	//reset
	if (input.getKeyDown(SDL_SCANCODE_R))
	{
		reset();
	}
}

Planet Player::comparePlanetLandedOn() { return *planetLandedOn; }

void Player::updatePlanetAngle()
{
	planetAngle = planetLandedOn->getPlayerAngle();
}

void Player::reset()
{
	image.setX(-600);
	image.setY(700);

	velocityX = 0;
	velocityY = 0;

	planetLandedOn->setLandedHere(false);
	landed = false;
}