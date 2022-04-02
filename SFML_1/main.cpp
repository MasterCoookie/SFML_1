#include <iostream>
#include "Game.h"

int main() {
	// Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//init game engine
	Game game;

	// loop
	int count = 0;
	while (game.running() && !game.getEndGame()) {
		// polling

		// Update
		game.Update();

		// Render
		game.Render();
		//rendering game
	}

	return 0;
}