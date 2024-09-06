#pragma once
#include "TextureManager.h"
#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include "Critter.h"
#include <vector>
#include "Grid.h"

class GameManager
{
public:

	GameManager();
	~GameManager();

	static GameManager& GetGameManager() { static GameManager instance; return instance; }

	virtual bool Init();
	virtual void Run();
	virtual void Exit();

	void HandleCollisions();
	int ScreenSpace() const;

public:
	const int screenWidth = 800;
	const int screenHeight = 800;
	const int CRITTER_COUNT = 100;
	const int MAX_VELOCITY = 80;

private:
	TextureManager tm;
	Grid grid;

	// create some critters
	Critter* critters;
	Critter* destroyer;

	float timer = 1;
	Vector2 nextSpawnPos;
};

