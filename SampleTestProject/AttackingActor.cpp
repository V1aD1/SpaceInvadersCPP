#include "AttackingActor.h"

AttackingActor::AttackingActor(float projectileSpeed, gfx::Texture* projectileTexture, std::map<int, Projectile*>* activeProjectiles)
{
	ProjectileSpeed = projectileSpeed;
	ProjectileTexture = projectileTexture;
	ActiveProjectiles = activeProjectiles;
}

AttackingActor::~AttackingActor()
{
}
