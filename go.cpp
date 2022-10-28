#include "go.h"
#include <stdlib.h>

#pragma region Board

	Board::Board(int size) {
		size = size < 2 ? 2 : size; size = size > 1000 ? 1000 : size; // size must be between 2 and 1000 (inclusive)
		this->size = size;
		boardData = (char*)malloc(size * size * sizeof(char)); // Allocate memory for the board
		if (boardData == NULL)
			exit(1);
		for (int i = 0; i < size * size; i++)
			boardData[i] = ' ';
	}

	Board::~Board() {
		free(boardData); // Free the memory allocated for board data
	}

	// Alows to copy one board to another
	Board& Board::operator=(const Board& other) {
		if (this == &other) // Check for self-assignment
			return *this;
		if (size != other.size) {
			free(boardData);
			size = other.size;
			boardData = (char*)malloc(size * size * sizeof(char)); // Allocate memory for the board
			if (boardData == NULL)
				exit(1);
		}
		for (int i = 0; i < size * size; i++)
			boardData[i] = other.boardData[i];
		return *this;
	}

	// Get the value of a field on the board (row, column)
	char Board::get(const int row, const int col) {
		return boardData[row * size + col];
	}

	// Set the value of a field on the board (row, column, value)
	void Board::set(const int row, const int col, const char c) {
		boardData[row * size + col] = c;
	}

	// Alows to access the board as a 2D array
	char* Board::operator[](const int index) {
		return &boardData[index * size];
	}
	
	int Board::getSize() {
		return size;
	}

#pragma endregion



#pragma region Game

	Game::Game(const int size) {
		board = Board::Board(size);
		isBlacksTrun = true;
	}
	Game::~Game() {

	}

#pragma endregion