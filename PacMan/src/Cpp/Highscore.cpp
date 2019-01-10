#include "Highscore.h"

Highscore::Highscore() { //LOAD THE HIGHSCORE
	FILE *f = nullptr;
	_wfopen_s(&f, L"res/other/Highscore.sc", L"rb");
	
	if (f == nullptr)
		return;

	fread(&score, sizeof(int), 1, f);
	fclose(f);
}

Highscore::~Highscore() {
	SaveScore(score);
}

void Highscore::SaveScore(int score) { //SAVE THE HIGHSCORE IF ITS LARGER THAN THE CURRENT SCORE
	if (this->score < score) {
		this->score = score;
		FILE *f = nullptr;
		_wfopen_s(&f, L"res/other/Highscore.sc", L"wb");
		
		if (f == nullptr)
			return;
		
		fwrite(&this->score, sizeof(int), 1, f);
		fclose(f);
	}
}
