#pragma once

#include <Windows.h>
#include <string>

//DEFINES USED IN THE WHOLE PROGRAM TO REDUCE REDUNDANCIES

#define w 28
#define h 31

#define PACMAN_START_X w / 2 - 0.5
#define PACMAN_START_Y h / 2 + 8

#define DEATH_TIME 20
#define SPEED 2

#define UPDATE_INTERVALS 2

#define RED_START_X w / 2 - 0.5
#define RED_START_Y h / 2 - 1
#define RED_SPEED 1.8
#define RED_SCATTER_POS 0, 0

#define PINK_START_X w / 2 - 2.5
#define PINK_START_Y h / 2 - 1
#define PINK_SPEED 1.6
#define PINK_SCATTER_POS w, 0

#define BLUE_START_X w / 2 - 0.5
#define BLUE_START_Y h / 2 - 1
#define BLUE_SPEED 1.5
#define BLUE_SCATTER_POS 0, h

#define GREEN_START_X w / 2 + 1.5
#define GREEN_START_Y h / 2 - 1
#define GREEN_SPEED 1.5
#define GREEN_SCATTER_POS w, h

//USED TO SCALE EVERYTHING INSIDE THE MAP TO THE COORECT SCREEN COORDS
#define toMap(x, y) (int)((x) * 8 + 2), (int)(((y) * 8) + 20)

//HELPER FUNKTION TO CLEAR TOE SCREEN
#define CLS Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK)
#define CLSc c->Fill(0, 0, c->ScreenWidth(), c->ScreenHeight(), PIXEL_SOLID, FG_BLACK)

//USED TO START A NEW SOUND
void PlayNewSound(const std::wstring& s, int async = 1);

//USED TO UPDATE ALL MOVING ENTITIES
enum DIREC {
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3,
	NONE = 4
};

//USED IN THE MAIN GAME LOOP TO STRUCTUR THE CODE
enum GAMESTATE {
	MENU,
	PAUSEMENU,
	START,
	RESET,
	LVLUP,
	HOWTO,
	PLAY,
	GAMEOVER,
	QUIT
};
