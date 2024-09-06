#pragma once
#include "Critter.h"
#include <vector>

class Grid
{
private:

	static const int NUM_CELLS = 8;
	static const int CELL_SIZE = 100;

	struct Cell {

		std::vector<Critter*> containedCritters;


		void Draw(int posX, int posY);
	};

	std::vector<Cell*> cells_;


public:
	Grid();
	~Grid();

	void Add(Critter* toAdd);

	//Get index on the 1D array with two ints x and y
	int GetIndex(int x, int y);
	//Get index on the 1D array with vector
	int GetIndex(Vector2 pos);
	//Get coords on the grid
	Vector2 GetGridXY(int index);
	//Get coords in screen space
	Vector2 GetWorldXY(int index);

	void Draw();

};