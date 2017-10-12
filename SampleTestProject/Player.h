#pragma once

#ifndef PLAYER_H
#define PLAYER_H


#include <iostream>
#include <map>
#include "gfx.h"

#include "..\Rocket.h"
#include "..\AttackingActor.h"
#include "..\MovingActor.h"

//The player object
class Player : virtual public AttackingActor, virtual public MovingActor
{
private:
	int Health;
	gfx::InputHandler* InputHandler;

public:
	Player(int health, 
		    float speed, 
			float rocketSpeed, 
			gfx::Texture* playerTexture, 
			gfx::Texture* rocketTexture, 
			gfx::InputHandler* inputHandler,
			std::map<int, Projectile*>* activeProjectiles);

	virtual void FireProjectile();
	void Process(float delta, Grid* grid);
	virtual void OnCollisionEnter();

	void Move(float delta);
	void TakeDamage(int damage);

	int GetHealth();
	~Player();
};
#endif // !PLAYER_H
