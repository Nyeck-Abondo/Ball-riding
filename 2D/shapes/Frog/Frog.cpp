#include "Frog.h"

namespace sf {
    namespace animals {

        
        Frog::Frog(maths::Vector2D position, pixels Bodyolor, pixels headColor, float defaulthealth, float stamina, float dmg,
            float speed, float jmpLimit)
        : f_position(position), f_color (Bodyolor), f_headColor(headColor), f_health(defaulthealth), f_stamina(stamina),
        f_speedLimit(speed), f_jumpLimit(jmpLimit), f_healthBar(maths::Vector2D(f_position.m_x - 60.f, f_position.m_y - 80.f), 120.0f, 7.f, 4, pixels(31, 186, 63, 200), pixels(250, 250, 255, 200)) {
            f_healtState = HealthState::GOOD;
            f_damage = 50.0f;
            f_level = 1;
            f_stamina = 100.0f;
            f_maxlevel = 25; 
            f_maxhealth = defaulthealth; 
        }

        void Frog::Move() {}

        void Frog::ApplyDamage (int damage) {
            float hCopy = f_health;
            f_health -= damage;
            f_healthBar.width = (f_health * f_healthBar.width) / ((hCopy <= 0) ? 1 : hCopy);
        }

        void Frog::AttackAnimation() {}

        void Frog::DrawHealthBar(FrameBuffer& fb) {
            if (f_health > 2 * f_maxhealth / 3) f_healtState = HealthState::GOOD;
            if (f_health <= f_maxhealth / 3 && f_health > f_maxhealth / 3) f_healtState = HealthState::MEDIUM;
            if (f_health < f_maxhealth / 3) f_healtState = HealthState::LOW;

            switch (f_healtState) {
                case HealthState::GOOD :
                    f_healthBar.innerColor = pixels(31, 186, 63, 200);
                break;

                case HealthState::MEDIUM :
                    f_healthBar.innerColor = pixels(245, 192, 10, 220);
                break;

                case HealthState::LOW :
                    f_healthBar.innerColor = pixels(224, 11, 15, 220);
                break;
            }

            f_healthBar.DrawRenctangle(5, fb);
        }

        void Frog::DrawEyes(FrameBuffer& fb) {
            maths::Vector2D clip = f_head.GetCenter().mainPos;
            DrawFillCircle(sf::maths::Vector2D(clip.m_x + 20.0f, clip.m_y - 30.0f), 18, sf::pixels(13, 117, 74, 255), fb);
            DrawFillCircle(sf::maths::Vector2D(clip.m_x + 20.0f, clip.m_y - 30.0f), 10, sf::pixels(255, 192, 41, 210), fb);
            DrawFillCircle(sf::maths::Vector2D(clip.m_x - 10.0f, clip.m_y + 10.0f), 5, sf::pixels(58, 171, 72, 200), fb);
            DrawRoundedRect(
                sf::maths::Vector2D(clip.m_x + 18.0f, clip.m_y - 30.0f).m_x,
                sf::maths::Vector2D(clip.m_x + 18.0f, clip.m_y - 30.0f).m_y,
                10, 5, 2,
                fb,
                sf::pixels(36, 27, 6, 240)
            );
        }

        void Frog::Render(FrameBuffer& fb) {
            f_body.Render(fb, f_color);
            f_head.Render(fb, f_headColor);

            DrawEyes(fb);
            DrawHealthBar(fb);
        }

        void Frog::Update(float clampX, float clampY, float gravity, float deltaTime) {
            f_body.Update(clampX, f_jumpLimit, 900.0f, deltaTime, 395);
            f_head.Update(
                clampX, clampY,
                900.0f, 
                deltaTime, 
                1100, 
                sf::maths::Vector2D(f_body.GetCenter().mainPos.m_x - 35.0f, f_body.GetCenter().mainPos.m_y + f_body.GetRadius() - 70.0f), 
                sf::maths::Vector2D(f_body.GetCenter().mainPos.m_x + 60, f_body.GetCenter().mainPos.m_y + f_body.GetRadius() - 80.0f),
                0, 14
            );
            f_position = f_body.GetCenter().mainPos;
            f_healthBar.pos = maths::Vector2D(f_position.m_x - 70.f, f_position.m_y - 95.f);
        }
        
    } // namespace animals
    
} // namespace sf
