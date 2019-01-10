#pragma once
#pragma comment(lib, "Winmm.lib") // NEEDED FOR SOUNDS

#include "ConsoleGameEngine.h"
#include "defines.h"

#include "RES.h"

#include "PacMan.h"
#include "Ghost.h"
#include "Map.h"
#include "Menu.h"
#include "Fruit.h"

class Game : public ConsoleGameEngine {
public:
	Game() { 
		//SETS THE CONSOLE TITLE
		sAppName = L"PacMan";
	}

	~Game() { 
		//DELETES TO ELIMENATE DATA LEEKS
		delete map;
		delete menu;
		delete res;
	}

	bool OnUserCreate();
	bool OnUserUpdate(float ElapsedTime);

	bool Play(float ElapsedTime);
	bool GameOver(float ElapsedTime);
	void StartPlaying(float ElpasedTime);
	void lvlUp();

	void GhostCollided(Ghost& ghost);
	void DrawMap(float ElapsedTime);

	void PlayUpdate(float ElapsedTime);
	void PlayDraw(float ElapsedTime);
	void Reset();

private:
	Fruit fruit = Fruit(this);
	//MAIN ENTITIES 
	PacMan pacman = PacMan(this, PACMAN_START_X, PACMAN_START_Y);
	RedGhost redGhost = RedGhost(this, L"ghosts/redGhost/redGhost", RED_START_X, RED_START_Y);
	PinkGhost pinkGhost = PinkGhost(this, L"ghosts/pinkGhost/pinkGhost", PINK_START_X, PINK_START_Y);
	BlueGhost blueGhost = BlueGhost(this, L"ghosts/blueGhost/blueGhost", PINK_START_X, PINK_START_Y);
	GreenGhost greenGhost = GreenGhost(this, L"ghosts/greenGhost/greenGhost", PINK_START_X, PINK_START_Y);
	//STORES IMPORTANT SPRITES
	RES* res = nullptr;
	//MAP
	Map* map = nullptr;
	//MENU DRAWING AND UPDATING
	Menu* menu = nullptr;
	//GAMESTATES
	GAMESTATE GameState = MENU;
};
