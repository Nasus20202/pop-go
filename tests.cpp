#include <cassert>
#include "go.h"

class Tests {
private:
	void placeStones() {
		Game game = Game::Game(DEFAULT_SIZE);
		game.placeStone(1, 0);
		game.placeStone(1, 0);
		game.placeStone(0, 1);
		assert(game.getBoard()->get(0, 1) == WHITE_STATE);
	}
public:
	void runTests() {
		placeStones();
	}
};