#include "Grid.h"

Grid::Grid()
{
	for (int i = 0; i < NUM_CELLS * NUM_CELLS; ++i) {
		cells_.push_back(new Cell);
	}
}

Grid::~Grid()
{
	cells_.clear();
}



int Grid::GetIndex(int x, int y)
{
	return x + NUM_CELLS * y;
}

int Grid::GetIndex(Vector2 pos)
{
	return pos.x + NUM_CELLS * pos.y;
}

Vector2 Grid::GetGridXY(int index)
{
	return Vector2{(float)(index % NUM_CELLS), (float)(floor(index / NUM_CELLS)) };
}

Vector2 Grid::GetWorldXY(int index)
{
	return Vector2{ (float)(index % NUM_CELLS) * CELL_SIZE, (float)(floor(index / NUM_CELLS)) * CELL_SIZE }
}

void Grid::Draw()
{
	for (int i = 0; i < NUM_CELLS * NUM_CELLS; ++i) {
		cells_[i]->Draw((i % 8) * 100, (i / 8) * 100);
	}
}

//Cells Funcs

void Grid::Cell::Draw(int posX, int posY)
{
	DrawRectangleLines(posX, posY, 100, 100, RED);
}
