#include "defines.h"

void PlayNewSound(const std::wstring& s, int async) {
	PlaySound(NULL, NULL, NULL); //STOP ALL PLAYING SOUNDS
	std::wstring p = L"res/other/sounds/" + s + L".wav";

	if (async == 1)
		PlaySound((LPCWSTR)p.c_str(), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC); //PLAY NEW SOUNDS ASYNC SO GAME GOES ON AND LOOPS THE SOUND
	else if (async == 2)
		PlaySound((LPCWSTR)p.c_str(), NULL, SND_FILENAME | SND_ASYNC); //PLAY NEW SOUNDS ASYNC
	else
		PlaySound((LPCWSTR)p.c_str(), NULL, SND_FILENAME); //PLAY NEW SOUNDS
}