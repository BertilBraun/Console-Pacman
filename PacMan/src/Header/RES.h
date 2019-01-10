#pragma once
#include "ConsoleGameEngine.h"
#include <map>
#include <string>

class RES {
public:
	RES() {}
	RES(ConsoleGameEngine* c);
	~RES();

	void drawText(const std::wstring& s, int x, int y);
	Sprite* GetSprite(const std::wstring& s);

private:
	std::map<std::wstring, Sprite*> res;
	ConsoleGameEngine* c = nullptr;
};
