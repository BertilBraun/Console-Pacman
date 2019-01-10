#include "Fruit.h"

Fruit::Fruit(ConsoleGameEngine* c) : c(c) {
	Revive(2); // CREATE 2 NEW FRUITS
}

Fruit::~Fruit() {
	delete s;
}

void Fruit::Update(float ElpasedTime, PacMan& pm) {
	Timer += ElpasedTime;

	if (uses) {
		if (!(rand() % 10) || Timer > 30) //BASED ON RANDOM OR A MAXIMUM TIME LET A FRUIT BE INTERACTED WITH
			interactable = true;

		if (interactable && pm.IsInRange(w / 2 - 0.5, h / 2 + 2, 0.8)) { // FRUIT IS GETTING USED
			switch (i) { //BASED ON ID APPLY EFFECTS
			case 0:
				if (pm.Health < 3)
					pm.Health++;
				break;
			case 1:
				pm.roundScore += 500;
				break;
			case 2:
				pm.roundScore += 500;
				break;
			}
			Revive(uses - 1); // CREATE NEW FRUIT
		}
	}
}

void Fruit::Revive(int use) {
	i = rand() % 3;
	s = new Sprite(L"res/enviroment/Fruits/" + std::to_wstring(i) + L".spr");
	uses = use;
	Timer = 0;
	interactable = false;
}

void Fruit::DrawSelf() {
	//FRUIT CAN BE INTERACTED WITH SO DISPLAY IT
	if (interactable && uses)
		c->DrawSprite(toMap((w / 2) - 0.5, (h / 2) + 2), s);

	//DRAW FRUITS IN LOWER LEFT CORNER
	for (int i = 0; i < uses - interactable; i++)
		c->DrawSprite(45 + (i * 13), c->ScreenHeight() - 15, s);
}