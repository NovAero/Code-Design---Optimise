#pragma once
#include "TextureManager.h"
#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include "Critter.h"
#include <vector>

class GameManager
{
public:

	GameManager();
	~GameManager();

	static GameManager& GetGameManager() { static GameManager instance; return instance; }

	void Init();
	void Run();
	void Exit();

	int ScreenSpace() const;

public:
	const int screenWidth = 800;
	const int screenHeight = 450;

private:
	TextureManager tm;
	Grid grid;

	// create some critters

	const int CRITTER_COUNT = 50;
	const int MAX_VELOCITY = 80;

	Critter* critters;
	Critter destroyer;

	float timer = 1;
	Vector2 nextSpawnPos = destroyer.GetPosition();
};

