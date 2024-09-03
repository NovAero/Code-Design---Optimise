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

	void Init();
	void Run();
	void Exit();


private:

	int screenWidth = 800;

	int screenHeight = 450;

	TextureManager tm;

	// create some critters

	const int CRITTER_COUNT = 50;
	const int MAX_VELOCITY = 80;

	Critter* critters;
	Critter destroyer;

	float timer = 1;
	Vector2 nextSpawnPos = destroyer.GetPosition();
};

