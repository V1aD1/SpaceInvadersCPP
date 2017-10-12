#include "Projectile.h"

extern int ScreenSizeX;
extern int ScreenSizeY;

Projectile::Projectile(int id, float speed, std::pair<int, int> direction, std::pair<float, float> startPos, gfx::Texture* texture) : MovingActor(id, speed, direction, startPos, texture)
{
}

void Projectile::OnCollisionEnter()
{
	Alive = false;
}

Projectile::~Projectile()
{
}
