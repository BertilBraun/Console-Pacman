#pragma once
#include "ConsoleGameEngine.h"
#include "defines.h"

#include "Animation.h"
#include "Tile.h"

class Entity { // BASE CLASS FOR ALL MOVEABLE ENTITIES
protected:
	ConsoleGameEngine* c = nullptr;

public:
	bool animat = false;
	Animation anim[4];
	float deathTimer = 0, Timer = 0, x = 0, y = 0;
	bool Dead = false;
	int gx = x, gy = y;
	float sx = 0, sy = 0;

	DIREC dir  = UP;
	DIREC soll = UP;

	Entity(ConsoleGameEngine* c, const std::wstring& SpriteName, float x = 0, float y = 0) : c(c), x(x), y(y), gx(x), gy(y) {
		LoadSprites(SpriteName);
	}

	void LoadSprites(const std::wstring& path);	
	bool Update(Tile* map, float ElapsedTime, float Speed, bool Dead);
	bool IsInRange(float ox, float oy, float range = 1);
	void DrawSelf(float ElapsedTime);
	void UpdateTimer(float ElapsedTime);
	void UpdatePos(float ElapsedTime, float Speed, bool b = false);
	bool CheckCollission(Tile* map);
	void UpdateDir(Tile* map, bool Dead);

	virtual bool Died(); 
	virtual void Revive();
	virtual void Idle(); 
	virtual void Death(); 

};

