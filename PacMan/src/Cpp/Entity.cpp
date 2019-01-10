
#include "Entity.h"

bool Entity::IsInRange(float ox, float oy, float range) { //ONE LINER FOR COLLISSION CHECKS
	return ((abs(x - ox) < range) && (abs(y - oy) < range));
}

void Entity::LoadSprites(const std::wstring& path) {

	//UP SPRITES CREATED
	anim[0].sprite[0] = new Sprite(L"res/" + path + L"UpClosed.spr");
	anim[0].sprite[1] = new Sprite(L"res/" + path + L"UpOpen.spr");
	 
	//RIGHT SPRITES CREATED
	anim[1].sprite[0] = new Sprite(L"res/" + path + L"RightClosed.spr");
	anim[1].sprite[1] = new Sprite(L"res/" + path + L"RightOpen.spr");

	//DOWN SPRITES CREATED
	anim[2].sprite[0] = new Sprite(L"res/" + path + L"DownClosed.spr");
	anim[2].sprite[1] = new Sprite(L"res/" + path + L"DownOpen.spr");

	//LEFT SPRITES CREATED
	anim[3].sprite[0] = new Sprite(L"res/" + path + L"LeftClosed.spr");
	anim[3].sprite[1] = new Sprite(L"res/" + path + L"LeftOpen.spr");
}

void Entity::DrawSelf(float ElapsedTime) {

	if (Dead) {
		deathTimer += ElapsedTime * 10; //UPDATE DEATHTIME
		Death(); // DRAW THE SPECIFIC DEATH SPRITES
	}
	else if (dir != NONE) {
		UpdateTimer(ElapsedTime); //UPDATE TIMER
		c->DrawSprite(toMap(x, y), anim[dir].sprite[animat]); // DRAW ENTITY SPRITE BASED ON THE DIRECTION AND THE ANIMATION STATE
	}
	else
		Idle();	//DRAW THE IDLE ANIMATIONS
}

bool Entity::Update(Tile* map, float ElapsedTime, float Speed, bool Dead) {

	if (Dead) { //ONLY UPDATE IF DEAD
		UpdateDir(map, Dead); //UPDATES DIR BASED ON POSITION AND SOLL
		UpdatePos(ElapsedTime, Speed); //INCREASES THE SAMPLE X AND Y COORDS
		CheckCollission(map); //CHECK FOR COLLISSION ELSE SET POSITION EQUAL TO SAMPLE POSITION
	}
	else
		return Died(); //IF !DEAD UPDATE BASED ON THE DIED FUNCTION
	return true;
}

void Entity::UpdateTimer(float ElapsedTime) {
	Timer += ElapsedTime;

	if (Timer >= 0.3f) { // ALL 0.3 SECONDS CHANGE THE ANIMATION STATE -> WALKING ANIMATION
		Timer -= 0.3f;
		animat = !animat;
	}
}

void Entity::UpdateDir(Tile* map, bool Dead) {

	if ((dir == NONE || map[gx + w * gy].corner) && Dead) { //ONLY UPDATE IF ENTITY IS ON A CORNER OR DIR IS EQUAL TO NONE
		if (soll == UP && !map[(int)(gx + w * (y - 1))].solid && ((dir == RIGHT && x - floor(x) < 0.25) ||
			(dir == LEFT && x - floor(x) > 0.75) || dir == NONE || dir == DOWN)) { //IF SOLL IS THE SAME AND THE COORDS ARE BETWEEN TO POINTS SINCE ROUNDING WILL MAKE THEM JUMP OTHERWISE
			dir = UP;
			x = round(x); // ROUND TO CENTER ENTITY
			gy -= 1;
		}
		if (soll == DOWN && !map[(int)(gx + w * (y + 1))].solid && ((dir == RIGHT && x - floor(x) < 0.25) ||
			(dir == LEFT && x - floor(x) > 0.75) || dir == NONE || dir == UP)) { //IF SOLL IS THE SAME AND THE COORDS ARE BETWEEN TO POINTS SINCE ROUNDING WILL MAKE THEM JUMP OTHERWISE
			dir = DOWN;
			x = round(x); // ROUND TO CENTER ENTITY
			gy += 1;
		}
		if (soll == LEFT && !map[(int)((gx - 1) + w * y)].solid && ((dir == DOWN && y - floor(y) < 0.25) ||
			(dir == UP && y - floor(y) > 0.75) || dir == NONE || dir == RIGHT)) { //IF SOLL IS THE SAME AND THE COORDS ARE BETWEEN TO POINTS SINCE ROUNDING WILL MAKE THEM JUMP OTHERWISE
			dir = LEFT;
			y = round(y); // ROUND TO CENTER ENTITY
			gx -= 1;
		}
		if (soll == RIGHT && !map[(int)((gx + 1) + w * y)].solid && ((dir == DOWN && y - floor(y) < 0.25) ||
			(dir == UP && y - floor(y) > 0.75) || dir == NONE || dir == LEFT)) { //IF SOLL IS THE SAME AND THE COORDS ARE BETWEEN TO POINTS SINCE ROUNDING WILL MAKE THEM JUMP OTHERWISE
			dir = RIGHT;
			y = round(y); // ROUND TO CENTER ENTITY
			gx += 1;
		}
	}
}

void Entity::UpdatePos(float ElapsedTime, float Speed, bool b) {

	//CREATES THE SAMPLE COORDS BASED ON DIRECTION
	switch (dir) {
	case UP:	sx = x;							sy = y - Speed * ElapsedTime;	break;
	case RIGHT:	sx = x + Speed * ElapsedTime;	sy = y;							break;
	case DOWN:	sx = x;							sy = y + Speed * ElapsedTime;	break;
	case LEFT:	sx = x - Speed * ElapsedTime;	sy = y;							break;
	case NONE:	sx = x;							sy = y;							break;
	}

	//IF THE POITION IS IN THE TUNNEL MOVE TO THE OTHER SIDE
	if (sx > 26.9 && sy == 14)
		sx -= 26.8;
	//IF THE POITION IS IN THE TUNNEL MOVE TO THE OTHER SIDE
	if (sx < 0.1 && sy == 14)
		sx += 26.8;
	
	//OVERRIDE FOR MENU ANIMATIONS
	if (b) {
		x = sx;
		y = sy;
		gx = round(x);
		gy = round(y);
	}
}

bool Entity::CheckCollission(Tile* map) {

	if (map[(int)(round(sx)) + w * (int)(round(sy))].solid) { //MAP AT SAMPLE COORDS IS SOLID
		dir = NONE;	//STOP RUNNING INTO THE WALL
		x = round(x); //CENTER COORDS
		y = round(y);
		gx = round(x);
		gy = round(y);
		return true;
	}
	x = sx; //MAP AT SAMPLE COORDS ISNT SOLID -> SET COORDS EQUAL TO SAMPLE COORDS
	y = sy;
	gx = round(x);
	gy = round(y);
	return false;
}

bool Entity::Died() { 
	return true; //LEFT FOR OVERRIDE
}
void Entity::Revive() {
	//LEFT FOR OVERRIDE
}

void Entity::Idle() {
	c->DrawSprite(toMap(x, y), anim[0].sprite[0]); //LEFT FOR OVERRIDE ELSE DONT CRASH DRAW FIRST SPRITE
}
void Entity::Death() {
	c->DrawSprite(toMap(x, y), anim[0].sprite[0]); //LEFT FOR OVERRIDE ELSE DONT CRASH DRAW FIRST SPRITE
}