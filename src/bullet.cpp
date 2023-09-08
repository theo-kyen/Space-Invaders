#include <iostream>
#include "../include/bullet.hpp"
using namespace std;

Bullet::Bullet(int x, int y, BulletOrigin origin)
{
    m_origin = origin;
    m_pos_x = x;
    m_pos_y = y;
    m_vel = 8;
    if (origin == BulletOrigin::Ship)
        m_vel *= -1;
    m_hitbox = {m_pos_x, m_pos_y, BULLET_SIZE, BULLET_SIZE};
    m_hit = false;
}

BulletOrigin Bullet::GetOrigin() { return m_origin; }

int Bullet::GetX() { return m_pos_x; }

int Bullet::GetY() { return m_pos_y; }

SDL_Rect Bullet::GetHitbox() { return m_hitbox; }

bool Bullet::BeenHit() { return m_hit; }

void Bullet::Hit() { m_hit = true; }

void Bullet::Move()
{
    m_pos_y += m_vel;
    m_hitbox.y = m_pos_y;
}

void Bullet::Render(SDL_Renderer *renderer)
{
    if (m_origin == BulletOrigin::Ship)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &m_hitbox);
}