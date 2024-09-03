#pragma once
#include "Grid.h"
#include "Critter.h"

Grid::Grid()
{
    int* ptr = const_cast<int*>(&CELL_SIZE);
    *ptr = (GameManager::GetGameManager().ScreenSpace() / NUM_CELLS);
    delete ptr;

    // Clear the grid.
    for (int x = 0; x < NUM_CELLS; x++)
    {
        for (int y = 0; y < NUM_CELLS; y++)
        {
            cells_[x][y] = NULL;
        }
    }
}

void Grid::Add(Critter* unit)
{
    // Determine which grid cell it's in.
    int cellX = (int)(unit->m_position.x / Grid::CELL_SIZE);
    int cellY = (int)(unit->m_position.y / Grid::CELL_SIZE);

    // Add to the front of list for the cell it's in.
    unit->prev_ = NULL;
    unit->next_ = cells_[cellX][cellY];
    cells_[cellX][cellY] = unit;

    if (unit->next_ != NULL)
    {
        unit->next_->prev_ = unit;
    }
}

void Grid::HandleCollision()
{
    for (int x = 0; x < NUM_CELLS; x++)
    {
        for (int y = 0; y < NUM_CELLS; y++)
        {
            HandleCell(cells_[x][y]);
        }
    }
}

void Grid::HandleCell(Critter* unit)
{
    while (unit != NULL)
    {
        Critter* other = unit->next_;
        while (other != NULL)
        {
            if (unit->m_position.x == other->m_position.x &&
                unit->m_position.y == other->m_position.y)
            {
                HandleAttack(unit, other);
            }
            other = other->next_;
        }

        unit = unit->next_;
    }
}

void Grid::Move(Critter* unit, double x, double y)
{
    // See which cell it was in.
    int oldCellX = (int)(unit->m_position.x / Grid::CELL_SIZE);
    int oldCellY = (int)(unit->m_position.y / Grid::CELL_SIZE);

    // See which cell it's moving to.
    int cellX = (int)(x / Grid::CELL_SIZE);
    int cellY = (int)(y / Grid::CELL_SIZE);

    unit->m_position.x = x;
    unit->m_position.y = y;

    // If it didn't change cells, we're done.
    if (oldCellX == cellX && oldCellY == cellY) return;

    // Unlink it from the list of its old cell.
    if (unit->prev_ != NULL)
    {
        unit->prev_->next_ = unit->next_;
    }

    if (unit->next_ != NULL)
    {
        unit->next_->prev_ = unit->prev_;
    }

    // If it's the head of a list, remove it.
    if (cells_[oldCellX][oldCellY] == unit)
    {
        cells_[oldCellX][oldCellY] = unit->next_;
    }

    // Add it back to the grid at its new cell.
    Add(unit);
}
