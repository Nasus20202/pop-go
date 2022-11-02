#include <cassert>
#include "go.h"
#include "tests.h"

void Tests::placeStones0() {
	Game game = Game::Game(5);
	game.placeStone(1, 0);
	game.placeStone(1, 0);
	game.placeStone(0, 1);
	assert(game.getBoard()->get(0, 1) == WHITE_STATE);
}

void Tests::liberties0() {
	Game game = Game::Game(10);
	Board board = Board::Board(10);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++)
			board.set(i, j, WHITE_STATE);
	}
	board.set(1, 1, BLACK_STATE);
	board.set(1, 2, BLACK_STATE);
	board.set(1, 3, BLACK_STATE);
	board.set(2, 3, BLACK_STATE);
	board.set(1, 2, BLACK_STATE);
	board.set(2, 2, EMPTY_STATE);
	board.set(2, 4, EMPTY_STATE);
	board.set(3, 1, EMPTY_STATE);
	game.setBoard(&board);
	bool firstMove = game.placeStone(3, 1);
	int firstLiberties = game.getBoard()->countLiberties(1, 2);
	bool secondMove = game.placeStone(2, 2);
	char firstColor = game.getBoard()->get(2, 2);
	int secondLiberties = game.getBoard()->countLiberties(1, 2);
	game.placeStone(2, 9);
	game.placeStone(2, 4);
	game.placeStone(2, 8);
	game.placeStone(2, 5);
	int thirdLiberties = game.getBoard()->countLiberties(1, 2);
	assert(firstMove == false && firstLiberties == 2 && secondMove == true && firstColor == BLACK_STATE && secondLiberties == 1 && thirdLiberties == 3);

}
void Tests::liberties1() {
	Board board = Board::Board(5);
	for (int i = 1; i < 5; i++) {
		board.set(2, i, WHITE_STATE);
	}
	board.set(3, 3, WHITE_STATE);
	int liberties = board.countLiberties(2, 2);
	assert(liberties == 9);
}
void Tests::liberties2() {
	Board board = Board::Board(6);
	board.set(3, 4, BLACK_STATE);
	board.set(2, 3, WHITE_STATE);
	board.set(1, 3, WHITE_STATE);
	board.set(2, 4, WHITE_STATE);
	int liberties = board.countLiberties(2, 3);
	assert(liberties == 6);
}
void Tests::liberties3() {
	Board board = Board::Board(5);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			board.set(i, j, BLACK_STATE);
		}
	}
	board.set(1, 1, WHITE_STATE);
	int firstLiberties = board.countLiberties(1, 1);
	board.set(1, 2, WHITE_STATE);
	int secondLiberties = board.countLiberties(1, 1);
	board.set(1, 3, WHITE_STATE);
	int thirdLiberties = board.countLiberties(1, 1);
	assert(firstLiberties == 0 && secondLiberties == 1 && thirdLiberties == 3);
}

void Tests::capture0() {
	Game game = Game::Game(7);
	Board board = Board::Board(7);
	board.set(0, 1, WHITE_STATE); board.set(1, 0, WHITE_STATE); board.set(2, 1, WHITE_STATE); board.set(0, 2, WHITE_STATE); board.set(2, 2, WHITE_STATE); board.set(5, 2, WHITE_STATE); board.set(6, 3, WHITE_STATE);
	game.setBoard(&board);
	game.placeStone(1, 1);
	game.placeStone(4, 3);
	game.placeStone(1, 2);
	game.placeStone(1, 3);
	game.placeStone(5, 3);
	game.placeStone(5, 4);
	char state0 = game.getBoard()->get(1, 1);
	char state1 = game.getBoard()->get(1, 2);
	char state2 = game.getBoard()->get(5, 3);
	assert(state0 == EMPTY_STATE && state1 == EMPTY_STATE && state2 == EMPTY_STATE);
}

void Tests::koRule0() {
	Game game = Game::Game(7);
	Board board = Board::Board(7);
	board.set(0, 1, WHITE_STATE); board.set(1, 0, WHITE_STATE); board.set(2, 1, WHITE_STATE); board.set(0, 2, BLACK_STATE); board.set(1, 3, BLACK_STATE); board.set(2, 2, BLACK_STATE);
	game.setBoard(&board);
	game.placeStone(1, 1);
	game.placeStone(1, 2);
	bool firstMove = game.placeStone(1, 1);
	char firstState = game.getBoard()->get(1, 1);
	char player = game.getCurrentPlayer();
	game.placeStone(5, 5);
	game.placeStone(6, 5);
	bool secondMove = game.placeStone(1, 1);
	char secondState = game.getBoard()->get(1, 2);
	bool thirdMove = game.placeStone(1, 2);
	char thirdState = game.getBoard()->get(1, 2);
	assert(firstState == EMPTY_STATE && firstMove == false && player == BLACK_STATE && secondMove == true && secondState == EMPTY_STATE && thirdMove == false && thirdState == EMPTY_STATE);
}

void Tests::runTests() {
	placeStones0();
	liberties0();
	liberties1();
	liberties2();
	liberties3();
	capture0();
	koRule0();
}