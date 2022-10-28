#pragma once
#define DEFAULT_SIZE 19

class Board {
private:
	int size;
	char* boardData; // the board is a 2D array of chars (char pointer to size*size chars)
public:
	Board(int size = DEFAULT_SIZE);
	~Board();
	Board& operator=(const Board& other);
	char get(const int row, const int col);
	void set(const int row, const int col, const char c);
	char* operator[](const int index);
	int getSize();
};

class Game {
private:
	Board board;
	bool isBlacksTrun; // Black always go first
public:
	Game(const int size = DEFAULT_SIZE);
	~Game();
};

