#pragma once

#ifndef GRID_H
#define GRID_H

#include <map>

#include "gfx.h"
#include "..\GridEntry.h"

class MovingActor;

extern int rowSize;
extern int colSize;

//Grid object which greatly simplifies collision.
//No matter the resolution, the world space is divided
//into 100 sections, and an object only has to check 
//it's local sections for overlapping objects
class Grid
{
private:
	std::map<std::pair<int, int>, GridEntry*> grid;
	
public:
	Grid();

	std::list<GridEntry*> GetRelevantEntriesInGrid(MovingActor* object);
	void AddEnemiesToGrid(std::map<int, MovingActor*> objectsToAdd);
	void AddProjectilesToGrid(std::map<int, MovingActor*> objectsToAdd);
	void AddPlayerToGrid(MovingActor* playerObject);
	void Clear();

	~Grid();
};

#endif // !GRID_H