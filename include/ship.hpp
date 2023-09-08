#pragma once

#include "texture.hpp"
#include "bullet.hpp"

// represents the ship that the player can control
class Ship
{
public:
    const static int SHIP_WIDTH = 40;
    const static int SHIP_HEIGHT = 20;
    const static int SHIP_VEL = 5;

    Ship(int x, int y);

    // accessor functions
    int GetLives();
    SDL_Rect GetHitbox();

    // revert the ship back to its original position
    void Reset(int x, int y);
    // remove a life when this ship's been hit
    void Hit();

    // handles any movement events
    void HandleMoveEvent(SDL_Event &e);
    // moves this ship
    void Move(int max_width, int max_height);
    // renders this ship to the screen
    void Render(SDL_Renderer *renderer);

    // shoots a bullet
    Bullet Shoot();

private:
    int m_lives;
    int m_pos_x;
    int m_pos_y;
    int m_vel;
    SDL_Rect m_hitbox;
};