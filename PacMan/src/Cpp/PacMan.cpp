
#include "PacMan.h"

PacMan::PacMan(ConsoleGameEngine* c, float x, float y) : Entity(c, L"pacman/pacman", x, y) {
	dir = UP;
	//ROUND PACMAN USED IF THE DIRECTION IS NONE
	IdleSprite = new Sprite(L"res/pacman/pacmanRound.spr");
	//DEATH SPRITES FOR THE DEATH ANIMATION
	DeathSprite[0] = IdleSprite;
	for (int i = 1; i < 10; i++)
		DeathSprite[i] = new Sprite(L"res/pacman/Death/pacmanDeath" + std::to_wstring(i) + L".spr");
	DeathSprite[10] = new Sprite(L"res/pacman/Death/pacmanPuff.spr");
}

PacMan::~PacMan() {}

bool PacMan::Update(float ElapsedTime, Tile* map) {
	//GETS USER INPUT
	UserInput(ElapsedTime);
	//UPDATES PACMAN LOCATION BASED ON USER INPUT AND PACMANS SPEED
	return Entity::Update(map, ElapsedTime, SPEED, !Dead);
}

void PacMan::UserInput(float ElapsedTime) {
	//GETKEY RETURNS THE STATE OF THE KEY | BASED ON INPUT UPDATE SOLL DIRECTION
	if (c->GetKey(VK_UP).Held || c->GetKey('W').Held)
		soll = UP;
	if (c->GetKey(VK_DOWN).Held || c->GetKey('S').Held)
		soll = DOWN;
	if (c->GetKey(VK_LEFT).Held || c->GetKey('A').Held)
		soll = LEFT;
	if (c->GetKey(VK_RIGHT).Held || c->GetKey('D').Held)
		soll = RIGHT;
}

void PacMan::Death() {
	
	if (deathTimer < 11)//DRAW THE DEATH ANIMATION
		c->DrawSprite(toMap(x, y), DeathSprite[(int)(deathTimer)]);
}


void PacMan::Idle() {
	//DRAW THE IDLE SPRITE
	c->DrawSprite(toMap(x, y), IdleSprite);
}

bool PacMan::Died() {
	if (!iterated) { //ONLY DO THIS ONCE PER DEATH
		iterated = true;
		deathTimer = 0;
		Health--;
	}
	return Health > 0;
}

void PacMan::Revive() {
	//RESET EVERYTHING
	x = PACMAN_START_X;
	y = PACMAN_START_Y;
	gx = x;
	gy = y;
	deathTimer = 0;
	iterated = false;
	Dead = false;
	dir = NONE;
	soll = NONE;
}