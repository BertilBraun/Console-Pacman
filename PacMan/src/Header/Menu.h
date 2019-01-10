#pragma once

#include "ConsoleGameEngine.h"
#include "defines.h"

#include "PacMan.h"
#include "Map.h"
#include "Highscore.h"
#include "RES.h"
#include "Animation.h"
#include "Entity.h"
#include "Fruit.h"


class Menu {
public:
	Menu(ConsoleGameEngine* c);
	~Menu();

	bool Update(float ElapsedTime, GAMESTATE& s, RES& res, PacMan& pacman, Map& map, Fruit& fruit);
	void UpdateAnim(float ElapsedTime, RES& res);
	bool Draw(float ElapsedTime, GAMESTATE& gameState, RES& res, PacMan& pacman, Map& map, Fruit& fruit);
	void DrawHowTo(RES& res, GAMESTATE& gameState);

	ConsoleGameEngine* c = nullptr;
	Entity bigPacman = Entity(c, L"pacman/Big/pacmanBig", c->ScreenWidth(), 0);
	Highscore* highscore = nullptr;
	float Timer = 0;

	Entity ghosts[5]{	Entity(c, L"ghosts/redGhost/redGhost", c->ScreenWidth() + 15, 0),
						Entity(c, L"ghosts/pinkGhost/pinkGhost", c->ScreenWidth() + 30, 0),
						Entity(c, L"ghosts/blueGhost/blueGhost", c->ScreenWidth() + 45, 0),
						Entity(c, L"ghosts/greenGhost/greenGhost", c->ScreenWidth() + 60, 0),
						Entity(c, L"ghosts/fearedGhost/fearedGhost", 65, 0) };
};

