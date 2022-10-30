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

	// Recursive function for counting liberties, marks every liberty as 1 in visited array
	void Board::recursiveLiberties(const int x, const int y, char color, char* visited) {
		int size = getSize(); int index = x * size + y;
		if (x < 0 || x > size - 1 || y < 0 || y > size - 1) // Check if the field is on the board
			return;
		if (visited[index] != 0)
			return;
		char type = get(x, y);
		if (type == EMPTY_STATE) {
			visited[index] = 1; return; // It's a liberty
		}
		else if (type == color) {
			visited[index] = 2; // Run recursive function for all neighbours
		}
		else {
			visited[index] = -1; return; // Dead end - oposite color
		}
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++)
				if ((i == 0) != (j == 0)) {
					int row = x + i, col = y + j;
					recursiveLiberties(row, col, color, visited);
				}
		}
	}
	
	// Count the liberties of a stone, if stone doesn't exits return -1
	int Board::countLiberties(const int x, const int y) {
		char color = get(x, y);
		if (color == EMPTY_STATE)
			return -1;
		char* visited = (char*)malloc(size * size * sizeof(char)); // Allocate memory for visited array
		if (visited == NULL) // If the allocation failed
			exit(1);
		for (int i = 0; i < size * size; i++) // 0 - unvisited, -1 - other color, 1 - free, 2 - same color;
			visited[i] = 0;
		recursiveLiberties(x, y, color, visited); int liberties = 0;
		for (int i = 0; i < size * size; i++) {
			if (visited[i] == 1)
				liberties++;
		}
		free(visited);
		return liberties;
	}

	// Returs pointer to size*size array of integers representing number of liberties of every stone, -1 is empty
	int* Board::generateLibertiesMatrix() {
		int* liberties = (int*)malloc(size * size * sizeof(int)); // Allocate memory for liberties array
		if (liberties == NULL) // If the allocation failed
			exit(1);
		for (int i = 0; i < size * size; i++) // -2 - not visited, -1 - no stone, 0+ - number of stone's liberties
			liberties[i] = -2;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int index = i * size + j;
				if (liberties[index] != -2)
					continue;
				if (get(i, j) == EMPTY_STATE)
					liberties[index] = -1;
				char* visited = (char*)malloc(size * size * sizeof(char)); // Allocate memory for visited array
				if (visited == NULL) // If the allocation failed
					exit(1);
				for (int i = 0; i < size * size; i++) // 0 - unvisited, -1 - other color, 1 - free, 2 - same color;
					visited[i] = 0;
				recursiveLiberties(i, j, get(i, j), visited); int libertiesCount = 0;
				for (int i = 0; i < size * size; i++) {
					int a = visited[i];
					if (visited[i] == 1)
						libertiesCount++;
				}
				for (int i = 0; i < size * size; i++) {
					if (visited[i] == 2)
						liberties[i] = libertiesCount;
				}
				free(visited);
			}
		}
		return liberties;
	}

#pragma endregion

#pragma region Game

	Game::Game(const int size) {
		board = Board::Board(size);
		isBlacksTurn = true;
		whitePoints = 0, blackPoints = 0;
	}
	
	Game::~Game() {

	}

	// Check if the move is legal (free space, inside board, has liberties)
	bool Game::checkIfLegalMove(const int x, const int y) {
		int size = board.getSize();
		if (x < 0 || x > size - 1 || y < 0 || y > size - 1) // Check for overflow
			return false;
		if (board.get(x, y) != EMPTY_STATE)
			return false;
		Board tempBoard = Board::Board(size);
		tempBoard = board; // Just to have a new pointer
		char color = isBlacksTurn ? BLACK_STATE : WHITE_STATE;
		tempBoard.set(x, y, color);
		if (tempBoard.countLiberties(x, y) == 0)
			return false;
		return true;
	}
	
	// Create and replace board
	void Game::newBoard(const int size) {
		board = Board::Board(size);
		isBlacksTurn = true;
		whitePoints = 0, blackPoints = 0;
	}

	// Clear the board out of all dead stones
	void Game::removeAllDeadStones() {
		int* liberties = board.generateLibertiesMatrix(); int size = board.getSize();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int index = i * size + j;
				if (liberties[index] == -1)
					continue;
				if (liberties[index] == 0)
					removeDeadStone(i, j);
			}
		}
		free(liberties);
	}

	// Remove stone and add points
	void Game::removeDeadStone(const int x, const int y) {
		char color = board.get(x, y);
		if (color == WHITE_STATE)
			blackPoints++;
		else if (color == BLACK_STATE)
			whitePoints++;
		board.set(x, y, EMPTY_STATE);
	}

	// Place new stone with right color
	bool Game::placeStone(const int x, const int y){
		if (!checkIfLegalMove(x, y))
			return false;
		board.set(x, y, isBlacksTurn ? BLACK_STATE : WHITE_STATE);
		isBlacksTurn = !isBlacksTurn;
		removeAllDeadStones();
		return true;
	}

	// Get current player, true = black, false = white
	bool Game::getCurrentPlayer() {
		return isBlacksTurn;
	}

	// Returns points of one player
	int Game::getPoints(const char color) {
		if (color == WHITE_STATE)
			return whitePoints;
		if (color == BLACK_STATE)
			return blackPoints;
		return 0;
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