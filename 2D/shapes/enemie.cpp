#include "enemies.h"

namespace sf {
    
    bool Enemy::IsInPatrolRange(float playerdst) {
        if (playerdst <= m_agressiveRange) {
            return true;
        }
        return false;
    }

    void Enemy::FollowPlayer(maths::Vector2D eDirection) {
        m_body->ApplyDisplacement(eDirection, 50);
    }

    void Enemy::PerformAttack(maths::Vector2D eDirection ,float playerdst,float deltaTime) {
        static float attackTime = 600.0f;
        if ((playerdst <= m_attackRange) && CanAttack()) {
            AttackAnimation(eDirection);
            attackTime -= 500.0f * deltaTime;
            if (attackTime <= 0.0f) {
                m_recastTime = m_tempRecastTime;
                attackTime = 1600.0f;
            }
        }
        if (!CanAttack()) RecastTime(deltaTime);
    }

    void Enemy::AttackAnimation(maths::Vector2D playerPos) {
        maths::Vector2D jump(0.0f, 3.0f);
        m_body->ApplyDisplacement(playerPos, 30);
        m_body->ApplyDisplacement(jump, 50);
    }

    void Enemy::RecastTime(float deltaTime) {
        m_recastTime -= 500 * deltaTime;
    }

    void Enemy::Update(float clampX, float clampY, Ball& player, float deltaTime) {
        m_body->Update(clampX , clampY, 900.0f, deltaTime, 250);

        int pLinks = player.GetAllPoints().size();
        int eLinks = m_body->GetAllPoints().size();

        maths::Vector2D pMainPos = player.GetAllPoints()[(int)(pLinks / 2)]->mainPos;
        maths::Vector2D eMainPos = m_body->GetAllPoints()[(int)(eLinks / 2)]->mainPos;
        maths::Vector2D eDirection = (eMainPos - pMainPos) * -1;
        float playerDst = pMainPos.Distance(eMainPos);

        if (IsInPatrolRange(playerDst)) FollowPlayer(eDirection);
        PerformAttack(eDirection, playerDst, deltaTime);
    }

    void Enemy::Render(FrameBuffer& fb, pixels color) {
        m_body->Render(fb, color);
    }

} // namespace sf
