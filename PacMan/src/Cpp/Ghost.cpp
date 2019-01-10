
#include "Ghost.h"


Ghost::Ghost(ConsoleGameEngine* c, const std::wstring& path, float x, float y) : Entity(c, path, x, y) {
	//FEARD GHOST SPRITES
	feared[0].sprite[0] = new Sprite(L"res/ghosts/fearedGhost/fearedGhostRightClosed.spr");
	feared[0].sprite[1] = new Sprite(L"res/ghosts/fearedGhost/fearedGhostRightOpen.spr");
	feared[1].sprite[0] = new Sprite(L"res/ghosts/fearedGhost/fearedGhostUpClosed.spr");
	feared[1].sprite[1] = new Sprite(L"res/ghosts/fearedGhost/fearedGhostUpOpen.spr");

	//EYE / DEAD GHOST SPRITES
	eye[0] = new Sprite(L"res/ghosts/Eyes/eyesUp.spr");
	eye[1] = new Sprite(L"res/ghosts/Eyes/eyesRight.spr");
	eye[2] = new Sprite(L"res/ghosts/Eyes/eyesLeft.spr");
	eye[3] = new Sprite(L"res/ghosts/Eyes/eyesDown.spr");
}


Ghost::~Ghost() {
}

bool Ghost::Update(Tile* map, float ElapsedTime, PacMan& pacman) {

	switch (state) { //UPDATE GHOSTS BASED ON THEIR STATE
	case INBOX:
		InBoxTimer += ElapsedTime;
		if (InBoxTimer >= TimeToGo) { //GHOST IS SET OUT TO HUNT
			state = CHASE;
			soll = NONE;
			dir = NONE;
			y = GhostStartY - 3; //MOVE GHOST OUTSIDE THE BOX
			gy = GhostStartY - 3; //MOVE GHOST OUTSIDE THE BOX
		}
		break;
	case CHASE:
		GTimer += ElapsedTime;
		if (GTimer > 15) { // AFTER 15 SECONDS LEAVE THE PLAYER
			GTimer = 0;
			state = SCATTER;
		}
		Algorithm(ElapsedTime, map, pacman); // UPDATE THE TARGET POS
		break;
	case SCATTER:
		GTimer += ElapsedTime;
		if (pacman.lvl < 5) { //BASED ON PACMAN LVL DECREASE THE TIME IN SCATTER MODE
			if (GTimer > 6) {
				GTimer = 0;
				state = CHASE;
			}
		} 
		else  if (pacman.lvl < 10) { //BASED ON PACMAN LVL DECREASE THE TIME IN SCATTER MODE
			if (GTimer > 3) {
				GTimer = 0;
				state = CHASE;
			}
		} 
		else {
			GTimer = 0;
			state = CHASE;
		}
		target = frightend; //TARGETTILE IS THE FRIGHTEND POS
		break;
	case FRIGHTEND:
		GTimer += ElapsedTime;
		if (pacman.lvl < 5) { //BASED ON PACMAN LVL DECREASE THE TIME IN FRIGHTEND MODE
			if (GTimer > 6) {
				GTimer = 0;
				state = CHASE;
				pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
				PlayNewSound(L"play");
			}
		} else  if (pacman.lvl < 10) { //BASED ON PACMAN LVL DECREASE THE TIME IN FRIGHTEND MODE
			if (GTimer > 3) {
				GTimer = 0;
				state = CHASE;
				pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
				PlayNewSound(L"play");
			}
		} else {
			GTimer = 0;
			state = CHASE;
			pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
			PlayNewSound(L"play");
		}
		target = frightend; //TARGETTILE IS THE FRIGHTEND POS
		break;
	case RETURNING:
		GTimer += ElapsedTime;
		if (Pos(GhostStartX, GhostStartY - 3) == Pos(gx, gy) || GTimer > 20) //IF THE GHOST IS BACK REVIVE HIM
			Revive();

		target = Posf(GhostStartX, GhostStartY - 3);  //TARGETTILE IS THE HOME POS
		break;
	}

	if (Dead) { //GHOST IS DEAD SO SET THE STATE TO RETURNING
		GTimer = 0;
		state = RETURNING;
	}
	if (map[gx + w * gy].corner || dir == NONE) //ONLY IF THE GHOST IS ON A CORNER OR NOT MOVING UPDATE THE ALGORITHM POS UPDATE (SAVE COMPUTING POWER)
		AlgPosUpdate(target, map);

	switch (state) { //UPDATE MOVEMENT BASED ON STATE EQUALING VELOCITIES FROM BASE CLASS ENTITY
	case FRIGHTEND:
		Entity::Update(map, ElapsedTime, 0.5, true);
		break;
	case RETURNING:
		Entity::Update(map, ElapsedTime, GhostSpeed * 2, true);
		break;
	default:
		if (GTimer > 10 && IsInRange(RED_START_X, RED_START_Y, 2)) { //IF GHOST BUGGED OUT FORCE HIM OUT AFTER 10 SECONDS
			y = GhostStartY - 3;
			gy = GhostStartY - 3;
		}
		Entity::Update(map, ElapsedTime, GhostSpeed + (pacman.lvl > 10 ? 0.3 : 0), true); //AFTER LVL 10 THE GHOSTS ARE PARTIALY FASTER THAN PACMAN
		break;
	}

	return IsInRange(pacman.x, pacman.y, 0.8); //RETURN IF THE GHOST IS OVERLAPPING WITH PACMAN
}

