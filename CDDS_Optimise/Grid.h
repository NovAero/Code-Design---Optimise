#pragma once
#include "Critter.h"
#include <vector>

class Grid
{
public:

	Critter* destroyer;

private:

	static const int NUM_CELLS = 8;
	static const int CELL_SIZE = 100;

	struct Cell {

		Vector2 GetGridXY(int index) {
			return Vector2{ (float)(index % NUM_CELLS), (float)(floor(index / NUM_CELLS)) };
		}

		int width;
		Vector2 pos;

		std::vector<Critter*> containedCritters;

		void Draw(int posX, int posY);

		Cell(int index) {
			width = CELL_SIZE;
			pos = GetGridXY(index);
		}
	};


	std::vector<Cell*> cells_;

	int MAX_VELOCITY = 80;

public:
	Grid();
	~Grid();

	//Add a critter to the grid, will automatically determine which cell it fits into
	void Add(Critter* toAdd);
	void CheckContents(Cell* cell);
	
	void HandleCollisions();
	void HandleDestroyerCell(Cell* cell);
	void HandleBounceCell(Cell* cell);

	//Get index on the 1D array with two ints x and y
	int GetIndex(int x, int y);
	//Get index on the 1D array with vector
	int GetIndex(Vector2 pos);
	//Get coords on the grid
	Vector2 GetGridXY(int index);
	//Get coords in screen space
	Vector2 GetWorldXY(int index);

	void Update(float delta);
	void Draw();

};