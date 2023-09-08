#pragma once

#include "texture.hpp"

enum class BulletOrigin : char
{
    Ship = 'S',
    Enemy = 'E'
};

// represents a bullet that both enemies and players can fire
class Bullet
{
public:
    const static int BULLET_SIZE = 5;

    Bullet(int x, int y, BulletOrigin origin);

    // accessor functions
    BulletOrigin GetOrigin();
    int GetX();
    int GetY();
    SDL_Rect GetHitbox();
    bool BeenHit();

    // sets this bullet's hit member to true
    void Hit();
    // moves this bullet
    void Move();
    // renders this bullet to the screen
    void Render(SDL_Renderer *renderer);

private:
    BulletOrigin m_origin;
    int m_pos_x;
    int m_pos_y;
    int m_vel;
    SDL_Rect m_hitbox;
    bool m_hit;
};