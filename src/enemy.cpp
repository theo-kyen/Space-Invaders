#include "../include/enemy.hpp"
using namespace std;

Enemy::Enemy(EnemyType type, int x, int y)
{
    m_type = type;
    m_pos_x = x;
    m_pos_y = y;
    m_init_pos_x = x;
    m_vel = 1;
    m_hitbox = {m_pos_x, m_pos_y, ENEMY_SIZE, ENEMY_SIZE};
    m_hit = false;
    m_rendered = true;
}

Enemy::~Enemy()
{
    m_type = static_cast<EnemyType>(0);
    m_pos_x = 0;
    m_pos_y = 0;
    m_init_pos_x = 0;
    m_vel = 0;
    m_hitbox = {0, 0, 0, 0};
    m_hit = 0;
    m_rendered = 0;
}

EnemyType Enemy::GetType() { return m_type; }

int Enemy::GetX() { return m_pos_x; }

int Enemy::GetY() { return m_pos_y; }

SDL_Rect Enemy::GetHitbox() { return m_hitbox; }

bool Enemy::BeenHit() { return m_hit; }

bool Enemy::IsRendered() { return m_rendered; }

void Enemy::Hit() { m_hit = true; }

void Enemy::Remove()
{
    m_rendered = false;
    m_hitbox = {m_pos_x, m_pos_y, 0, 0};
}

void Enemy::FlipVel() { m_vel *= -1; }

void Enemy::MoveDown(int amount)
{
    m_pos_y += amount;
    m_hitbox = {m_pos_x, m_pos_y, ENEMY_SIZE, ENEMY_SIZE};
}

void Enemy::Move(int max_width, int max_height)
{
    m_pos_x += m_vel;
    m_hitbox.x = m_pos_x;
}

void Enemy::Render(SDL_Renderer *renderer)
{
    if (m_rendered)
    {
        switch (m_type)
        {
        case EnemyType::A:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case EnemyType::B:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            break;
        case EnemyType::C:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            break;
        }
        SDL_RenderFillRect(renderer, &m_hitbox);
    }
}

Bullet Enemy::Shoot()
{
    return Bullet(m_pos_x, m_pos_y, BulletOrigin::Enemy);
}