#include "enemies.h"

namespace SF {
    
    bool Enemy::IsInPatrolRange(float playerdst) {
        if (playerdst <= m_agressiveRange) {
            return true;
        }
        return false;
    }

    void Enemy::FollowPlayer(Vector2D eDirection) {
        m_body->ApplyDisplacement(eDirection, 50);
    }

    void Enemy::PerformAttack(Vector2D eDirection ,float playerdst,float deltaTime) {
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

    void Enemy::AttackAnimation(Vector2D playerPos) {
        Vector2D jump(0.0f, 3.0f);
        m_body->ApplyDisplacement(playerPos, 30);
        m_body->ApplyDisplacement(jump, 50);
    }

    void Enemy::RecastTime(float deltaTime) {
        m_recastTime -= 500 * deltaTime;
    }

    void Enemy::Update(Ball& player, float deltaTime) {
        m_body->Update(900.0f, deltaTime, 250);

        int pLinks = player.GetAllPoints().size();
        int eLinks = m_body->GetAllPoints().size();

        Vector2D pMainPos = player.GetAllPoints()[(int)(pLinks / 2)]->mainPos;
        Vector2D eMainPos = m_body->GetAllPoints()[(int)(eLinks / 2)]->mainPos;
        Vector2D eDirection = (eMainPos - pMainPos) * -1;
        float playerDst = pMainPos.Distance(eMainPos);

        if (IsInPatrolRange(playerDst)) FollowPlayer(eDirection);
        PerformAttack(eDirection, playerDst, deltaTime);
    }

    void Enemy::Render(FrameBuffer& fb, pixels color) {
        m_body->Render(fb, color);
    }

} // namespace SF
