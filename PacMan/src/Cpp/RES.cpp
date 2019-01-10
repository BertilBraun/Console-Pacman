#include "RES.h"

RES::RES(ConsoleGameEngine* c) : c(c) {
	//LOAD ALL SPRITES ON CREATION
	res[L"A"] = new Sprite(L"res/other/Letters/A.spr");
	res[L"B"] = new Sprite(L"res/other/Letters/B.spr");
	res[L"C"] = new Sprite(L"res/other/Letters/C.spr");
	res[L"D"] = new Sprite(L"res/other/Letters/D.spr");
	res[L"E"] = new Sprite(L"res/other/Letters/E.spr");
	res[L"F"] = new Sprite(L"res/other/Letters/F.spr");
	res[L"G"] = new Sprite(L"res/other/Letters/G.spr");
	res[L"H"] = new Sprite(L"res/other/Letters/H.spr");
	res[L"I"] = new Sprite(L"res/other/Letters/I.spr");
	res[L"J"] = new Sprite(L"res/other/Letters/J.spr");
	res[L"K"] = new Sprite(L"res/other/Letters/K.spr");
	res[L"L"] = new Sprite(L"res/other/Letters/L.spr");
	res[L"M"] = new Sprite(L"res/other/Letters/M.spr");
	res[L"N"] = new Sprite(L"res/other/Letters/N.spr");
	res[L"O"] = new Sprite(L"res/other/Letters/O.spr");
	res[L"P"] = new Sprite(L"res/other/Letters/P.spr");
	res[L"Q"] = new Sprite(L"res/other/Letters/Q.spr");
	res[L"R"] = new Sprite(L"res/other/Letters/R.spr");
	res[L"S"] = new Sprite(L"res/other/Letters/S.spr");
	res[L"T"] = new Sprite(L"res/other/Letters/T.spr");
	res[L"U"] = new Sprite(L"res/other/Letters/U.spr");
	res[L"V"] = new Sprite(L"res/other/Letters/V.spr");
	res[L"W"] = new Sprite(L"res/other/Letters/W.spr");
	res[L"X"] = new Sprite(L"res/other/Letters/X.spr");
	res[L"Y"] = new Sprite(L"res/other/Letters/Y.spr");
	res[L"Z"] = new Sprite(L"res/other/Letters/Z.spr");

	res[L"0"] = new Sprite(L"res/other/Numbers/0.spr");
	res[L"1"] = new Sprite(L"res/other/Numbers/1.spr");
	res[L"2"] = new Sprite(L"res/other/Numbers/2.spr");
	res[L"3"] = new Sprite(L"res/other/Numbers/3.spr");
	res[L"4"] = new Sprite(L"res/other/Numbers/4.spr");
	res[L"5"] = new Sprite(L"res/other/Numbers/5.spr");
	res[L"6"] = new Sprite(L"res/other/Numbers/6.spr");
	res[L"7"] = new Sprite(L"res/other/Numbers/7.spr");
	res[L"8"] = new Sprite(L"res/other/Numbers/8.spr");
	res[L"9"] = new Sprite(L"res/other/Numbers/9.spr");

	res[L"Ready"] = new Sprite(L"res/enviroment/readySign.spr");

	res[L"smallCoin"] = new Sprite(L"res/enviroment/smallCoin.spr");
	res[L"bigCoin"] = new Sprite(L"res/enviroment/bigCoin.spr");
	
	res[L"icon"] = new Sprite(L"res/other/Icon/icon.spr");

	res[L"redGhost"] = new Sprite(L"res/ghosts/redGhost/redGhostUpClosed.spr");
	res[L"pinkGhost"] = new Sprite(L"res/ghosts/pinkGhost/pinkGhostUpClosed.spr");
	res[L"blueGhost"] = new Sprite(L"res/ghosts/blueGhost/blueGhostUpClosed.spr");
	res[L"greenGhost"] = new Sprite(L"res/ghosts/greenGhost/greenGhostUpClosed.spr");

	res[L"200"] = new Sprite(L"res/other/Points/200.spr");
	res[L"400"] = new Sprite(L"res/other/Points/400.spr");
	res[L"800"] = new Sprite(L"res/other/Points/800.spr");
	res[L"1600"] = new Sprite(L"res/other/Points/1600.spr");

}

RES::~RES() {
	for (auto &r : res) 
		delete r.second; //DELETE EVERY SINGLE LOADED FILE
}

Sprite* RES::GetSprite(const std::wstring& s) {
	return res[s]; //RETURN SPRITE POINTER BASED ON STRING PASSED IN
}

void RES::drawText(const std::wstring& s, int x, int y) {
	for (int i = 0; i < s.size(); i++) //SPLIT STRING INTO SINGLE CHARACTERS
		c->DrawSprite(x + (i * 6), y, res[std::wstring(1, s[i])]); //DRAW SPRITE TAKING THE SPRITE OUT OF THE MAP
}
