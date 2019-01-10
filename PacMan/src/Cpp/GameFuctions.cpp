#include "Game.h"

void Game::GhostCollided(Ghost& ghost) {
	if (ghost.state == FRIGHTEND) {
		ghost.InBoxTimer = 0;
		ghost.GTimer = 0;
		ghost.Dead = true;
		pacman.GhostScore *= 2;
		pacman.roundScore += pacman.GhostScore;
		PlayNewSound(L"eating", 2);
	}
	else if (ghost.state != RETURNING && !pacman.iterated) {
		pacman.Dead = true;
		PlayNewSound(L"death", 2);
	}
}

void Game::DrawMap(float ElapsedTime) {
	CLS; //CLEAR THE SCREEN

	DrawSprite(toMap(0, 0), map->mapSprite); //DRAW THE MAP SPRITE

	for (auto &p : map->points) // LOOP THROUGH ALL POINTS
		DrawSprite(toMap(p.first.x, p.first.y), p.second.sprite); // DRAW ALL POINTS

	pacman.DrawSelf(ElapsedTime); // DRAW PACMAN / DEATH ANIMATION IF HES DEAD

	//DRAW THE FRUITS (LOWER LEFT CORNER)
	res->drawText(L"FRUITS", 5, ScreenHeight() - 12);
	fruit.DrawSelf();

	//DRAW THE CURRENT LVL (TOP RIGHT)
	res->drawText(std::to_wstring(pacman.lvl + 1) + L"UP", 20, 4);

	//DRAW THE CURRENT SCORE
	res->drawText(L"SCORE " + std::to_wstring(pacman.roundScore), ScreenWidth() / 2 - 20, 4);

	//DRAW FOR EVERY LIVE LEFT ONE PACMAN (LOWER RIGHT CORNER
	res->drawText(L"LIVES", ScreenWidth() - 80, ScreenHeight() - 12);
	for (int i = 0; i < pacman.Health; i++)
		DrawSprite(ScreenWidth() - 45 + (i * 13), ScreenHeight() - 15, pacman.anim[1].sprite[0]);

}

void Game::Reset() {
	//RESETS THE POSITION AND STATES FOR EVERY MOVEABLE ENTITY
	pacman.Revive();
	redGhost.Revive();
	pinkGhost.Revive();
	blueGhost.Revive();
	greenGhost.Revive();
}

void Game::lvlUp() {
	//PLAYS THE LVLUP SOUND AND LETS IT PLAY FOR 1 / 3 OF A SECOND
	PlayNewSound(L"lvlUp");
	Sleep(300);
	//INCREASES THE LVL USED TO CALCULATE SPEEDS ETC
	pacman.lvl++;
	//RESETS THE EATEN POINTS
	map->points = map->allPoints;
	//INCREASES THE OVERALL SCORE AND RESETS THE LVL SCORE
	pacman.Score += pacman.roundScore;
	pacman.roundScore = 0;
	//GOES TO RESET THE OTHER ENTITIES
	GameState = RESET;
}

void Game::StartPlaying(float ElapsedTime) {
	//DRAWS MAP POINTS FRUITS ADD INFORMATION AND PACMAN
	DrawMap(ElapsedTime);
	//DRAWS THE READY SIGN SIGNALING THE START STATE
	DrawSprite(ScreenWidth() / 2 - 22, 157, res->GetSprite(L"Ready"));

	//WITH ESCAPE YOU ARE ABLE TO GO TO THE MENU
	if (GetKey(VK_ESCAPE).Pressed) {
		menu->Timer = 0;
		menu->bigPacman.x = ScreenWidth();
		menu->bigPacman.dir = NONE;
		for (int i = 1; i < 5; i++)
			menu->ghosts[i - 1].x = ScreenWidth() + i * 15;
		menu->ghosts[4].x = 65;
		GameState = PAUSEMENU;
		PlayNewSound(L"pause"); //STARTS NEW SOUND "PAUSE" IN THE BACKGROUND
	}
	//PRESSING ANY USED KEYS STARTS THE PLAYING STATE
	if (GetKey(VK_SPACE).Pressed || GetKey(VK_UP).Pressed ||
		GetKey(VK_DOWN).Pressed || GetKey(VK_RIGHT).Pressed ||
		GetKey(VK_LEFT).Pressed || GetKey(L'W').Pressed ||
		GetKey(L'S').Pressed || GetKey(L'D').Pressed ||
		GetKey(L'A').Pressed) {
		GameState = PLAY; //SETS STATE TO PLAY
		PlayNewSound(L"play"); // STARTS NEW SOUND "PLAY" IN THE BACKGROUND
	}
}

