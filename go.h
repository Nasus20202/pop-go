#pragma once
#define DEFAULT_SIZE 19
#define MAX_SIZE 2500
// Board states naming
#define WHITE_STATE 'w'
#define BLACK_STATE 'b'
#define EMPTY_STATE 'e'

class Board {
private:
	int size;
	char* boardData; // the board is a 2D array of chars (char pointer to size*size chars)
	void recursiveLiberties(const int x, const int y, char color, char* visited); // Recursive function for counting liberties
public:
	Board(int size = DEFAULT_SIZE);
	~Board();
	Board& operator=(const Board& other); // Copy constructor
	char* operator[](const int index); // Alows to access the board as a 2D array
	bool operator==(const Board& rightBoard); // Compares two boards
	
	char get(const int row, const int col); // Get the value of a field on the board (row, column)
	void set(const int row, const int col, const char c); // Set the value of a field on the board (row, column, value)
	int getSize();
	int countLiberties(const int x, const int y); // Count the liberties of a stone, if stone doesn't exits return -1
	int* generateLibertiesMatrix(); // Count liberties of every stone, -1 if no stone
	int* generateGroupLibertiesMatrix(const int x, const int y); // Count liberties of every stone in group, -1 if no stone
	char* toString(); // Convert board to string
};

class Game {
private:
	Board board, previousBoard;
	double whitePoints, blackPoints; int move;
	bool isBlacksTurn; // Black always go first
	void removeAllDeadStones(); // Remove all dead stones from the board
	void removeDeadNeighbours(const int x, const int y); // Remove dead stone touching the stone (x, y)
	void removeDeadStone(const int x, const int y); // Remove a stone and add points
public:
	Game(const int size = DEFAULT_SIZE);
	~Game();
	
	void newBoard(const int size = DEFAULT_SIZE);
	bool placeStone(const int x, const int y);
	bool checkIfLegalMove(const int x, const int y);

	char getCurrentPlayer();
	double getPoints(const char color);
	Board* getBoard();
	Board* getPreviousBoard();
	void setBoard(const Board &board);
	void setPreviousBoard(const Board& board);
};

