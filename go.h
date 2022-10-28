#pragma once
# define DEFAULT_SIZE 19

class Board {
private:
	int size;
	char* boardData; // the board is a 2D array of chars
public:
	Board(int size = DEFAULT_SIZE);
	~Board();
	Board& operator=(const Board& other);
};

class Game {
private:
	Board board;
	bool isBlacksTrun; // Black always go first
public:
	Game(int size = DEFAULT_SIZE);
	~Game();
};

