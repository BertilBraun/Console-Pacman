#include "Game.h"

std::atomic<bool> ConsoleGameEngine::bAtomActive(false);
std::atomic<bool> ConsoleGameEngine::bSwitch(false);
std::condition_variable ConsoleGameEngine::cvGameFinished;
std::mutex ConsoleGameEngine::muxGame;

bool Game::OnUserCreate() { // INIT FUNCTION

	srand(time(NULL)); // SEED RANDOM WITH TIME

	menu = new Menu((ConsoleGameEngine*)this); // CREATE NEW MENU ON STACK
	//DRAWS AND UPDATES ALL THE MENUS
	map = new Map(L"map");// CREATE NEW MAP ON STACK
	//STORES A MAP OF THE POINTS TO COLLECT
	//STORES A TILE MAP FOR COLLISSION & MOVEMENT CHECKS
	res = new RES(this);// CREATE NEW RES ON STACK
	//STORES SPRITES FOR TEXT DRAWING AND OTHER USES

	if (map->corrupted) //LOADED MAP DIDNT EXCEED THE EXPECTATIONS
		return false; //QUIT

	return true; //CONTINUE TO MAIN GAME LOOP
}

bool Game::OnUserUpdate(float ElapsedTime) { //CALLED ONE TIME PER FRAME
	switch (GameState) { //GAMESTATE USED TO STRUCTURE GAMEUPDATES
	case MENU:
		menu->Update(ElapsedTime, GameState, *res, pacman, *map, fruit);//UPDATE  / DRAW MENU
		break;
	case PAUSEMENU:
		menu->Update(ElapsedTime, GameState, *res, pacman, *map, fruit);//UPDATE  / DRAW MENU
		break;
	case HOWTO:
		menu->DrawHowTo(*res, GameState); 
		//SIMPLY DRAWS HOWTO AND WAITS FOR SPACE TO GET BACK TO THE MENU
		break;
	case LVLUP:
		lvlUp(); //RESETS / LEVELS UP ALL ENTITIES / MAP
		break;
	case RESET:
		if (pacman.Health < 0)
			fruit.Revive(2);
		Reset(); //RESET ENTITIES & MAP
		GameState = START; //GO TO START TO CONTINUE
		PlayNewSound(L"start", 2); //PLAY NEW SOUND
		break;
	case START:
		StartPlaying(ElapsedTime); //START FUNCTION
		break;
	case PLAY:
		Play(ElapsedTime);//PLAY FUNCTION
		break;
	case GAMEOVER:
		GameOver(ElapsedTime);//GAMEOVER FUNCTION
		break;
	case QUIT:
		menu->highscore->SaveScore(pacman.Score + pacman.roundScore); // SAVE HIGHSCORE
		PlaySound(NULL, NULL, NULL);//STOP ALL MUSIC
		return false; //QUIT MAIN GAME LOOP
	}
	return true; //CONTINUE IN LOOP
}
