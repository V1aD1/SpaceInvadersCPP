#include "GridEntry.h"

GridEntry::GridEntry()
{
}

void GridEntry::Clear()
{
	Enemies.clear();
	Projectiles.clear();
	PlayerActor = nullptr;
}


GridEntry::~GridEntry()
{
}
