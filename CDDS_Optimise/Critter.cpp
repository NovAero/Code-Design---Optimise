#include "Critter.h"
#include "TextureManager.h"
#include "Grid.h"

Critter::Critter() : grid_()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isLoaded = false;
}

Critter::~Critter()
{
	m_isLoaded = false;
}

void Critter::Init(Grid* grid, Vector2 position, Vector2 velocity, float radius, Texture2D texture)
{
	grid_ = grid;
	prev_ = NULL;
	next_ = NULL;


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

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_isDirty = false;
}


void Critter::Draw()
{
	if (m_isLoaded == false)
		return;

	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}
