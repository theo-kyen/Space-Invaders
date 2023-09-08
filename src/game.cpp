#include <iostream>
#include <sstream>
#include "../include/game.hpp"
using namespace std;

Game::Game()
{
    // init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Could not initialize SDL: " << SDL_GetError() << endl;
    }

    if (TTF_Init() < 0)
    {
        cout << "Could not initialize TTF: " << TTF_GetError() << endl;
    }

    // create window and renderer
    window = SDL_CreateWindow("Space Invaders - SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        cout << "Could not create window: " << SDL_GetError() << endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
    }

    font = TTF_OpenFont("assets/pixel_font.ttf", 24);
    if (font == NULL)
    {
        cout << "Could not load font: " << TTF_GetError() << endl;
    }

    ship = new Ship(SHIP_INIT_X, SHIP_INIT_Y);

    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        int x = i % NUM_ENEMIES_PER_ROW;
        int y = i / NUM_ENEMIES_PER_ROW;
        int x_pos = 160 + (45 * x);
        int y_pos = (y + 1) * 40;

        EnemyType type;
        if (y < 1)
            type = EnemyType::C;
        else if (y < 3)
            type = EnemyType::B;
        else
            type = EnemyType::A;

        enemies.push_back(Enemy(type, x_pos, y_pos));
    }
}

void Game::Run()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    ship->Render(renderer);
    RenderEnemies();
    RenderBullets();
    score_display.Render(renderer, 10, 5);
    lives_display.Render(renderer, 625, 5);

    SDL_RenderPresent(renderer);

    SDL_Delay(2000);

    int enemy_shoot_clock = 0;
    SDL_Event e;
    while (true)
    {
        if (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                break;
            else if ((e.type == SDL_KEYDOWN || e.type == SDL_KEYUP))
            {
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE && !ShipBulletExists())
                {
                    bullets.push_back(ship->Shoot());
                    NUM_BULLETS++;
                }
                else
                    ship->HandleMoveEvent(e);
            }
        }

        // if (enemy_shoot_clock == 50)
        // {
        //     bullets.push_back(enemies[0][0].Shoot());
        //     NUM_BULLETS++;
        //     enemy_shoot_clock = 0;
        // }

        ship->Move(SCREEN_WIDTH, SCREEN_HEIGHT);
        MoveEnemies();
        MoveBullets();

        // check for collisions here
        int collision = CheckBulletCollision();
        if (collision == 0)
        {
            ship->Hit();
            NUM_BULLETS = 0;
            SDL_Delay(1500);
            ship->Reset(SHIP_INIT_X, SHIP_INIT_Y);
            continue;
        }
        else if (collision == 1)
        {
            RemoveEnemies();
        }
        RemoveBullets();

        SDL_Color text_color = {255, 255, 255};
        stringstream score_text;
        score_text << "SCORE:" << score;
        score_display.LoadFromRenderedText(renderer, font, score_text.str().c_str(), text_color);
        stringstream lives_text;
        lives_text << "LIVES:" << ship->GetLives();
        lives_display.LoadFromRenderedText(renderer, font, lives_text.str().c_str(), text_color);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ship->Render(renderer);
        RenderEnemies();
        RenderBullets();
        score_display.Render(renderer, 10, 5);
        lives_display.Render(renderer, 625, 5);

        SDL_RenderPresent(renderer);

        // enemy_shoot_clock++;
    }

    Close();
}

void Game::Close()
{
    free(ship);

    score_display.Free();
    lives_display.Free();

    // free window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    TTF_CloseFont(font);
    font = NULL;

    // quit SDL subsystems
    TTF_Quit();
    SDL_Quit();
}

void Game::MoveEnemies()
{
    bool flip = false;
    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        enemies[i].Move(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        int j = i % NUM_ENEMIES_PER_ROW;
        if (enemies[i].GetX() <= 45 * j || enemies[i].GetX() + Enemy::ENEMY_SIZE >= SCREEN_WIDTH - (45 * (NUM_ENEMIES_PER_ROW - j - 1)))
        {
            flip = true;
            break;
        }
    }

    if (flip)
    {
        for (int i = 0; i < NUM_ENEMIES; i++)
        {
            // enemies[i].MoveDown(40);
            enemies[i].FlipVel();
        }
    }
}

void Game::RenderEnemies()
{
    for (int i = 0; i < NUM_ENEMIES; i++)
        enemies[i].Render(renderer);
}

void Game::MoveBullets()
{
    for (int i = 0; i < NUM_BULLETS; i++)
        bullets[i].Move();
}

void Game::RenderBullets()
{
    for (int i = 0; i < NUM_BULLETS; i++)
        bullets[i].Render(renderer);
}

void Game::RemoveEnemies()
{
    int count = 0;
    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        if (enemies[i].BeenHit())
        {
            switch (enemies[i].GetType())
            {
            case EnemyType::A:
                score += 10;
                break;
            case EnemyType::B:
                score += 20;
                break;
            case EnemyType::C:
                score += 30;
                break;
            }
            enemies.erase(enemies.begin() + i, enemies.begin() + i + 1);
            NUM_ENEMIES--;
            break;
        }
    }
}

void Game::RemoveBullets()
{
    vector<Bullet>::iterator it = bullets.begin();
    while (it != bullets.end())
    {
        if (it->BeenHit() || it->GetY() < 0 || it->GetY() > SCREEN_HEIGHT)
        {
            it = bullets.erase(it);
            NUM_BULLETS--;
        }
        else
            ++it;
    }
}

bool Game::ShipBulletExists()
{
    vector<Bullet>::iterator it = bullets.begin();
    while (it != bullets.end())
    {
        if (it->GetOrigin() == BulletOrigin::Ship)
            return true;
    }
    return false;
}

// checks for collisions between two rects
bool Game::CheckCollision(SDL_Rect a, SDL_Rect b)
{
    int left_a = a.x;
    int right_a = a.x + a.w;
    int top_a = a.y;
    int bottom_a = a.y + a.h;

    int left_b = b.x;
    int right_b = b.x + b.w;
    int top_b = b.y;
    int bottom_b = b.y + b.h;

    if (bottom_a <= top_b)
    {
        return false;
    }
    if (top_a >= bottom_b)
    {
        return false;
    }
    if (right_a <= left_b)
    {
        return false;
    }
    if (left_a >= right_b)
    {
        return false;
    }
    return true;
}

// BUG: Doesn't remove the last enemy in a row
int Game::CheckBulletCollision()
{
    for (int i = 0; i < NUM_BULLETS; i++)
    {
        if (bullets[i].GetOrigin() == BulletOrigin::Enemy)
        {
            if (CheckCollision(bullets[i].GetHitbox(), ship->GetHitbox()))
            {
                bullets[i].Hit();
                return 0;
            }
        }
        else
        {
            for (int j = 0; j < NUM_ENEMIES; j++)
            {
                if (CheckCollision(bullets[i].GetHitbox(), enemies[j].GetHitbox()))
                {
                    bullets[i].Hit();
                    enemies[j].Hit();
                    return 1;
                }
            }
        }
    }

    return -1;
}