void Ghost::AlgPosUpdate(Posf& pp, Tile* map) {
	Vec2 v1(500, 500), v2(500, 500), v3(500, 500), v4(500, 500);

	switch (dir) { //CREATE 4 VECTORS TO THE TARGET TILE IN EACH DIRECTION IF POSSIBLE
	case UP:
		if (!map[(gx + 0) + w * (gy - 1)].solid)
			v1 = Vec2(pp, Posf(gx, gy - 1));
		if (!map[(gx + 1) + w * (gy + 0)].solid)
			v2 = Vec2(pp, Posf(gx + 1, gy));
		if (!map[(gx - 1) + w * (gy + 0)].solid)
			v4 = Vec2(pp, Posf(gx - 1, gy));
		break;
	case DOWN:
		if (!map[(gx + 1) + w * (gy + 0)].solid)
			v2 = Vec2(pp, Posf(gx + 1, gy));
		if (!map[(gx + 0) + w * (gy + 1)].solid)
			v3 = Vec2(pp, Posf(gx, gy + 1));
		if (!map[(gx - 1) + w * (gy + 0)].solid)
			v4 = Vec2(pp, Posf(gx - 1, gy));
		break;
	case LEFT:
		if (!map[(gx + 0) + w * (gy - 1)].solid)
			v1 = Vec2(pp, Posf(gx, gy - 1));
		if (!map[(gx + 0) + w * (gy + 1)].solid)
			v3 = Vec2(pp, Posf(gx, gy + 1));
		if (!map[(gx - 1) + w * (gy + 0)].solid)
			v4 = Vec2(pp, Posf(gx - 1, gy));
		break;
	case RIGHT:
		if (!map[(gx + 0) + w * (gy - 1)].solid)
			v1 = Vec2(pp, Posf(gx, gy - 1));
		if (!map[(gx + 1) + w * (gy + 0)].solid)
			v2 = Vec2(pp, Posf(gx + 1, gy));
		if (!map[(gx + 0) + w * (gy + 1)].solid)
			v3 = Vec2(pp, Posf(gx, gy + 1));
		break;
	case NONE:
		if (!map[(gx + 0) + w * (gy - 1)].solid)
			v1 = Vec2(pp, Posf(gx, gy - 1));
		if (!map[(gx + 1) + w * (gy + 0)].solid)
			v2 = Vec2(pp, Posf(gx + 1, gy));
		if (!map[(gx + 0) + w * (gy + 1)].solid)
			v3 = Vec2(pp, Posf(gx, gy + 1));
		if (!map[(gx - 1) + w * (gy + 0)].solid)
			v4 = Vec2(pp, Posf(gx - 1, gy));
		break;
	}

	//SEARCH FOR THE SHORTEST AND UPDATE SOLL ACCORDING TO THAT VECTOR
	if (v1 < v2 && v1 < v3 && v1 < v4)
		soll = UP;
	else if (v2 < v3 && v2 < v4)
		soll = RIGHT;
	else if (v3 < v4)
		soll = DOWN;
	else
		soll = LEFT;
}

