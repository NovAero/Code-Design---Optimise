#pragma once
#include "raylib.h"

class Critter
{
	friend class Grid;	
protected:	
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;

	Texture2D m_texture;

	Grid* grid_;

	Critter* prev_;
	Critter* next_;

	bool m_isLoaded;
	bool m_isDirty;// indicates if we've already processed a collision response for this critter

public:
	Vector2 posInGrid;
	
public:
	Critter();
	Critter(Grid* grid, Vector2 position, Vector2 velocity, float radius, Texture2D texture);
	~Critter();

	void Init(Grid* grid, Vector2 position, Vector2 velocity, float radius, Texture2D texture);
	void Destroy();
	void Update(float dt);
	void Draw();

	float GetX() { return m_position.x; } 
	float GetY() { return m_position.y; }
	void SetX(float x) { m_position.x = x; }
	void SetY(float y) { m_position.y = y; }

	void Move(Vector2 vec);

	Vector2 GetPosition() { return m_position; }
	void SetPosition(Vector2 position) { m_position = position; }

	Vector2 GetVelocity() { return m_velocity; }
	void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

	float GetRadius() { return m_radius; }

	bool IsDirty() { return m_isDirty; }
	void SetDirty() { m_isDirty = true; }

	bool IsDead() { return m_isLoaded == false; }
};

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
	
	void HandleCollision(Critter* unit, Critter* other);

	void Add(Critter* unit);
	void HandleCritters();

	float UnitDistance(Critter* from, Critter* to);

	void HandleCell(int x, int y);
	void HandleUnit(Critter* unit, Critter* other);

	void Move(Critter* unit, Vector2 vec);

	void Draw();

	static const int NUM_CELLS = 8;
	static const int CELL_SIZE = 100;

private:
	Critter* cells_[NUM_CELLS][NUM_CELLS];
};