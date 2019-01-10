#pragma once

#include "Entity.h"
#include "PacMan.h"

#include "pos.h"
#include "vec2.h"

enum GHOSTSTATE {
	INBOX,
	CHASE,
	SCATTER,
	FRIGHTEND,
	RETURNING
};

class Ghost : public Entity {
public:
	GHOSTSTATE state = INBOX;
	Animation feared[2];
	Sprite* eye[4];
	float InBoxTimer = 0, TimeToGo = 3, GTimer = 0;

	Ghost(ConsoleGameEngine *c, const std::wstring& path, float x, float y);
	~Ghost();

	virtual void Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman) {}
	bool Update(Tile* map, float ElapsedTime, PacMan& pacman);
	void AlgPosUpdate(Posf& pp, Tile* map);
	void Revive();
	void DrawSelf(float ElapsedTime);

protected:
	float GhostSpeed = 0;
	float GhostStartX = 0, GhostStartY = 0;
	Posf frightend;
	Posf target;
};

class RedGhost : public Ghost {
public:
	RedGhost(ConsoleGameEngine *c, const std::wstring& path, float x, float y) : Ghost(c, path, x, y) {
		GhostSpeed = RED_SPEED;
		GhostStartX = RED_START_X;
		GhostStartY = RED_START_Y;
		TimeToGo = 0.5;
		frightend = Posf(RED_SCATTER_POS);
	}

	void Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman);

private:

};

class PinkGhost : public Ghost {
public:
	PinkGhost(ConsoleGameEngine *c, const std::wstring& path, float x, float y) : Ghost(c, path, x, y) {
		GhostSpeed = PINK_SPEED;
		GhostStartX = PINK_START_X;
		GhostStartY = PINK_START_Y;
		TimeToGo = 3;
		frightend = Posf(PINK_SCATTER_POS);
	}

	void Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman);
private:

};

class BlueGhost : public Ghost {
public:
	BlueGhost(ConsoleGameEngine *c, const std::wstring& path, float x, float y) : Ghost(c, path, x, y) {
		GhostSpeed = BLUE_SPEED;
		GhostStartX = BLUE_START_X;
		GhostStartY = BLUE_START_Y;
		TimeToGo = 5;
		frightend = Posf(BLUE_SCATTER_POS);
	}

	void Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman, RedGhost r, PinkGhost p);
	bool Update(Tile* map, float ElapsedTime, PacMan& pacman, RedGhost r, PinkGhost p);
private:
};


class GreenGhost : public Ghost {
public:
	GreenGhost(ConsoleGameEngine *c, const std::wstring& path, float x, float y) : Ghost(c, path, x, y) {
		GhostSpeed = GREEN_SPEED;
		GhostStartX = GREEN_START_X;
		GhostStartY = GREEN_START_Y;
		TimeToGo = 6;
		frightend = Posf(GREEN_SCATTER_POS);
	}

	void Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman);
private:
};