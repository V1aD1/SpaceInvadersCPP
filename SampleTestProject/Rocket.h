#pragma once

#ifndef ROCKET_H
#define ROCKET_H


#include "..\Projectile.h"

//Projectile that the Player shoots
class Rocket : public Projectile
{
public:
	Rocket(int id, float speed, std::pair<float, float> startPosInPixels, gfx::Texture* texture);
	virtual void Process(float delta, Grid* grid);
	~Rocket();
};
#endif // !ROCKET_H
