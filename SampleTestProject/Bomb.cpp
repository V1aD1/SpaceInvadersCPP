#include "Bomb.h"

extern int ScreenSizeX;
extern int ScreenSizeY;

Bomb::Bomb(int id, float speed, std::pair<float, float> startPos, gfx::Texture* texture): Projectile(id, speed, std::pair<int, int>(0, 1), startPos, texture)
{
}

//this function will handle the border checking, and collision detection
void Bomb::Process(float delta, Grid* grid)
{
	//border checking
	//only destroy enemy is they are below the screen,
	//if they are to the edge then they will go down a step then continue in the
	//opposite direction
	if (Position.second > ScreenSizeY)
	{
		Alive = false;
	}

	std::list<GridEntry*> relevantEntries = grid->GetRelevantEntriesInGrid(this);

	for each (auto entry in relevantEntries)
	{
		//if bomb has been destroyed, finish processing
		if (Alive == false)
			return;

		//if this is not null, then it means the player is contained in this grid unit
		if(entry->PlayerActor != nullptr)
		{
			if (this->IsCollidingWith(entry->PlayerActor)) {
				//killing self
				Alive = false;

				//notifying player of collision
				entry->PlayerActor->OnCollisionEnter();
			}
		}
	}
}

Bomb::~Bomb()
{
}