void Ghost::Revive() { // RESET EVERYTHING
	state = INBOX;
	x = GhostStartX;
	y = GhostStartY;
	gx = x;
	gy = y;
	GTimer = 0;
	InBoxTimer = 0;
	Dead = false;
	dir = NONE;
	soll = NONE;
}

void Ghost::DrawSelf(float ElapsedTime) {

	if (Dead) { //GHOST IS DEAD
		deathTimer += ElapsedTime * 10;
		if (dir == NONE) // GAVE AN ERROR ONCE
			c->DrawSprite(toMap(x,y), eye[0]);
		else // DRAW EYES
			c->DrawSprite(toMap(x,y), eye[dir]);
	}
	else {
		UpdateTimer(ElapsedTime); //UPDATE TIMING
		if (state == FRIGHTEND)
			c->DrawSprite(toMap(x,y), feared[((int)GTimer) % 2].sprite[animat]); //DRAW FEARED GHOST BLUE WHITE
		else
			c->DrawSprite(toMap(x,y), anim[dir % 4].sprite[animat]); // DRAW NORMAL GHOST BASED ON DIRECTION
	}
}

void RedGhost::Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman) {
	//SETS THE TARGET TILE TO PACMANS POS

	target = Posf(pacman.gx, pacman.gy);

}


void PinkGhost::Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman) {
	//SETS THE TARGET TILE BASED ON THE ALGORITHM ONE OR TWO STEPS AHEAD OF PACMAN
	
	Posf pp;
	switch (pacman.dir) {
	case UP:
		if (!map[(gx + 0) + w * (gy - 1)].solid)
			if (!map[(gx + 0) + w * (gy - 2)].solid)
				pp = Posf(pacman.gx, pacman.gy - 2);
			else
				pp = Posf(pacman.gx, pacman.gy - 1);
		else
			pp = Posf(pacman.gx, pacman.gy);
		break;
	case RIGHT:
		if (!map[(gx + 1) + w * (gy - 0)].solid)
			if (!map[(gx + 2) + w * (gy - 0)].solid)
				pp = Posf(pacman.gx + 2, pacman.gy);
			else
				pp = Posf(pacman.gx + 1, pacman.gy);
		else
			pp = Posf(pacman.gx, pacman.gy);
		break;
	case DOWN:
		if (!map[(gx + 0) + w * (gy + 1)].solid)
			if (!map[(gx + 0) + w * (gy + 2)].solid)
				pp = Posf(pacman.gx, pacman.gy + 2);
			else
				pp = Posf(pacman.gx, pacman.gy + 1);
		else
			pp = Posf(pacman.gx, pacman.gy);
		break;
	case LEFT:
		if (!map[(gx - 1) + w * (gy - 0)].solid)
			if (!map[(gx - 2) + w * (gy - 0)].solid)
				pp = Posf(pacman.gx - 2, pacman.gy);
			else
				pp = Posf(pacman.gx - 1, pacman.gy);
		else
			pp = Posf(pacman.gx, pacman.gy);
		break;
	case NONE:
		pp = Posf(pacman.gx, pacman.gy);
		break;
	}

	target = pp;
	
}

