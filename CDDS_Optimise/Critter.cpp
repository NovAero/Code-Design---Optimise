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

Critter::Critter( Vector2 position, Vector2 velocity, float radius, Texture2D texture, bool destroyer) : Critter()
{
	Init(position, velocity, radius, texture, destroyer);
}

Critter::~Critter()
{
	m_isLoaded = false;
}

void Critter::Init( Vector2 position, Vector2 velocity, float radius, Texture2D texture, bool destroyer)
{
	IsDestroyer = destroyer;


	m_position = position;
	m_velocity = velocity;
	m_radius = radius;

	m_texture = texture;
	
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

	Move(dt);

	m_isDirty = false;
}

void Critter::Draw()
{
	if (m_isLoaded == false)
		return;

	DrawTexture(m_texture, m_position.x, m_position.y, WHITE);
}

void Critter::Move(float dt)
{
	if (m_isLoaded == false) return;

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;


}


