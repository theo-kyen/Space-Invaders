#pragma once

#include <vector>
#include "texture.hpp"
#include "ship.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

class Game
{
public:
    Game();
    // runs the game
    void Run();

private:
    // frees data and shuts down SDL
    void Close();

    // moves enemies across the screen
    void MoveEnemies();
    // moves bullets across the screen
    void MoveBullets();

    // renders enemies onto the screen
    void RenderEnemies();
    // renders bullets onto the screen
    void RenderBullets();

    // removes any enemy that has been hit by a bullet
    void RemoveEnemies();
    // removes any bullet that has already hit a target
    void RemoveBullets();

    // checks if the ship has already shot a bullet
    bool ShipBulletExists();

    bool CheckCollision(SDL_Rect a, SDL_Rect b);
    // checks for bullet collisions
    // returns -1 if no entity was hit,
    // 0 if the ship was hit,
    // or 1 if an enemy was hit
    int CheckBulletCollision();

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const int SHIP_INIT_X = (SCREEN_WIDTH / 2) - (Ship::SHIP_WIDTH / 2);
    const int SHIP_INIT_Y = SCREEN_HEIGHT - (2 * Ship::SHIP_HEIGHT);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *font = NULL;

    Texture score_display;
    Texture lives_display;

    Ship *ship = NULL;

    int score = 0;

    std::vector<Enemy> enemies;
    const int NUM_ENEMY_ROWS = 5;
    const int NUM_ENEMIES_PER_ROW = 11;
    int NUM_ENEMIES = NUM_ENEMY_ROWS * NUM_ENEMIES_PER_ROW;

    int NUM_BULLETS = 0;
    std::vector<Bullet> bullets;
};