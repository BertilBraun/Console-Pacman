#include "Menu.h"


Menu::Menu(ConsoleGameEngine* c) : c(c) {
	highscore = new Highscore(); // CREATE NEW HIGHCORE OBJECT SO LOAD AND SAVE HIGHSCORES
}

Menu::~Menu() {
	delete highscore;
}

bool Menu::Update(float ElapsedTime, GAMESTATE& state, RES& res, PacMan& pacman, Map& map, Fruit& fruit) {
	
	Timer += ElapsedTime; //UPDATE TIMING

	return Draw(ElapsedTime, state, res, pacman, map, fruit); // DRAW EVERYTHING
}

void Menu::DrawHowTo(RES& res, GAMESTATE& gameState) {
	CLSc;

	res.drawText(L"HOW TO PLAY PACMAN", 15, 5);

	res.drawText(L"USE  W TO GO UP", 15, 25);
	res.drawText(L"     A TO GO LEFT", 15, 35);
	res.drawText(L"     S TO GO DOWN", 15, 45);
	res.drawText(L"     D TO GO RIGHT", 15, 55);
	res.drawText(L"OR USE THE ARROW KEYS", 15, 65);

	res.drawText(L"COLLECT ALL COINS", 15, 90);
	res.drawText(L"TO GET TO THE NEXT LVL", 15, 100);

	res.drawText(L"WATCH OUT AND DODGE THE GHOSTS", 15, 120);

	res.drawText(L"COLLECTING A BIG COIN", 15, 140);
	res.drawText(L"WILL ALLOW YOU TO EAT THE GHOSTS", 15, 150);
	res.drawText(L"FOR A LIMITED AMOUNT OF TIME", 15, 160);

	res.drawText(L"AFTER EATING ALL COINS YOU LEVEL UP", 15, 180);
	
	res.drawText(L"FRUITS RANOMLY SPAWN IN THE MIDDLE", 15, 200);
	res.drawText(L"OF THE MAP  EATING THEM PROVIDES", 15, 210);
	res.drawText(L"SPECIAL BOOSTS  TRY IT", 15, 220);

	res.drawText(L"CODED BY BERTIL AND SPRITES BY GOKI", 10, c->ScreenHeight() - 35);
	res.drawText(L"SPACE OR CLICK TO GO TO BACK", 25, c->ScreenHeight() - 20);

	c->DrawSprite(155, 20, res.GetSprite(L"icon"));

	if (c->GetKey(VK_SPACE).Released || c->GetMouseX() > 22 && c->GetMouseX() < 200 && c->GetMouseY() > c->ScreenHeight() - 22 && c->GetMouseY() < c->ScreenHeight() - 12 && c->GetMouse(0).Pressed)
		gameState = MENU;
}

void Menu::UpdateAnim(float ElapsedTime, RES& res) {

	if (Timer < 10 && Timer > 3) { //TIMING DONT START DIRECTLY THEN FOR THE FIRST 7 SECONDS
		bigPacman.dir = LEFT; //GOING LEFT

		c->DrawSprite(50, 174, res.GetSprite(L"bigCoin")); //DRAW A BIG COIN FOR PACMAN TO REACH

		for (int i = 0; i < 4; i++) { // DRAW & UPDATE ALL 4 GHOSTS
			ghosts[i].dir = LEFT;
			ghosts[i].UpdatePos(ElapsedTime, 25, true); //UPDATE
			c->DrawSprite(ghosts[i].x, 170, ghosts[i].anim[3].sprite[ghosts[i].animat]); //DRAW
		}
		c->DrawSprite(bigPacman.x, 170, bigPacman.anim[1].sprite[bigPacman.animat]);//DRAW
	}
	else if (Timer > 10) { //AFTER 10 SECONDS PACMAN GOT THE PILL AND IS NOW CHASING THE GHOSTS
		bigPacman.dir = RIGHT; //GOING RIGHT

		ghosts[4].dir = RIGHT; //4 GHOST IS THE BLUE WHITE FEARED GHOST
		ghosts[4].UpdatePos(ElapsedTime, 25, true); //UPDATE
		for (int i = 0; i < 4; i++)
			//DRAW THE FEARED GHOST 4x WITH DIFFERENT OFFSETS
			c->DrawSprite(ghosts[4].x + i * 15, 170, ghosts[4].anim[(int)((Timer + i) * 4) % 2].sprite[ghosts[4].animat]); 
		c->DrawSprite(bigPacman.x - 15, 160, bigPacman.anim[0].sprite[bigPacman.animat]); //DRAW
	}
	if (Timer > 20 || Timer == 0) { //WHEN JOINED INTO THE MENU OR THE ANIMATION ENDED RESET EVERYTHING
		if (Timer > 20)
			Timer -= 20;
		bigPacman.dir = NONE;
		bigPacman.x = c->ScreenWidth();
		for (int i = 1; i < 5; i++)
			ghosts[i - 1].x = c->ScreenWidth() + 15 * i;
		ghosts[4].x = 65;
	}
	for (int i = 0; i < 5; i++)
		ghosts[i].UpdateTimer(ElapsedTime);
	bigPacman.UpdateTimer(ElapsedTime);
	bigPacman.UpdatePos(ElapsedTime, 25, true);

}

