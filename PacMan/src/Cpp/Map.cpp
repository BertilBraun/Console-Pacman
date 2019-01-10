#include "Map.h"


Map::Map(const std::wstring& path) {
	TileMap = new Tile[w * h]; //CREATE A NEW TILE ARRAY
	mapSprite = new Sprite(L"res/enviroment/" + path + L".spr"); //LOAD THE MAP SPRITE

	if (!LoadMap(L"res/enviroment/" + path + L".txt")) //LOAD THE MAP
		corrupted = true;

	points = allPoints;
}

Map::~Map() {
	delete[] TileMap; //DELETE TILE ARRAY
	delete mapSprite; //DELETE MAP SPRITE
}

bool Map::UpdatePoints(PacMan& pacman, Ghost* g[4]) {

	Posf p(pacman.gx, pacman.gy);

	if (points.find(p) != points.end()) { //SEARCH FOR PACMANS POS
		pacman.roundScore += points.find(p)->second.score; //ADD THE POINTS TO THE SCORE
		if (points.find(p)->second.score == 50) {//BIG COIN SO GHOSTS GET FRIGHTEND
			for (int i = 0; i < 4; i++) {
				g[i]->GTimer = 0;
				g[i]->state = FRIGHTEND;
			}
			pacman.GhostScore = 100;
			PlayNewSound(L"frightend", 2);
		}
		points.erase(p); //ERASE POINT TO NOT BE PROCESSED FURTHER
	}

	return points.empty(); //RETURN IF THERE ARE POINTS LEFT FOR A LVL UP NONE MAY BE LEFT (EMPTY)
}

bool Map::LoadMap(const std::wstring& path) {
	std::ifstream stream;
	int temp;

	stream.open(path.c_str());

	if (!stream.good()) //MAP PATH CANT BE OPEND SO YOU CANT PLAY
		return false; 

	for (int y= 0; y < h; y++) //LOOP THROUGH THE WHOLE MAP
		for (int x = 0; x < w; x++) { //LOOP THROUGH THE WHOLE MAP
			
			stream >> temp;
			if (temp != x + w * y) //MAP FORMAT DOSNT EQUAL THE EXPECTED FORMAT
				return false;

			stream >> temp;

			switch (temp) { //BASED ON ID INSERT POINTS MAKE CORNERS AND SOLID MAP PARTS
			case 6:
				allPoints.insert(std::make_pair(Posf(x, y), Point(50)));
				TileMap[x + w * y].corner = true;
				break;
			case 5:
				allPoints.insert(std::make_pair(Posf(x, y), Point(50)));
				break;
			case 4:
				allPoints.insert(std::make_pair(Posf(x, y), Point(10)));
				break;
			case 3:
				allPoints.insert(std::make_pair(Posf(x, y), Point(10)));
				TileMap[x + w * y].corner = true;
				break;
			case 2:
				TileMap[x + w * y].corner = true;
				break;
			case 1:
				TileMap[x + w * y].solid = true;
				break;
			}
		}
	stream.close();
	return true;
}
