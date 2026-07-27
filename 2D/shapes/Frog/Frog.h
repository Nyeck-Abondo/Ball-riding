#pragma once 

#include "../ball.h"
#include "../ellipse.h"

namespace sf {
    namespace animals {

        enum class HealthState {
            GOOD,
            MEDIUM,
            LOW
        };

        class Frog {
            private:
            Ball f_body{sf::maths::Vector2D(500, 200), 50, 8, 23};
            Ball f_head{sf::maths::Vector2D(500, 200), 43, 8, 30};
            maths::Vector2D f_position;
            pixels f_color, f_headColor;
            HealthState f_healtState;
            Rectangle f_healthBar;
            float f_speedLimit;
            float f_health, f_stamina;
            float f_damage, f_jumpLimit;
            int f_level = 1;
            int f_maxlevel;

            float f_maxhealth;

            public:
            Frog(maths::Vector2D position, pixels Bodyolor, pixels headColor, float defaulthealth, float stamina, float dmg,
                float speed, float jmpLimit);
            ~Frog() { }

            //GETTERS
            Ball& GetBody() { return f_body; }
            Ball& GetHead() { return f_head; }
            HealthState GetHealthState() { return f_healtState; }
            maths::Vector2D GetPosition() {return f_position; }
            float GetHealth() { return f_health; }
            float GetStamina() { return f_stamina; }
            float GetSpeedLimit() { return f_speedLimit; }
            float GetLevel() { return f_level; }
            float GetDanage() { return f_damage; }

            //METHODES PUBLIQUES
            void Move();
            void ApplyDamage(int damage);
            void AttackAnimation();
            void DrawHealthBar(FrameBuffer& fb);
            void DrawEyes(FrameBuffer& fb);
            void Render(FrameBuffer& fb);
            void Update(float clampX, float clampY, float gravity, float deltaTime);
        };
            
    } // namespace animals

} // namespace sf
