#include "Enemy.h"

extern int ScreenSizeX;
extern int ScreenSizeY;
extern int ProjectileId;
extern bool GameOver;

static const float minFrameTime = 0.03f;
static float timePassedSinceLastBombDropAttempt = 0;

//all enemies start by heading to the right of the screen
Enemy::Enemy(int id, float speed, float bombSpeed, std::pair<float, float> position, gfx::Texture* enemyTexture, gfx::Texture* bombTexture, std::map<int, Projectile*>* activeProjectiles):
	AttackingActor(bombSpeed, bombTexture, activeProjectiles), MovingActor(id, speed, std::pair<int,int> (1,0), position, enemyTexture)
{
}
//delta parameter used to ensure consistent behaviour regardless of frame rate
void Enemy::Move(float delta)
{
	Position.first += delta * Speed * Direction.first;
	Position.second += ActorTexture->Height() * Direction.second;
}

//this function will handle the border checking, collision detection, direction change, bomb dropping
void Enemy::Process(float delta)
{
	//border checking
	//only destroy enemy is they are below the screen,
	//if they are to the edge then they will go down a step then continue in the
	//opposite direction
	if (Position.second > ScreenSizeY)
	{
		Alive = false;
		return;
	}

	//an enemy reached the bottom of the screen, so trigger the game over state
	if (Position.second > ScreenSizeY - ActorTexture->Height())
		GameOver = true;

	//only attempt to drop bombs max 30 times per second
	//this is done so that users with a higher fps do not
	//have more bombs dropped on them on average
	if (timePassedSinceLastBombDropAttempt > minFrameTime) {
		//from 1 to 1000
		int chanceOfDroppingBomb = rand() % 1000 + 1;

		//0.2% chance of dropping bomb
		if (chanceOfDroppingBomb >= 998) {
			FireProjectile();
		}

		timePassedSinceLastBombDropAttempt = 0;
	}

	timePassedSinceLastBombDropAttempt += delta;
}

//enemy dies when it's hit by a rocket
void Enemy::OnCollisionEnter()
{
	Alive = false;
}

void Enemy::FireProjectile() {
	(*ActiveProjectiles)[ProjectileId] = new Bomb(ProjectileId, ProjectileSpeed, std::pair<int, int>(Position.first, Position.second + ActorTexture->Height()), ProjectileTexture);
	ProjectileId++;
}

Enemy::~Enemy()
{
	std::cout << "Destructor for enemy: " << Id << " called \n";
}
