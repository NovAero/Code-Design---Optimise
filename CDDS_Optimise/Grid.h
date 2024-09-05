#pragma once
#include "Critter.h"

class Grid
{
public:
    Grid()
    {
        // Clear the grid.
        for (int x = 0; x < NUM_CELLS; x++)
        {
            for (int y = 0; y < NUM_CELLS; y++)
            {
                cells_[x][y] = NULL;
            }
        }
    }

    static const int NUM_CELLS = 16;
    static const int CELL_SIZE = 20;
private:
    Critter* cells_[NUM_CELLS][NUM_CELLS];
};