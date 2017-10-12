#pragma once

#ifndef ATTACKING_ACTOR_H
#define ATTACKING_ACTOR_H

#include <map>

#include "gfx.h"
#include "..\Projectile.h"

//An object that shoots projectiles
class AttackingActor
{
protected:
	float ProjectileSpeed;
	gfx::Texture*ProjectileTexture;
	std::map<int, Projectile*>* ActiveProjectiles;

public:
	AttackingActor(float projectileSpeed, gfx::Texture* projectileTexture, std::map<int, Projectile*>* activeProjectiles);

	//function should be overriden by the bomb and rocket classes
	virtual void FireProjectile() = 0;

	~AttackingActor();
};

#endif // !ATTACKING_ACTOR_H