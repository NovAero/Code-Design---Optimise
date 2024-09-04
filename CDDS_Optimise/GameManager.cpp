#include "GameManager.h"
#include <random>

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
    delete destroyer;
}


bool GameManager::Init()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    srand(time(NULL));

    critters = new Critter[CRITTER_COUNT + (CRITTER_COUNT / 4)];

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        // create a random direction vector for the velocity
        Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
        // normalize and scale by a random speed
        velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

        // create a critter in a random location
        critters[i].Init(
            { (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
            velocity,
            12, tm.GetTexture("10.png"));

        m_quadtree.Insert(&critters[i]);
    }

    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

    destroyer = new Critter(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, tm.GetTexture("9.png"));

    m_quadtree.Insert(destroyer);

    nextSpawnPos = destroyer->GetPosition();

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

        // kill any critter touching the destroyer
        // simple circle-to-circle collision check
        float dist = Vector2Distance(critters[i].GetPosition(), destroyer->GetPosition());
        if (dist < critters[i].GetRadius() + destroyer->GetRadius())
        {
            critters[i].Destroy();
            // this would be the perfect time to put the critter into an object pool
        }
    }

    // check for critter-on-critter collisions
    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        for (int j = 0; j < CRITTER_COUNT; j++) {
            if (i == j || critters[i].IsDirty()) // note: the other critter (j) could be dirty - that's OK
                continue;
            // check every critter against every other critter
            float dist = Vector2Distance(critters[i].GetPosition(), critters[j].GetPosition());
            if (dist < critters[i].GetRadius() + critters[j].GetRadius())
            {
                // collision!
                // do math to get critters bouncing
                Vector2 normal = Vector2Normalize(Vector2Subtract(critters[j].GetPosition(), critters[i].GetPosition()));

                // not even close to real physics, but fine for our needs
                critters[i].SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
                // set the critter to *dirty* so we know not to process any more collisions on it
                critters[i].SetDirty();

                // we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
                // to make sure the other critter is clean before we do the collision response
                if (!critters[j].IsDirty()) {
                    critters[j].SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
                    critters[j].SetDirty();
                }
                break;
            }
        }
    }

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
                critters[i].Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, tm.GetTexture("10.png"));
                break;
            }
        }
        nextSpawnPos = destroyer->GetPosition();
    }

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    m_quadtree.Draw();

    ClearBackground(RAYWHITE);

    // draw the critters
    /*for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Draw();
    }*/
    // draw the destroyer
    // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
    // destroyer, it's because the origin is at the top-left. ...you could fix that!)
    destroyer->Draw();

    DrawFPS(10, 10);
    //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

    EndDrawing();

}

void GameManager::Exit()
{
    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Destroy();
    }

}

int GameManager::ScreenSpace() const
{
    return screenHeight * screenWidth;
}
