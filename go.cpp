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

	// Count amout of free fields or same color stones around a stone
	int Board::countLibertiesNoChain(const int x, const int y) {
		char color = get(x, y);
		if (color == EMPTY_STATE)
			return -1;
		int liberties = 0;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if ((i == 0) != (j == 0)) { // Check only horizontal and vertical neighbours, != works as XOR operator for bools 
					int row = x + i, col = y + j;
					if (row < 0 || row > size - 1 || col < 0 || col > size - 1) // Check for overflow
						continue;
					if (get(row, col) == EMPTY_STATE)
						liberties++;
				}
		return liberties;
	}
	
	// Recursive function to count liberties in a chain
	int Board::countLibertiesInChain(const int x, const int y, const char color, Board* tempBoard, bool* visited, int liberties) {
		int size = tempBoard->getSize();
		if (x < 0 || x > size - 1 || y < 0 || y > size - 1) // Check for overflow
			return liberties;
		if (visited[x * size + y]) // If the field was already visited
			return liberties;
		visited[x * size + y] = true;
		const char fieldColor = get(x, y);
		if (fieldColor == EMPTY_STATE) // If the field is empty
			return liberties + 1;
		else if (fieldColor != color) // If the field is not the same color as the chain
			return liberties;
		int localLiberties = tempBoard->get(x, y);
		int neighbourLiberties = 0;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				if ((i == 0) != (j == 0)) { // Check only horizontal and vertical neighbours, != works as XOR operator for bools 
					int row = x + i, col = y + j;
					neighbourLiberties += countLibertiesInChain(row, col, color, tempBoard, visited, liberties);
				}
		if (neighbourLiberties > localLiberties)
			localLiberties = neighbourLiberties;
		if(localLiberties > liberties)
			liberties = localLiberties;
		return liberties;
	}
	
	// Count the liberties of a stone, if stone doesn't exits return -1
	int Board::countLiberties(const int x, const int y) {
		char color = get(x, y);
		if (color == EMPTY_STATE)
			return -1;			       // Initialize temporary board used to check for liberties in chains
		Board tempBoard = Board(size); // max amout of liberties is 4, which is less than capacity of char variable
		for (int i = 0; i < size; i++) // Calculate liberties of every stone of that color without chaining
			for (int j = 0; j < size; j++)
				if (get(i, j) == color) {
					tempBoard.set(i, j, countLibertiesNoChain(i, j));
				}
				else {
					tempBoard.set(i, j, 0);
				}
		bool* visited = (bool*) malloc(size * size * sizeof(bool)); // Allocate memory for the board
		if (visited == NULL)   // If the allocation failed
			exit(1);		   // end program with code 1
		for (int i = 0; i < size * size; i++)
			visited[i] = false;
		int liberties = countLibertiesInChain(x, y, color, &tempBoard, visited);
		free(visited);
		return liberties;
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