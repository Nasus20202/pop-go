#include "go.h"
#include <stdlib.h>

#pragma region Board

	Board::Board(int size) {
		size = size < 2 ? 2 : size; size = size > 1000 ? 1000 : size; // size must be between 2 and 1000 (inclusive)
		this->size = size;
		boardData = (char*)malloc(size * size * sizeof(char)); // Allocate memory for the board
		if (boardData == NULL) // If the allocation failed
			exit(1);		   // end program with code 1
		for (int i = 0; i < size * size; i++)
			boardData[i] = EMPTY_STATE;
	}

	Board::~Board() {
		free(boardData); // Free the memory allocated for board data
	}

	// Alows to copy one board to another
	Board& Board::operator=(const Board& other) {
		if (this == &other) // Check for self-assignment
			return *this;
		if (size != other.size) {
			free(boardData); // Free the memory allocated for old board data
			size = other.size;   // Change size of first board to size of the second one
			boardData = (char*)malloc(size * size * sizeof(char)); // Allocate memory for the board
			if (boardData == NULL) // If the allocation failed
				exit(1);		   // end program with code 1
		}
		for (int i = 0; i < size * size; i++)
			boardData[i] = other.boardData[i];
		return *this;
	}

	// Alows to access the board as a 2D array
	char* Board::operator[](const int index) 
	{
		return &boardData[(index % size) * size]; // Pointer to first element of row
	}
	
	// Compares two boards
	bool Board::operator==(const Board& rightBoard) {
		if (size != rightBoard.size)
			return false;
		for (int i = 0; i < size * size; i++)
			if (boardData[i] != rightBoard.boardData[i])
				return false;
		return true;
	}
	

	// Get the value of a field on the board (row, column)
	char Board::get(const int row, const int col) {
		int index = (row % size) * size + (col % size); // Calculate index of the field, check for overflow
		return boardData[index];
	}

	// Set the value of a field on the board (row, column, value)
	void Board::set(const int row, const int col, const char c) {
		int index = (row % size) * size + (col % size);
		boardData[index] = c;
	}
	
	int Board::getSize() {
		return size;
	}

#pragma endregion



#pragma region Game

	Game::Game(const int size) {
		board = Board::Board(size);
		isBlacksTurn = true;
	}
	
	Game::~Game() {

	}

	// Check if the move is legal (free space, inside board, has liberties)
	bool Game::checkIfLegalMove(const int x, const int y) {
		if (board.get(x, y) != EMPTY_STATE)
			return false;
		return true;
	}
	
	// Create and replace board
	void Game::newBoard(const int size) {
		board = Board::Board(size);
		isBlacksTurn = true;
	}

	// Place new stone with right color
	bool Game::placeStone(const int x, const int y){
		if (!checkIfLegalMove(x, y))
			return false;
		board.set(x, y, isBlacksTurn ? BLACK_STATE : WHITE_STATE);
		isBlacksTurn = !isBlacksTurn;
		return true;
	}

	// Get current player, true = black, false = white
	bool Game::getCurrentPlayer() {
		return isBlacksTurn;
	}

	// Get pointer to the board object
	Board* Game::getBoard() {
		return &board;
	}

	// Update board
	void Game::setBoard(const Board* board) {
		this->board = *board;
	}

#pragma endregion