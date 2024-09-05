#include "Critter.h"
#include "TextureManager.h"
#include "raymath.h"

constexpr float ATTACK_DISTANCE = 15.f;
const int MAX_VELOCITY = 80;

Critter::Critter()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isLoaded = false;
}

Critter::Critter(Grid* grid, Vector2 position, Vector2 velocity, float radius, Texture2D texture, bool destroyer) : Critter()
{
	Init(grid, position, velocity, radius, texture, destroyer);
}

Critter::~Critter()
{
	m_isLoaded = false;
}

void Critter::Init(Grid* grid, Vector2 position, Vector2 velocity, float radius, Texture2D texture, bool destroyer)
{
	IsDestroyer = destroyer;

	grid_ = grid;

	m_position = position;
	m_velocity = velocity;
	m_radius = radius;

	m_texture = texture;
	
	m_isLoaded = true;
	grid_->Add(this);
}

void Critter::Destroy()
{
	m_isLoaded = false;
}

void Critter::Update(float dt)
{
	if (m_isLoaded == false)
		return;

	Vector2 moveVector = { m_velocity.x * dt, m_velocity.y * dt };

	Move(moveVector);

	//m_position.x += m_velocity.x * dt;
	//m_position.y += m_velocity.y * dt;

	m_isDirty = false;
}

void Critter::Draw()
{
	if (m_isLoaded == false)
		return;

	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}

void Critter::Move(Vector2 vec)
{
	grid_->Move(this, vec);
}



//Grid Functions

void Grid::Add(Critter* unit)
{
	// Determine which grid cell it's in.
	int cellX = (int)(unit->m_position.x / Grid::CELL_SIZE);
	int cellY = (int)(unit->m_position.y / Grid::CELL_SIZE);

	// Add to the front of list for the cell it's in.

	if (cellX >= 8) cellX = 7; //Sinze the position can truncate UP, if x or y == 8 they need to be bumped down
	if (cellY >= 8) cellY = 7;
	if (cellX <= -1) cellX = 0;
	if (cellY <= -1) cellY = 0;

	unit->prev_ = NULL;
	unit->next_ = cells_[cellX][cellY];

	cells_[cellX][cellY] = unit;

	if (unit->next_ != nullptr)
	{
		unit->next_->prev_ = unit;
	}
}

void Grid::HandleCritters()
{
	for (int x = 0; x < NUM_CELLS; x++)
	{
		for (int y = 0; y < NUM_CELLS; y++)
		{
			HandleCell(x , y);
		}
	}
}

void Grid::HandleCollision(Critter* unit, Critter* other)
{
	if (unit->IsDestroyer == true && other->IsDestroyer == true) {
		return;
	}

	if (unit->IsDestroyer == true) {
		other->Destroy();
		// this would be the perfect time to put the critter into an object pool
	}
	else if (other->IsDestroyer == true) {
		unit->Destroy();
	}
	else {
		// collision!
				// do math to get critters bouncing
		Vector2 normal = Vector2Normalize(Vector2Subtract(unit->GetPosition(), other->GetPosition()));

		// not even close to real physics, but fine for our needs
		unit->SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
		// set the critter to *dirty* so we know not to process any more collisions on it
		unit->SetDirty();

		// we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
		// to make sure the other critter is clean before we do the collision response
		if (!other->IsDirty()) {
			other->SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
			other->SetDirty();
		}
	}
}

float Grid::UnitDistance(Critter* from, Critter* to)
{
	float distance = Vector2Distance(from->GetPosition(), to->GetPosition());
	return distance;
}

void Grid::HandleCell(int x, int y)
{
	Critter* unit = cells_[x][y];
	while (unit != NULL)
	{
		// Handle other units in this cell.
		HandleUnit(unit, unit->next_);

		// Also try the neighboring cells.
		if (x > 0 && y > 0) HandleUnit(unit, cells_[x - 1][y - 1]);
		if (x > 0) HandleUnit(unit, cells_[x - 1][y]);
		if (y > 0) HandleUnit(unit, cells_[x][y - 1]);
		if (x > 0 && y < NUM_CELLS - 1)
		{
			HandleUnit(unit, cells_[x - 1][y + 1]);
		}
		unit = unit->next_;
	}
}

void Grid::HandleUnit(Critter* unit, Critter* other)
{
	while (other != NULL)
	{

		if (UnitDistance(unit, other) < ATTACK_DISTANCE)
		{
			HandleCollision(unit, other);
		}

		other = other->next_;

		if (other != NULL) {
			if (other == other->next_) {
				other->next_ = NULL; //object pointing to itself
			}
			else if (other->next_ == other->prev_ && other->next_ != NULL) {
				other->next_ = NULL; //If there are 2 objects pointing to eachother's backs, circle reference
			}
		}
	}
}

void Grid::Move(Critter* unit, Vector2 vec)
{
	float x = vec.x;
	float y = vec.y;

	// See which cell it was in.
	int oldCellX = (int)(unit->m_position.x / Grid::CELL_SIZE);
	int oldCellY = (int)(unit->m_position.y / Grid::CELL_SIZE);

	// See which cell it's moving to.
	int cellX = (int)(x / Grid::CELL_SIZE);
	int cellY = (int)(y / Grid::CELL_SIZE);
	
	unit->m_position.x += x;
	unit->m_position.y += y;

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

void Grid::Draw()
{
	for (int i = 0; i < NUM_CELLS*CELL_SIZE; i+=CELL_SIZE) {
		for (int j = 0; j < NUM_CELLS*CELL_SIZE; j+=CELL_SIZE) {
			DrawRectangleLines(i,j,CELL_SIZE,CELL_SIZE, RED);
		}
	}
}
