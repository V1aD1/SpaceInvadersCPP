#include "gfx.h"
#include "..\Level.h"
#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>


/**
 * This example is purely to meant to show how the API can be used to create a window and
 * render some textures to screen. Structures and organization of code are chosen to keep the
 * example short and not meant as a good starting point for a larger project (such as a Space
 * Invaders game).
 */

//global variables
int ScreenSizeX = 1000;
int ScreenSizeY = 720;
int NumEnemies = 20;
float EnemySpeed = 150;
float PlayerSpeed = 400;
float ProjectileSpeed = 200;
int ProjectileId = 0;
bool GameOver = false;
int HighScore = 0;

std::map<int, Projectile*> activeProjectiles;

int main()
{
	//reading the high score
	std::ifstream highScoreFile("highscore.txt");
	if (highScoreFile.is_open()) {
		std::string line;
		while (std::getline(highScoreFile, line)) {
			HighScore = std::stoi(line);
			break;
		}
	}
	highScoreFile.close();


	//reading the config file
	std::ifstream gameConfigFile("gameconfig.txt");
	if (gameConfigFile.is_open()) {
		std::string line;
		std::size_t found;
		while (std::getline(gameConfigFile, line)) {
			//ScreenSizeX found
			if ((found = line.find("resolution.x")) != std::string::npos) {
				if ((found = line.find(":")) != std::string::npos)
					ScreenSizeX = std::stoi(line.substr(found+1));
			}

			 else if ((found = line.find("resolution.y")) != std::string::npos) {
				 if ((found = line.find(":")) != std::string::npos)
					 ScreenSizeY = std::stoi(line.substr(found + 1));
			 }

			 else if ((found = line.find("enemy.speed")) != std::string::npos) {
				 if ((found = line.find(":")) != std::string::npos)
					 EnemySpeed = std::stoi(line.substr(found + 1));
			 }

			 else if ((found = line.find("player.speed")) != std::string::npos) {
				 if ((found = line.find(":")) != std::string::npos)
					 PlayerSpeed = std::stoi(line.substr(found + 1));
			 }

			 else if ((found = line.find("numEnemies")) != std::string::npos) {
				 if ((found = line.find(":")) != std::string::npos)
					 NumEnemies = std::stoi(line.substr(found + 1));
			 }

			 else if ((found = line.find("projectile.speed")) != std::string::npos) {
				 if ((found = line.find(":")) != std::string::npos)
					 ProjectileSpeed = std::stoi(line.substr(found + 1));
			 }
		}
	}
	gameConfigFile.close();

	Level level;

	gfx::IWindow* levelWindow = gfx::IWindow::CreateGameWindow(
		"LBC Invaders",
		ScreenSizeX,
		ScreenSizeY,
		std::bind(&Level::OnInit, &level, std::placeholders::_1),
		std::bind(&Level::OnRender, &level),
		std::bind(&Level::OnUpdate, &level, std::placeholders::_1)
	);

	//after window is called, OnInit is called, which among other things,
	//sets data.initialized to true 
	if (level.initialized)
	{
		//Update() calls OnInit once, then OnRender and OnUpdate forever
		while (levelWindow->Update() && level.GameOver == false)
		{
		}
	}

	if (level.Score > HighScore)
	{

		std::ofstream highScoreFile("highScore.txt");

		highScoreFile << level.Score << std::endl;

		highScoreFile.close();
	}


	levelWindow->Release();

	return 0;
}

