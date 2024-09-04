#pragma once
#include "TextureManager.h"
#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include "Critter.h"
#include <vector>
#include "QuadTree.h"

class GameManager
{
public:

	GameManager();
	~GameManager();

	static GameManager& GetGameManager() { static GameManager instance; return instance; }

	virtual bool Init();
	virtual void Run();
	virtual void Exit();

	int ScreenSpace() const;

public:
	const int screenWidth = 800;
	const int screenHeight = 450;
	const int CRITTER_COUNT = 50;
	const int MAX_VELOCITY = 80;

private:
	TextureManager tm;
	Quadtree m_quadtree{ screenWidth, screenHeight };

	// create some critters


	Critter* critters;
	Critter* destroyer;

	float timer = 1;
	Vector2 nextSpawnPos;
};

