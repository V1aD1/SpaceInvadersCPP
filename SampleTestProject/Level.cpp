/*#include "Level.h"

extern int ScreenSizeX;
extern int ScreenSizeY;

Level::Level()
{
	EnemySpeed = 150;
	PlayerSpeed = 200;
	RocketSpeed = 200;
	NumEnemies = 20;

	grid = new Grid();
}

bool Level::UpdateEnemyDirectionIfNecessary(MovingActor* enemy) {

	if (EnemyDirection == std::pair<int, int>(1, 0)) {
		//at right edge of window so change direction to down
		if (enemy->GetPosition().first >= ScreenSizeX - enemy1Texture.Width()) {
			EnemyDirection = std::pair<int, int>(0, 1);
			return true;
		}
	}

	//left
	else if (EnemyDirection == std::pair<int, int>(-1, 0)) {
		//at left edge of window so change direction to down
		if (enemy->GetPosition().first <= 0) {
			EnemyDirection = std::pair<int, int>(0, 1);
			return true;
		}
	}

	//down
	else if (EnemyDirection == std::pair<int, int>(0, 1)) {
		if (enemy->GetPosition().first >= ScreenSizeX - enemy1Texture.Width()) {
			EnemySpeed += EnemySpeed*0.1;
			EnemyDirection = std::pair<int, int>(-1, 0);
			return true;
		}
		else if (enemy->GetPosition().first <= 0) {
			EnemySpeed += EnemySpeed*0.1;
			EnemyDirection = std::pair<int, int>(1, 0);
			return true;
		}
	}

	return false;

}

//this function should handle loading all the textures and
//all the enemies and their placements, according to a config file
void Level::OnInit(gfx::IWindow* theWindow)
{
	std::cout << "OnInit() called \n";

	//load sprites
	backGroundTexture.LoadFromFile(".\\Assets\\Background.png", 1);
	enemy1Texture.LoadFromFile(".\\Assets\\Invader01.png", 0.3f);
	enemy2Texture.LoadFromFile(".\\Assets\\Invader02.png", 0.3f);
	enemy3Texture.LoadFromFile(".\\Assets\\Invader03.png", 0.3f);
	bombTexture.LoadFromFile(".\\Assets\\invaderbomb.png", 0.3f);
	playerTexture.LoadFromFile(".\\Assets\\spaceship.png", 0.3f);
	rocketTexture.LoadFromFile(".\\Assets\\spaceshipLazer.png", 0.3f);

	window = theWindow;

	//create the enemies
	int xPos = ScreenSizeX / 20, yPos = ScreenSizeY / 10, numPerRow = 0;
	gfx::Texture* textureToUseForRow = &enemy1Texture;
	for (int i = 0; i < NumEnemies; i++) {
		enemies[i] = new Enemy(i, 100, 200, std::pair<int, int>(xPos, yPos), textureToUseForRow, &bombTexture, ActiveProjectiles);
		numPerRow++;
		xPos += enemy1Texture.Width() + ScreenSizeX / 20;
		if (xPos > ScreenSizeX - enemy1Texture.Width() || numPerRow > 5)
		{
			//every row will have a different texture than the previous row
			if (textureToUseForRow == &enemy1Texture) {
				textureToUseForRow = &enemy2Texture;
			}

			else if (textureToUseForRow == &enemy2Texture) {
				textureToUseForRow = &enemy3Texture;
			}

			else {
				textureToUseForRow = &enemy1Texture;
			}

			yPos += enemy1Texture.Height();
			xPos = ScreenSizeX / 20;
			numPerRow = 0;
		}
	}

	//creating the player
	player = new Player(3, PlayerSpeed, RocketSpeed, &playerTexture, &rocketTexture, theWindow->GetInputHandler(), ActiveProjectiles);

	Initialized = true;
}

void Level::OnRender()
{
	//drawing the background
	window->Draw(&backGroundTexture, 0, 0);

	//drawing the enemies
	for each (auto enemy in enemies)
	{
		window->Draw(enemy.second->GetTexture(), enemy.second->GetPosition().first, enemy.second->GetPosition().second);
	}

	//drawing the projectiles
	for each (auto projectile in ActiveProjectiles)
	{
		window->Draw(projectile.second->GetTexture(), projectile.second->GetPosition().first, projectile.second->GetPosition().second);
	}

	//drawing the player
	window->Draw(player->GetTexture(), player->GetPosition().first, player->GetPosition().second);

	std::stringstream stream;
	stream << "Health: " << player->GetHealth() << " Score: " << Score;
	//health and score
	window->Draw(stream.str().c_str(), 25, 0, 0);

}

std::map<int, MovingActor*> CastToMovingActor(std::map<int, Projectile*> objectsToCast) {
	std::map<int, MovingActor*> movingActorMap;

	std::map<int, Projectile*>::iterator it = objectsToCast.begin();
	for (it; it != objectsToCast.end(); ++it) {
		MovingActor* a = static_cast<MovingActor*>(&(*it->second));
		movingActorMap[a->GetId()] = a;
	}

	return movingActorMap;
}

std::map<int, MovingActor*> CastToMovingActor(std::map<int, Enemy*> objectsToCast) {
	std::map<int, MovingActor*> movingActorMap;

	std::map<int, Enemy*>::iterator it = objectsToCast.begin();
	for (it; it != objectsToCast.end(); ++it) {
		MovingActor* a = static_cast<MovingActor*>(&(*it->second));
		movingActorMap[a->GetId()] = a;
	}

	return movingActorMap;
}

void Level::OnUpdate(float delta)
{
	grid->Clear();

	grid->AddEnemiesToGrid(CastToMovingActor(enemies));
	grid->AddProjectilesToGrid(CastToMovingActor(ActiveProjectiles));
	grid->AddPlayerToGrid(player);

	//processing enemies
	for each (auto enemy in enemies)
	{
		enemy.second->Process(delta);
	}

	//processing projectiles
	for each (auto projectile in ActiveProjectiles)
	{
		projectile.second->Process(delta, grid);
	}

	//processing player
	player->Process(delta, grid);

	//deleting enemies that have died
	for (auto it = enemies.begin(); it != enemies.cend();)
	{
		MovingActor* enemy = it._Ptr->_Myval.second;

		if (enemy->IsAlive() == false) {
			delete it._Ptr->_Myval.second;
			it = enemies.erase(it);
		}

		else {
			++it;
		}
	}

	//deleting projectiles that have been destroyed
	for (auto it = ActiveProjectiles.begin(); it != ActiveProjectiles.cend();)
	{
		Projectile* projectile = it._Ptr->_Myval.second;

		if (projectile->IsAlive() == false) {
			delete it._Ptr->_Myval.second;
			it = ActiveProjectiles.erase(it);
		}

		else {
			++it;
		}
	}

	//moving enemies
	for each (auto enemy in enemies)
	{
		enemy.second->Move(delta);
	}

	//moving bombs
	for each (auto projectile in ActiveProjectiles)
	{
		projectile.second->Move(delta);
	}

	//moving player
	player->Move(delta);

	//determining enemy direction
	for each (auto enemy in enemies) {
		//UpdateEnemyDirectionIfNecessary() returns true if EnemyDirection was updated
		//this is done to ensure enemy direction changes only once a frame
		if (UpdateEnemyDirectionIfNecessary(enemy.second))
			break;
	}

	//updating enemy direction and speec
	for each (auto enemy in enemies) {
		//UpdateEnemyDirectionIfNecessary() returns true if EnemyDirection was updated
		//this is done to ensure enemy direction changes only once a frame
		enemy.second->SetDirection(EnemyDirection);
		enemy.second->SetSpeed(EnemySpeed);
	}
}

bool Level::IsInitialized()
{
	return Initialized;
}

Level::~Level()
{
}
*/