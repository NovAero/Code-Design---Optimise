#include "Critter.h"
#include "TextureManager.h"

Critter::Critter()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isLoaded = false;
}

Critter::Critter(Vector2 position, Vector2 velocity, float radius, Texture2D texture) : Critter()
{
	Init(position, velocity, radius, texture);
}

Critter::~Critter()
{
	m_isLoaded = false;
}

void Critter::Init(Vector2 position, Vector2 velocity, float radius, Texture2D texture)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;

	m_texture = texture;

	m_bounds.m_centre = position;
	m_bounds.m_halfSize.x = radius/2;
	m_bounds.m_halfSize.y = radius/2;
	
	m_isLoaded = true;
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