bool BlueGhost::Update(Tile* map, float ElapsedTime, PacMan& pacman, RedGhost r, PinkGhost p) {

	switch (state) { //UPDATE GHOSTS BASED ON THEIR STATE
	case INBOX:
		InBoxTimer += ElapsedTime;
		if (InBoxTimer >= TimeToGo) { //GHOST IS SET OUT TO HUNT
			state = CHASE;
			soll = NONE;
			dir = NONE;
			y = GhostStartY - 3; //MOVE GHOST OUTSIDE THE BOX
			gy = GhostStartY - 3; //MOVE GHOST OUTSIDE THE BOX
		}
		break;
	case CHASE:
		GTimer += ElapsedTime;
		if (GTimer > 15) { // AFTER 15 SECONDS LEAVE THE PLAYER
			GTimer = 0;
			state = SCATTER;
		}
		Algorithm(ElapsedTime, map, pacman, r, p); // UPDATE THE TARGET POS USING REDGHOSTS POS AND PINKGHOSTS ALGORITHM
		break;
	case SCATTER:
		GTimer += ElapsedTime;
		if (pacman.lvl < 5) { //BASED ON PACMAN LVL DECREASE THE TIME IN SCATTER MODE
			if (GTimer > 6) {
				GTimer = 0;
				state = CHASE;
			}
		}
		else  if (pacman.lvl < 10) { //BASED ON PACMAN LVL DECREASE THE TIME IN SCATTER MODE
			if (GTimer > 3) {
				GTimer = 0;
				state = CHASE;
			}
		}
		else {
			GTimer = 0;
			state = CHASE;
		}
		target = frightend; //TARGETTILE IS THE FRIGHTEND POS
		break;
	case FRIGHTEND:
		GTimer += ElapsedTime;
		if (GTimer > 2) 
			PlayNewSound(L"play");
		if (pacman.lvl < 5) { //BASED ON PACMAN LVL DECREASE THE TIME IN FRIGHTEND MODE
			if (GTimer > 6) {
				GTimer = 0;
				state = CHASE;
				pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
			}
		}
		else  if (pacman.lvl < 10) { //BASED ON PACMAN LVL DECREASE THE TIME IN FRIGHTEND MODE
			if (GTimer > 3) {
				GTimer = 0;
				state = CHASE;
				pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
			}
		}
		else {
			GTimer = 0;
			state = CHASE;
			pacman.GhostScore = 100; //AFTER GOING OUT OF FRIGHTEND MODE RESET THE POINTS THE PLAYER CAN GET FOR EATING GHOSTS
		}
		target = frightend; //TARGETTILE IS THE FRIGHTEND POS
		break;
	case RETURNING:
		GTimer += ElapsedTime;
		if (Pos(GhostStartX, GhostStartY - 3) == Pos(gx, gy) || GTimer > 20) //IF THE GHOST IS BACK REVIVE HIM
			Revive();

		target = Posf(GhostStartX, GhostStartY - 3);  //TARGETTILE IS THE HOME POS
		break;
	}

	if (Dead) { //GHOST IS DEAD SO SET THE STATE TO RETURNING
		GTimer = 0;
		state = RETURNING;
	}
	if (map[gx + w * gy].corner || dir == NONE) //ONLY IF THE GHOST IS ON A CORNER OR NOT MOVING UPDATE THE ALGORITHM POS UPDATE (SAVE COMPUTING POWER)
		AlgPosUpdate(target, map);

	switch (state) { //UPDATE MOVEMENT BASED ON STATE EQUALING VELOCITIES FROM BASE CLASS ENTITY
	case FRIGHTEND:
		Entity::Update(map, ElapsedTime, 0.5, true);
		break;
	case RETURNING:
		Entity::Update(map, ElapsedTime, GhostSpeed * 2, true);
		break;
	default:
		if (GTimer > 10 && IsInRange(RED_START_X, RED_START_Y, 2)) { //IF GHOST BUGGED OUT FORCE HIM OUT AFTER 10 SECONDS
			y = GhostStartY - 3;
			gy = GhostStartY - 3;
		}
		Entity::Update(map, ElapsedTime, GhostSpeed + (pacman.lvl > 10 ? 0.3 : 0), true); //AFTER LVL 10 THE GHOSTS ARE PARTIALY FASTER THAN PACMAN
		break;
	}

	return IsInRange(pacman.x, pacman.y, 0.8); //RETURN IF THE GHOST IS OVERLAPPING WITH PACMAN
}

void BlueGhost::Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman, RedGhost r, PinkGhost p) {
	//SETS THE TARGET TILE TO DOUBLE THE LENGTH OF THE VECTOR OF THE PINKGHOST ALGORITHM GOING THROUGH THE POS OF THE RED GHOST

	p.Algorithm(ElapsedTime, map, pacman);

	Vec2 v(Posf(r.gx, r.gy), target);

	v *= 2;

	target = Posf(v.x + x, v.y + y);
}


void GreenGhost::Algorithm(float ElapsedTime, Tile* map, const PacMan& pacman) {
	//SETS THE TARGET TILE TO PACMANS POS WHILE BEING MORE THAN 8 AWAY AFTER THAT RUN BACK TO THE SCATTER POS

	if (Vec2(Posf(pacman.gx, pacman.gy), Posf(gx, gy)).length() > 8)
		target = Posf(pacman.gx, pacman.gy);
	else
		target = Posf(GREEN_SCATTER_POS);

}
