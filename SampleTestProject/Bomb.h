#pragma once

#ifndef BOMB_H
#define BOMB_H


#include "..\Projectile.h"

//The projectile that theenemies shoot
class Bomb : public Projectile
{
public:
	Bomb(int id, float speed, std::pair<float, float> startPosInPixels, gfx::Texture* texture);

	virtual void Process(float delta, Grid* grid);

	~Bomb();
};

#endif // !BOMB_H
