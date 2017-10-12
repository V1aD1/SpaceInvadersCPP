#pragma once
#ifndef LEVEL_H 
#define LEVEL_H

#include "gfx.h"
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "..\Enemy.h"
#include "..\Player.h"

extern int ScreenSizeX;
extern int ScreenSizeY;
extern int HighScore;
extern int NumEnemies;
extern float EnemySpeed;
extern float PlayerSpeed;
extern float ProjectileSpeed;


//ugly,but necessary for downcasting
std::map<int, MovingActor*> CastToMovingActor(std::map<int, Projectile*> objectsToCast) {
	std::map<int, MovingActor*> movingActorMap;

	std::map<int, Projectile*>::iterator it = objectsToCast.begin();
	for (it; it != objectsToCast.end(); ++it) {
		MovingActor* a = static_cast<MovingActor*>(&(*it->second));
		movingActorMap[a->GetId()] = a;
	}

	return movingActorMap;
}

//ugly,but necessary for downcasting
std::map<int, MovingActor*> CastToMovingActor(std::map<int, Enemy*> objectsToCast) {
	std::map<int, MovingActor*> movingActorMap;

	std::map<int, Enemy*>::iterator it = objectsToCast.begin();
	for (it; it != objectsToCast.end(); ++it) {
		MovingActor* a = static_cast<MovingActor*>(&(*it->second));
		movingActorMap[a->GetId()] = a;
	}

	return movingActorMap;
}

//The object that handles the world space etc
struct Level
{
public:
	bool initialized = false;
	float LocalEnemySpeed;
	int Score = 0;
	bool GameOver = false;

	std::map<int, Enemy*> ActiveEnemies;
	std::pair<int, int> OrigEnemyDirection = std::pair<int, int>(1, 0);
	std::pair<int, int> EnemyDirection;
	std::map<int, Projectile*> ActiveProjectiles;


	gfx::IWindow* window = nullptr;
	gfx::Texture enemy1Texture;
	gfx::Texture enemy2Texture;
	gfx::Texture enemy3Texture;
	gfx::Texture backGroundTexture;
	gfx::Texture bombTexture;
	gfx::Texture playerTexture;
	gfx::Texture rocketTexture;

	Player* player;
	Grid* grid = new Grid();

	//this function should handle loading all the textures and
	//all the enemies and their placements, according to a config file
	void OnInit(gfx::IWindow* theWindow)
	{
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
		CreateEnemies();

		//creating the player
		player = new Player(3, PlayerSpeed, ProjectileSpeed, &playerTexture, &rocketTexture, window->GetInputHandler(), &ActiveProjectiles);

		initialized = true;
	}

	void OnRender()
	{
		//drawing the background
		window->Draw(&backGroundTexture, 0, 0);

		//drawing the enemies
		for each (auto enemy in ActiveEnemies)
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

		//drawing health, score and highscore
		std::stringstream stream;
		stream << "Health: " << player->GetHealth() << " Score: " << Score << " Highscore: "<< std::max(Score, HighScore);
		window->Draw(stream.str().c_str(), 25, 0, 0);

	}

	void OnUpdate(float delta)
	{
		//the player has killed all enemies, so respawn them again
		if (ActiveEnemies.size() == 0)
			CreateEnemies();

		//used to determine score
		int prevActiveEnemies = ActiveEnemies.size();

		//clear the grid every frame (inneficient)
		grid->Clear();

		//add all enemies to the grid
		//wanted this to be done in the Process(...) function
		//of each object, but I ran into a cyclical reference issue
		//that I couldn't figure out...
		grid->AddEnemiesToGrid(CastToMovingActor(ActiveEnemies));
		grid->AddProjectilesToGrid(CastToMovingActor(ActiveProjectiles));
		grid->AddPlayerToGrid(player);

		//processing enemies
		for each (auto enemy in ActiveEnemies)
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
		for (auto it = ActiveEnemies.begin(); it != ActiveEnemies.cend();)
		{
			MovingActor* enemy = it._Ptr->_Myval.second;

			if (enemy->IsAlive() == false) {
				delete it._Ptr->_Myval.second;
				it = ActiveEnemies.erase(it);
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
		for each (auto enemy in ActiveEnemies)
		{
			enemy.second->Move(delta);
		}

		//moving projectiles
		for each (auto projectile in ActiveProjectiles)
		{
			projectile.second->Move(delta);
		}

		//moving player
		player->Move(delta);

		//determining enemy direction
		for each (auto enemy in ActiveEnemies) {
			//UpdateEnemyDirectionIfNecessary() returns true if EnemyDirection was updated
			//this is done to ensure enemy direction changes only once a frame
			if (UpdateEnemyDirectionIfNecessary(enemy.second))
				break;
		}

		//updating enemy direction and speed
		for each (auto enemy in ActiveEnemies) {
			//UpdateEnemyDirectionIfNecessary() returns true if EnemyDirection was updated
			//this is done to ensure enemy direction changes only once a frame
			enemy.second->SetDirection(EnemyDirection);
			enemy.second->SetSpeed(LocalEnemySpeed);
		}

		//updating the score
		//score increases everytime player kills an enemy
		Score += prevActiveEnemies - ActiveEnemies.size();

		if (player->IsAlive() == false)
			GameOver = true;
	}

	bool UpdateEnemyDirectionIfNecessary(MovingActor* enemy) {

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
				LocalEnemySpeed += LocalEnemySpeed*0.1;
				EnemyDirection = std::pair<int, int>(-1, 0);
				return true;
			}
			else if (enemy->GetPosition().first <= 0) {
				LocalEnemySpeed += LocalEnemySpeed*0.1;
				EnemyDirection = std::pair<int, int>(1, 0);
				return true;
			}
		}

		return false;

	}

	void CreateEnemies() {
		LocalEnemySpeed = EnemySpeed;
		EnemyDirection = OrigEnemyDirection;


		int xPos = ScreenSizeX / 20, yPos = ScreenSizeY / 10, numPerRow = 0;
		gfx::Texture* textureToUseForRow = &enemy1Texture;


		for (int i = 0; i < NumEnemies; i++) {
			ActiveEnemies[i] = new Enemy(i, LocalEnemySpeed, ProjectileSpeed, std::pair<int, int>(xPos, yPos), textureToUseForRow, &bombTexture, &ActiveProjectiles);
			numPerRow++;
			xPos += enemy1Texture.Width() + ScreenSizeX / 20;

			if (xPos > ScreenSizeX - enemy1Texture.Width() || numPerRow > 4)
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
	}
};

#endif // !LEVEL_H
