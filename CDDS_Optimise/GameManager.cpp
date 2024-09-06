#include "GameManager.h"
#include <random>
#include <iostream>

GameManager::GameManager() {}

GameManager::~GameManager() {}

bool GameManager::Init()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    srand(time(NULL));

    critters = new Critter[CRITTER_COUNT + 10];


    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        // create a random direction vector for the velocity
        Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
        // normalize and scale by a random speed
        velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

        // create a critter in a random location
        critters[i].Init(Vector2{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
            velocity,
            12, tm.GetTexture("10.png"), false);

        grid.Add(&critters[i]);
    }

    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

    destroyer = new Critter( Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 30, tm.GetTexture("9.png"), true);

    nextSpawnPos = destroyer->GetPosition();

    grid.destroyer = destroyer;

    return true;
}

void GameManager::Run()
{
    // Update
       //----------------------------------------------------------------------------------
       // TODO: Update your variables here
       //----------------------------------------------------------------------------------

    float delta = GetFrameTime();

    // update the destroyer
    destroyer->Update(delta);
    // check each critter against screen bounds
    if (destroyer->GetX() < 0) {
        destroyer->SetX(0);
        destroyer->SetVelocity(Vector2{ -destroyer->GetVelocity().x, destroyer->GetVelocity().y });
    }
    if (destroyer->GetX() > screenWidth) {
        destroyer->SetX(screenWidth);
        destroyer->SetVelocity(Vector2{ -destroyer->GetVelocity().x, destroyer->GetVelocity().y });
    }
    if (destroyer->GetY() < 0) {
        destroyer->SetY(0);
        destroyer->SetVelocity(Vector2{ destroyer->GetVelocity().x, -destroyer->GetVelocity().y });
    }
    if (destroyer->GetY() > screenHeight) {
        destroyer->SetY(screenHeight);
        destroyer->SetVelocity(Vector2{ destroyer->GetVelocity().x, -destroyer->GetVelocity().y });
    }

    // update the critters
    // (dirty flags will be cleared during update)
    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Update(delta);

        // check each critter against screen bounds
        if (critters[i].GetX() < 0) {
            critters[i].SetX(0);
            critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
        }
        if (critters[i].GetX() > screenWidth) {
            critters[i].SetX(screenWidth);
            critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
        }
        if (critters[i].GetY() < 0) {
            critters[i].SetY(0);
            critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
        }
        if (critters[i].GetY() > screenHeight) {
            critters[i].SetY(screenHeight);
            critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
        }

    }

    //Check for critter collisjons

    timer -= delta;
    if (timer <= 0)
    {
        timer = 1;

        // find any dead critters and spit them out (respawn)
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            if (critters[i].IsDead())
            {
                Vector2 normal = Vector2Normalize(destroyer->GetVelocity());

                // get a position behind the destroyer, and far enough away that the critter won't bump into it again
                Vector2 pos = destroyer->GetPosition();
                pos = Vector2Add(pos, Vector2Scale(normal, -50));
                // its pretty ineficient to keep reloading textures. ...if only there was something else we could do
                critters[i].Init( pos, Vector2Scale(normal, -MAX_VELOCITY), 12, tm.GetTexture("10.png"), false);
                break;
            }
        }
        nextSpawnPos = destroyer->GetPosition();
    }

    grid.Update(delta);
    HandleCollisions();

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    grid.Draw();

    // draw the critters
    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Draw();
    }
    // draw the destroyer
    destroyer->Draw();
    // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
    // destroyer, it's because the origin is at the top-left. ...you could fix that!)

    DrawFPS(10, 10);

    EndDrawing();
}

void GameManager::Exit()
{
    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Destroy();
    }
}

void GameManager::HandleCollisions()
{
    grid.HandleCollisions();
}

int GameManager::ScreenSpace() const
{
    return screenHeight * screenWidth;
}