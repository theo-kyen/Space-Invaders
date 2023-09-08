#pragma once

#include "texture.hpp"
#include "bullet.hpp"

enum class EnemyType : char
{
    A = 'A',
    B,
    C
};

// represents an enemy alien
class Enemy
{
public:
    const static int ENEMY_SIZE = 30;

    Enemy(EnemyType type, int x, int y);
    ~Enemy();

    // accessor functions
    EnemyType GetType();
    int GetX();
    int GetY();
    SDL_Rect GetHitbox();
    bool BeenHit();
    bool IsRendered();

    // sets the hit member to true
    void Hit();
    void Remove();

    // flips this enemy's velocity
    // so that it moves the other way
    void FlipVel();
    // moves this enemy down by the given amount
    void MoveDown(int amount);
    // moves this enemy
    void Move(int max_width, int max_height);
    // renders this enemy to the screen
    void Render(SDL_Renderer *renderer);

    // shoots a bullet
    Bullet Shoot();

private:
    EnemyType m_type;
    int m_pos_x;
    int m_pos_y;
    int m_init_pos_x;
    int m_vel;
    SDL_Rect m_hitbox;
    bool m_hit;
    bool m_rendered;
};