bool Game::Play(float ElapsedTime) {

	std::thread UpdateThread(&Game::PlayUpdate, this, ElapsedTime); // CREATES A NEW THREAD SINCE THESE TWO OPERATIONS ARE THE MOSE COPUTING HEAVY
	std::thread DrawThread(&Game::PlayDraw, this, ElapsedTime); //IN THE WHOLE PROGRAM LETS THEM COMPUTE THINGS SEPERATLY

	//LETS YOU PAUSE INTO THE MENU
	if (GetKey(VK_ESCAPE).Pressed) {
		menu->Timer = 0;
		menu->bigPacman.x = ScreenWidth();
		menu->bigPacman.dir = NONE;
		for (int i = 1; i < 5; i++)
			menu->ghosts[i - 1].x = ScreenWidth() + i * 15;
		menu->ghosts[4].x = 65;
		GameState = PAUSEMENU;
		PlayNewSound(L"pause"); //STARTS NEW SOUND "PAUSE" IN THE BACKGROUND
	}

	UpdateThread.join(); // WAITS FOR THREADS TO FININSH TO CONTINUE THE MAIN LOOP
	DrawThread.join();
	return true;
}

void Game::PlayUpdate(float ElapsedTime) { // CALLED IN A SEPERATE THREAD

	for (int i = 0; i < UPDATE_INTERVALS; i++) { // ITERATE MULTIPLE TIMES TO INCREASE PRESICION

		if (redGhost.Update(map->TileMap, ElapsedTime, pacman)) //UPDATES ALGORITHEMS AND POSITIONS
			GhostCollided(redGhost); //GHOST AND PACMAN OVERLAP SO DECIDE WHAT TO DO

		if (pinkGhost.Update(map->TileMap, ElapsedTime, pacman))//UPDATES ALGORITHEMS AND POSITIONS
			GhostCollided(pinkGhost);//GHOST AND PACMAN OVERLAP SO DECIDE WHAT TO DO

		if (blueGhost.Update(map->TileMap, ElapsedTime, pacman, redGhost, pinkGhost))//UPDATES ALGORITHEMS AND POSITIONS
			GhostCollided(blueGhost);//GHOST AND PACMAN OVERLAP SO DECIDE WHAT TO DO

		if (greenGhost.Update(map->TileMap, ElapsedTime, pacman))//UPDATES ALGORITHEMS AND POSITIONS
			GhostCollided(greenGhost);//GHOST AND PACMAN OVERLAP SO DECIDE WHAT TO DO

		if (pacman.Dead && pacman.deathTimer > DEATH_TIME) //PACMAN DIED BUT STILL HAS HEALTH SINCE IN THE PREVIOUS ITERATION WHEN HE DIED THE STATE WASNT SET TO GAMEOVER
			GameState = RESET;

		if (!pacman.Update(ElapsedTime, map->TileMap)) { //GET USER INPUT UPDATE POSITIONS AND RETURN IF THE HEALTH IS STILL ABOVE 0
			GameState = GAMEOVER; //HEALTH = 0 SO ITS GAME OVER
			menu->highscore->SaveScore(pacman.Score + pacman.roundScore); //SAVE THE HIGHSCORE IN AN EXTRA DOCUMENT TO LOAD ON RESTART
		}

		Ghost* g[4]{ &redGhost, &pinkGhost, &blueGhost, &greenGhost };

		if (map->UpdatePoints(pacman, g)) // UPDATES THE POINTS / LARGE ONE UPDATES ALL GHOSTS
			GameState = LVLUP; // NO POINTS LEFT SO THIS LVL IS FINISHED

		fruit.Update(ElapsedTime, pacman); //UPDATE THE FRUITS
	}
}

void Game::PlayDraw(float ElapsedTime) {

	//DRAWS MAP POINTS FRUITS ADD INFORMATION AND PACMAN
	DrawMap(ElapsedTime); 

	if (pacman.GhostScore > 100) //PACMAN ATE ATLEAST ONE GHOST SO DRAW THE SCORE HE EARNED
		DrawSprite(toMap(pacman.x + 0.8, pacman.y), res->GetSprite(std::to_wstring(pacman.GhostScore)));

	if (!pacman.Dead) { // DRAW ALL GHOSTS ONLY IF PACMAN IS STILL ALIVE
		redGhost.DrawSelf(ElapsedTime);
		pinkGhost.DrawSelf(ElapsedTime);
		blueGhost.DrawSelf(ElapsedTime);
		greenGhost.DrawSelf(ElapsedTime);
	}
}

bool Game::GameOver(float ElapsedTime) {
	CLS; //CLEAR THE SCREEN
	if (pacman.deathTimer < DEATH_TIME) //ONLY IF PACMANS DEATH ANIMATION HASNT FINISHED
		DrawMap(ElapsedTime); //DRAWS MAP POINTS FRUITS ADD INFORMATION AND PACMAN

	if (pacman.deathTimer > DEATH_TIME) { //SHOW THE GAME OVER SIGN
		pacman.deathTimer += 10 * ElapsedTime;
		res->drawText(L"GAME OVER", ScreenWidth() / 2 - 27, ScreenHeight() / 2 - 5);
	}
	if (pacman.deathTimer > DEATH_TIME + 20) //RESET TO THE MENU
		GameState = MENU;
	
	return true;
}





