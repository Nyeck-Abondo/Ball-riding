#pragma once

#include "ball.h"

namespace SF {
    
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

        void FollowPlayer(Vector2D eDirection);
        void PerformAttack(Vector2D eDirection ,float playerdst,float deltaTime);
        void AttackAnimation(Vector2D playerPos);
        void ApplyRandomMove(float deltaTime);
        void RecastTime(float delTaTime);

        //Rendu
        void Update(Ball& player, float deltaTime);
        void Render(FrameBuffer& fb, pixels color);
    };

} // namespace SF
