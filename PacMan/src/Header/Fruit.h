#pragma once
#include "Pos.h"
#include "PacMan.h"

class Fruit {
public:
	Fruit(ConsoleGameEngine* c);
	~Fruit();

	void Update(float ElpasedTime, PacMan& pm);
	void Revive(int use);
	void DrawSelf();

	Sprite* s = nullptr;
	ConsoleGameEngine* c = nullptr;
	int i = 0, uses = 0;
	float Timer = 0;
	bool interactable = false;
};

