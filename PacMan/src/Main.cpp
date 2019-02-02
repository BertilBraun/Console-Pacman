#include "Game.h"

int main()
{
	Game game;

	if (game.ConstructConsole(228, 290, 2, 2))
		game.Start();
	else {
    std::cout << "Failed To Initialze!\n"; 
    System("Pause");
  }
}
