#pragma once

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "..\Grid.h"
#include "..\MovingActor.h"

//Base class for all projectiles (rockets and bombs)
class Projectile : public MovingActor
{
public:
	Projectile(int id, float speed, std::pair<int,int> direction, std::pair<float, float> startPosInPixels, gfx::Texture* texture);
	virtual void OnCollisionEnter();
	virtual void Process(float delta, Grid* grid) = 0;

	~Projectile();
};
#endif // !PROJECTILE_H
