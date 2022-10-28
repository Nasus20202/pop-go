#include "go.h"
#include <stdlib.h>
#include <iostream>

Board::Board(int size) {
	size = size < 2 ? 2 : size; size = size > 1000 ? 1000: size; // size must be between 2 and 1000 (inclusive)
	this->size = size;
	boardData = (char*)malloc(size * size * sizeof(char)); // Allocate memory for the board
	if (boardData == NULL)
		exit(1);
	for (int i = 0; i < size * size; i++)
		boardData[i] = 'a';
}

Board::~Board() {
	free(boardData);
}

Board& Board::operator=(const Board& other) {
	return *this;
}

Game::Game(int size) {
	board = Board::Board(size);
	isBlacksTrun = true;
}
Game::~Game() {

}