#include <cassert>
#include "go.h"
#pragma once

class Tests {
private:
	void placeStones0() {
		Game game = Game::Game(5);
		game.placeStone(1, 0);
		game.placeStone(1, 0);
		game.placeStone(0, 1);
		assert(game.getBoard()->get(0, 1) == WHITE_STATE);
	}
	void liberties0() {
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
public:
	void runTests() {
		placeStones0();
		liberties0();
	}
};