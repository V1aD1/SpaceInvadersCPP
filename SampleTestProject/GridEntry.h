#pragma once

#ifndef GRID_ENTRY_H
#define GRID_ENTRY_H

#include <list>
#include "..\MovingActor.h"

//Object that represents a "section" of the grid.
//The Enemies and Projectiles lists are checked by
// respective actors for collisions.
//For example, the rocket class only checks
//the Enemies list to determine if it has 
//collided with something meaningful
class GridEntry
{
public:

	std::list<MovingActor*> Enemies;
	std::list<MovingActor*> Projectiles;
	MovingActor* PlayerActor;

public:

	GridEntry();

	void Clear();

	~GridEntry();
};
#endif // !GRIDENTRY_H

