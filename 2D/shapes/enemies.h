#pragma once

#include "ball.h"

namespace sf {
    
    class Enemy {
        private:
        Ball* m_body;
        int m_damage;
        int m_agressiveRange;
        int m_attackRange;
        float m_recastTime, m_tempRecastTime;

        public:
        Enemy(Ball* body, int damage, int range, float recast) 
        : m_body(body), m_damage(damage), m_agressiveRange(range),
        m_recastTime(recast), m_tempRecastTime(recast) { }
        ~Enemy() {}

        //METHODES
        bool IsInPatrolRange(float playerdst);
        bool CanAttack() { return m_recastTime <= 0.0f; }

        void FollowPlayer(maths::Vector2D eDirection);
        void PerformAttack(maths::Vector2D eDirection ,float playerdst,float deltaTime);
        void AttackAnimation(maths::Vector2D playerPos);
        void ApplyRandomMove(float deltaTime);
        void RecastTime(float delTaTime);

        //Rendu
        void Update(float clampX, float clampY, Ball& player, float deltaTime);
        void Render(render::Renderer& renderer, pixels color);
    };

} // namespace sf
