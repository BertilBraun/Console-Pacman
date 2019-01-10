#pragma once

#include <string>

#include "Points.h"
#include "Entity.h"
#include "ConsoleGameEngine.h"

class PacMan : public Entity {
public:
	int Health = 3;
	int roundScore = 0, Score = 0, lvl = 0, GhostScore = 100;

public:
	PacMan(ConsoleGameEngine* c, float x, float y);
	~PacMan();

	bool Update(float ElapsedTime, Tile* map);
	void UserInput(float ElapsedTime);

	void Death() override;
	void Idle() override;
	bool Died() override;
	void Revive() override;

	Sprite* IdleSprite = nullptr;
	Sprite* DeathSprite[11] = { nullptr };
	bool iterated = false;
};

