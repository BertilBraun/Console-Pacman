#pragma once

#include "ConsoleGameEngine.h"

struct Point {
	
	Point(int score) : score(score) {
		switch (score) {
		case 10: sprite = new Sprite(L"res/enviroment/smallCoin.spr"); break;
		case 50: sprite = new Sprite(L"res/enviroment/bigCoin.spr"); break;
		}
	}

	int score = 0;
	Sprite* sprite = nullptr;
};