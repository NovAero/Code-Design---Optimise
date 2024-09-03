#pragma once
#include "GameManager.h"

class Critter;

class Grid {
public:
	Grid();

	void Add(Critter* unit);

	void HandleCollision();

	void HandleCell(Critter* unit);

	void Move(Critter* unit, double x, double y);

	static const int NUM_CELLS = 10;
	static const int CELL_SIZE;

private:
    
    Critter* cells_[NUM_CELLS][NUM_CELLS];

};