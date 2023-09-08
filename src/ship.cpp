#include <iostream>
#include "../include/ship.hpp"

Ship::Ship(int x, int y)
{
    m_lives = 3;
    m_pos_x = x;
    m_pos_y = y;
    m_vel = 0;
    m_hitbox = {m_pos_x, m_pos_y, SHIP_WIDTH, SHIP_HEIGHT};
}

int Ship::GetLives() { return m_lives; }

SDL_Rect Ship::GetHitbox() { return m_hitbox; }

void Ship::Reset(int x, int y)
{
    m_pos_x = x;
    m_pos_y = y;
    m_vel = 0;
    m_hitbox = {m_pos_x, m_pos_y, SHIP_WIDTH, SHIP_HEIGHT};
}

void Ship::Hit() { m_lives--; }

void Ship::HandleMoveEvent(SDL_Event &e)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_a:
        case SDLK_LEFT:
            m_vel -= SHIP_VEL;
            break;
        case SDLK_d:
        case SDLK_RIGHT:
            m_vel += SHIP_VEL;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        m_vel = 0;
}

void Ship::Move(int max_width, int max_height)
{
    m_pos_x += m_vel;
    if (m_pos_x < 0 || m_pos_x + SHIP_WIDTH > max_width)
        m_pos_x -= m_vel;
    m_hitbox.x = m_pos_x;
}

void Ship::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &m_hitbox);
}

Bullet Ship::Shoot()
{
    return Bullet(m_pos_x + (SHIP_WIDTH / 2) - (Bullet::BULLET_SIZE / 2),
                  (m_pos_y - Bullet::BULLET_SIZE), BulletOrigin::Ship);
}