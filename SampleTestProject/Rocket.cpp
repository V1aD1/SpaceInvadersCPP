#include "Rocket.h"

extern int ScreenSizeX;
extern int ScreenSizeY;

Rocket::Rocket(int id, float speed, std::pair<float, float> startPosInPixels, gfx::Texture* texture) : Projectile(id, speed, std::pair<int, int>(0, -1), startPosInPixels, texture)
{
}

//this function will handle the border checking, and collision detection
void Rocket::Process(float delta, Grid* grid)
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
		for each (auto enemy in entry->Enemies)
		{
			//if rocket has been destroyed, finish processing
			if (Alive == false)
				return;

			//only check enemy if they are alive
			if (enemy->IsAlive()) {
				if (this->IsCollidingWith(enemy)) {
					//killing self
  					Alive = false;

					//notifying enemy of collision
					enemy->OnCollisionEnter();
				}
			}
		}
	}
}

Rocket::~Rocket()
{
}
