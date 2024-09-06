#include "Grid.h"
#include <string>
#include "raymath.h"

Grid::Grid()
{
	for (int i = 0; i < NUM_CELLS * NUM_CELLS; ++i) {
		cells_.push_back(new Cell(i));
	}
}

Grid::~Grid()
{

	cells_.clear();

	delete destroyer;
}

void Grid::Add(Critter* toAdd)
{
	if (toAdd == nullptr) return; 

	int index = GetIndex(toAdd->GetPosition());
	toAdd->oldCell = index;

	if (index >= NUM_CELLS*NUM_CELLS) {
		index = (NUM_CELLS*NUM_CELLS)-1;
	}
	if (index <= -1) {
		index = 0;
	}

	cells_[index]->containedCritters.push_back(toAdd);
}

void Grid::CheckContents(Cell* cell)
{
	for (int i = 0; i < cell->containedCritters.size(); ++i) {
		Vector2 newPosition = cell->containedCritters[i]->GetPosition();

		int newCell = GetIndex(newPosition);

		//if it hasnt moved cells, do nothing
		if (cell->containedCritters[i]->oldCell == newCell) {
			++i;
			continue;
		}
		if (newCell >= cells_.size()) {
			++i;
			continue;
		}
		cell->containedCritters[i]->oldCell = newCell;
		
		cells_[newCell]->containedCritters.push_back(cell->containedCritters[i]);

		Critter* d = cell->containedCritters[i];
		auto it = std::find(cell->containedCritters.begin(), cell->containedCritters.end(),	d);
		// If element is found found, erase it 
		if (it != cell->containedCritters.end()) {
			cell->containedCritters.erase(it);
		}
	}
}

void Grid::HandleCollisions()
{
	int destroyerCell = GetIndex(destroyer->GetPosition());
	
	if (destroyerCell >= NUM_CELLS * NUM_CELLS) {
		destroyerCell = (NUM_CELLS * NUM_CELLS) - 1;
	}
	if (destroyerCell <= -1) {
		destroyerCell = 0;
	}

	HandleDestroyerCell(cells_[destroyerCell]);

	for (auto it : cells_) {
		HandleBounceCell(it);
	}
}

void Grid::HandleDestroyerCell(Cell* cell)
{
	//Check if any critters are in radius to be destroyed
	for (int it = 0; it < cell->containedCritters.size(); ++it) {
		if (cell->containedCritters[it] == destroyer) continue;

		float dist = Vector2Distance(cell->containedCritters[it]->GetPosition(), destroyer->GetPosition());
		if (dist < cell->containedCritters[it]->GetRadius() + destroyer->GetRadius())
		{
			cell->containedCritters[it]->Destroy();
		}
	}
}

void Grid::HandleBounceCell(Cell* cell)
{
	for (int i = 0; i < cell->containedCritters.size(); i++)
	{
		for (int j = 0; j < cell->containedCritters.size(); j++) {
			if (cell->containedCritters[i]->IsDestroyer == true || cell->containedCritters[j]->IsDestroyer == true) continue;
			if (i == j || cell->containedCritters[i]->IsDirty()) // note: the other critter (j) could be dirty - that's OK
				continue;
			// check every critter against every other critter
			float dist = Vector2Distance(cell->containedCritters[i]->GetPosition(), cell->containedCritters[j]->GetPosition());
			if (dist < cell->containedCritters[i]->GetRadius() + cell->containedCritters[j]->GetRadius())
			{
				// collision!
				// do math to get critters bouncing
				Vector2 normal = Vector2Normalize(Vector2Subtract(cell->containedCritters[j]->GetPosition(), cell->containedCritters[i]->GetPosition()));

				// not even close to real physics, but fine for our needs
				cell->containedCritters[i]->SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
				// set the critter to *dirty* so we know not to process any more collisions on it
				cell->containedCritters[i]->SetDirty();

				// we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
				// to make sure the other critter is clean before we do the collision response
				if (!cell->containedCritters[j]->IsDirty()) {
					cell->containedCritters[j]->SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
					cell->containedCritters[j]->SetDirty();
				}
				break;
			}
		}
	}
}

int Grid::GetIndex(int x, int y)
{
	return x + NUM_CELLS * y;
}

int Grid::GetIndex(Vector2 pos)
{
	int x = pos.x /= CELL_SIZE;
	int y = pos.y /= CELL_SIZE;
	return x + NUM_CELLS * y;
}

Vector2 Grid::GetGridXY(int index)
{
	return Vector2{(float)(index % NUM_CELLS), (float)(floor(index / NUM_CELLS)) };
}

Vector2 Grid::GetWorldXY(int index)
{
	return Vector2{(float)(index % NUM_CELLS) * CELL_SIZE, (float)(floor(index / NUM_CELLS)) * CELL_SIZE };
}

void Grid::Update(float delta)
{
	for (int i = 0; i<cells_.size(); i++) {
		if (cells_[i]->containedCritters.empty()) continue;
		CheckContents(cells_[i]);
	}
}

void Grid::Draw()
{
	for (int i = 0; i < NUM_CELLS * NUM_CELLS; ++i) {
		cells_[i]->Draw(GetWorldXY(i).x, GetWorldXY(i).y);
	}
}

//Cells Funcs

void Grid::Cell::Draw(int posX, int posY)
{
	DrawRectangleLines(posX, posY, 100, 100, RED);

	int s = containedCritters.size();

	std::string size = std::to_string(s);

	DrawText(size.c_str(), posX+50, posY+50, 20, DARKGREEN);
}
