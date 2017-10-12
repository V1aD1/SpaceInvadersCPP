#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <list>
#include <map>
#include "gfx.h"
#include "..\Bomb.h"
#include "..\AttackingActor.h"
#include "..\MovingActor.h"

//The Enemy class
class Enemy : virtual public AttackingActor, virtual public MovingActor
{

public:
	Enemy(int id, float speed, float bombSpeed, std::pair<float, float> position, gfx::Texture* enemyTexture, gfx::Texture* bombTexture, std::map<int, Projectile*>* activeProjectiles);
	void Move(float delta);
	
	virtual void FireProjectile();
	virtual void Process(float delta);
	virtual void OnCollisionEnter();

	~Enemy();
};
#endif // !ENEMY_H

