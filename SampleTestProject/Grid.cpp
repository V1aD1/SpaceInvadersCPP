#include <algorithm>
#include "Grid.h"

extern int ScreenSizeX;
extern int ScreenSizeY;

//every unit will encompass 1/100th of the screen
static const std::pair<int, int> unitFraction = std::pair<int, int>(10,10);

Grid::Grid()
{
	std::pair<int,int> unitSize = std::pair<int, int>(ScreenSizeX / unitFraction.first, ScreenSizeY / unitFraction.second);

	for (int i = 0; unitSize.first*i < ScreenSizeX; i++) {
		for (int j = 0; unitSize.second*j < ScreenSizeY; j++) {
			grid[std::pair<int, int>(i, j)] = new GridEntry();
		}
	}
}

std::list<GridEntry*> Grid::GetRelevantEntriesInGrid(MovingActor* actor)
{	

	std::pair<int, int> startGridPos = std::pair<int,int> (actor->GetPosition().first/ScreenSizeX * unitFraction.first, actor->GetPosition().second/ScreenSizeY * unitFraction.second);
	std::pair<int, int> endGridPos = std::pair<int, int>(((actor->GetPosition().first + actor->GetTexture()->Width())/ScreenSizeX * unitFraction.first) + 1,
														 ((actor->GetPosition().second + actor->GetTexture()->Height())/ScreenSizeY * unitFraction.second) + 1);


	startGridPos.first = std::max(0, startGridPos.first);
	startGridPos.first = std::min(unitFraction.first-1, startGridPos.first);

	startGridPos.second = std::max(0, startGridPos.second);
	startGridPos.second = std::min(unitFraction.first-1, startGridPos.second);

	endGridPos.first = std::max(0, endGridPos.first);
	endGridPos.first = std::min(unitFraction.first - 1, endGridPos.first);


	endGridPos.second = std::max(0, endGridPos.second);
	endGridPos.second = std::min(unitFraction.first - 1, endGridPos.second);
	
	std::list<GridEntry*> relevantEntries;
	
	for (int i = startGridPos.first; i <= endGridPos.first; i++) {
		for (int j = startGridPos.second; j <= endGridPos.second; j++) {
			relevantEntries.push_back(grid[std::pair<int, int>(i, j)]);
		}
	}

	return relevantEntries;
}

void Grid::AddEnemiesToGrid(std::map<int, MovingActor*> objectsToAdd)
{
	for each (auto object in objectsToAdd)
	{
		std::list<GridEntry*> entries = GetRelevantEntriesInGrid(object.second);
		for each (auto entry in entries)
		{
			entry->Enemies.push_back(object.second);
		}
	}
}

void Grid::AddProjectilesToGrid(std::map<int, MovingActor*> objectsToAdd)
{
	for each (auto object in objectsToAdd)
	{
		std::list<GridEntry*> entries = GetRelevantEntriesInGrid(object.second);
		for each (auto entry in entries)
		{
			entry->Projectiles.push_back(object.second);
		}
	}
}

void Grid::AddPlayerToGrid(MovingActor * playerObject)
{
		std::list<GridEntry*> entries = GetRelevantEntriesInGrid(playerObject);
		for each (auto entry in entries)
		{
			entry->PlayerActor = playerObject;
		}
}

void Grid::Clear()
{
	for each (auto entry in grid)
	{
		entry.second->Clear();
	}
}


Grid::~Grid()
{
}