bool Menu::Draw(float ElapsedTime, GAMESTATE& gameState, RES& res, PacMan& pacman, Map& map, Fruit& fruit) {
	CLSc;//CLEAR SCREEN

	UpdateAnim(ElapsedTime, res); //UPDATE THE ONGOING ANIMATION

	//DRAW THE INFORMATIONS
	res.drawText(L"HIGHSCORE " + std::to_wstring(highscore->score), c->ScreenWidth() / 2 - 45, 3);
	res.drawText(L"CHARACTER   NICKNAME", 65, 50);

	c->DrawSprite(50, 65, res.GetSprite(L"redGhost"));
	res.drawText(L"SHADOW      BLINKY", 65, 65);

	c->DrawSprite(50, 80, res.GetSprite(L"pinkGhost"));
	res.drawText(L"SPEEDY      PINKY", 65, 80);

	c->DrawSprite(50, 95, res.GetSprite(L"blueGhost"));
	res.drawText(L"BASHFULL    INKY", 65, 95);

	c->DrawSprite(50, 110, res.GetSprite(L"greenGhost"));
	res.drawText(L"POLEY       CLYDE", 65, 110);

	c->DrawSprite(85, 240, res.GetSprite(L"smallCoin"));
	res.drawText(L"10 PTS", 100, 240);

	c->DrawSprite(85, 255, res.GetSprite(L"bigCoin"));
	res.drawText(L"50 PTS", 100, 255);

	if (gameState == MENU) { //DEPENDANT ON THE MENU STATE DIFFERENT THINGS HAVE TO BE DRAWN / UPDATED
		//SETS STATE TO DRAW THE HOW TO MENU
		res.drawText(L"HOW TO PLAY", c->ScreenWidth() / 2 - 34, c->ScreenHeight() - 70);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 36 && c->GetMouseX() < c->ScreenWidth() / 2 + 36 && c->GetMouseY() > c->ScreenHeight() - 72 && c->GetMouseY() < c->ScreenHeight() - 62 && c->GetMouse(0).Pressed)
			gameState = HOWTO;

		//QUITS
		res.drawText(L"QUIT", c->ScreenWidth() / 2 - 12, c->ScreenHeight() - 60);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 12 && c->GetMouseX() < c->ScreenWidth() / 2 + 12 && c->GetMouseY() > c->ScreenHeight() - 62 && c->GetMouseY() < c->ScreenHeight() - 52 && c->GetMouse(0).Pressed)
			gameState = QUIT;

		//RESETS EVERYTHING! THEN GOES TO START
		res.drawText(L"PLAY", c->ScreenWidth() / 2 - 12, c->ScreenHeight() - 80);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 14 && c->GetMouseX() < c->ScreenWidth() / 2 + 14 && c->GetMouseY() > c->ScreenHeight() - 82 && c->GetMouseY() < c->ScreenHeight() - 72 && c->GetMouse(0).Pressed || c->GetKey(VK_SPACE).Released) {
			pacman.Health = 3;
			pacman.Score = 0;
			pacman.roundScore = 0;
			map.points = map.allPoints;
			gameState = RESET;
			fruit.Revive(2);
			PlayNewSound(L"start", 2);
			return false;
		}
	}
	else if (gameState == PAUSEMENU) {
		//SETS STATE TO MAIN MENU
		res.drawText(L"MAIN MENU", c->ScreenWidth() / 2 - 28, c->ScreenHeight() - 70);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 30 && c->GetMouseX() < c->ScreenWidth() / 2 + 30 && c->GetMouseY() > c->ScreenHeight() - 72 && c->GetMouseY() < c->ScreenHeight() - 62 && c->GetMouse(0).Pressed) {
			gameState = MENU;
			highscore->SaveScore(pacman.Score + pacman.roundScore); // SAVES THE HIGHSCORE
		}
		//QUIT
		res.drawText(L"QUIT", c->ScreenWidth() / 2 - 12, c->ScreenHeight() - 60);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 12 && c->GetMouseX() < c->ScreenWidth() / 2 + 12 && c->GetMouseY() > c->ScreenHeight() - 62 && c->GetMouseY() < c->ScreenHeight() - 52 && c->GetMouse(0).Pressed)
			gameState = QUIT;
		//GOES BACK TO START / PLAY STATE
		res.drawText(L"CONTINUE", c->ScreenWidth() / 2 - 24, c->ScreenHeight() - 80);
		if (c->GetMouseX() > c->ScreenWidth() / 2 - 26 && c->GetMouseX() < c->ScreenWidth() / 2 + 26 && c->GetMouseY() > c->ScreenHeight() - 82 && c->GetMouseY() < c->ScreenHeight() - 72 && c->GetMouse(0).Pressed || c->GetKey(VK_SPACE).Released) {
			gameState = START;
			PlayNewSound(L"start", 2);
		}
	}
}