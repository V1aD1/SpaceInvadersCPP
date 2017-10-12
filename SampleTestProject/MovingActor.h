#pragma once

#ifndef MOVING_ACTOR_H
#define MOVING_ACTOR_H

#include <iostream>
#include <list>
#include "gfx.h"

//Object that moves, and therefore has a speed, direction and position etc.
class MovingActor
{
protected:
	int Id;
	float Speed;
	std::pair<int, int> Direction;
	std::pair<float, float> Position;
	std::list<std::pair<int, int>> RelevantEntriesInGrid;
	gfx::Texture* ActorTexture;
	bool Alive;

public:
	MovingActor(int id, float speed, std::pair<int, int> direction, std::pair<float, float> position, gfx::Texture* texture);

	virtual void Move(float delta);
	virtual void OnCollisionEnter() = 0;

	bool IsCollidingWith(MovingActor* actor);

	int GetId();
	float GetSpeed();
	bool IsAlive();
	std::pair<float, float> GetPosition();
	std::pair<int, int> GetDirection();
	gfx::Texture* GetTexture();

	void SetDirection(std::pair<int, int> newDir);
	void SetSpeed(float newSpeed);

	~MovingActor();
};

#endif // !MOVING_ACTOR_H