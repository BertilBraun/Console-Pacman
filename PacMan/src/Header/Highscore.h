#pragma once
#include <fstream>

class Highscore {
public:
	Highscore();
	~Highscore();

	void SaveScore(int score);
	int score = 0;
};

