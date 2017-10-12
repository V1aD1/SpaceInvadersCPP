#pragma once
#ifndef MAINMENU_H 
#define MAINMENU_H

#include "gfx.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>


struct MainMenu
{
public:
	gfx::IWindow* window = nullptr;
	gfx::InputHandler* inputHandler;
	bool StartGame = false;
	bool initialized = false;
	int HighScore = 0;

	//this function should handle loading all the textures and
	//all the enemies and their placements, according to a config file
	void OnInit(gfx::IWindow* theWindow)
	{
		window = theWindow;
		inputHandler = window->GetInputHandler();

		std::ifstream highScoreFile("highscore.txt");

		if (highScoreFile.is_open()) {
			std::string line;
			while (std::getline(highScoreFile, line)) {
				HighScore = std::stoi(line);
			}
		}

		initialized = true;
	}

	void OnRender()
	{
		std::stringstream stream;
		stream << "Welcome to Vlad Baderca's Invaders Game!! The score to beat is: " << HighScore << "\n";
		stream << "Press the SPACE BAR to start the game!\n";
		window->Draw(stream.str().c_str(), 25, 0, 0);
	}

	void OnUpdate(float delta)
	{
		if (inputHandler->IsSpaceDown())
			StartGame = true;
	}

};

#endif // !MAINMENU_H
