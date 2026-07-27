#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"
#include "../../UI/boutons/button.h"
#include "../../UI/notification/notification.h"
#include "../../2D/shapes/enemies.h"
#include "../../UI/panel/panel.h"
#include "../../2D/shapes/Frog/Frog.h"
#include <chrono>

int main() {
    sf::Window* win32 = sf::Window::Create("Ball-riding", 1920, 1080);
    win32->Initialize();

    sf::Notification notif(
        "Bienvenue Player !", 
        win32->GetWidth() / 2 - 300,
        500, 600, 70, 45, 
        sf::Image("C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/002-actif-1.png", 500, 75),
        sf::NotificationType::standardNotificationDown,
        sf::pixels(13, 9, 20),
        sf::pixels(58, 186, 75)
    );

    sf::Image picture(
        "C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/Swamp_landscape_for_game_202607082311.jpeg",
        sf::maths::Vector2D(0, 0),
        1550, 800
    );
    
    sf::LoadImageFromAssets(picture);

    sf::animals::Frog player(
        sf::maths::Vector2D(200, 200), 
        sf::pixels(13, 117, 74, 255), 
        sf::pixels(13, 117, 74, 255), 
        100.0f, 150.0f, 20.0f, 50.0f, 250.0f
    );

    sf::animals::Frog p2(
        sf::maths::Vector2D(300, 200),
        sf::pixels(204, 136, 6, 250), 
        sf::pixels(204, 136, 6, 250), 
        100.0f, 150.0f, 20.0f, 50.0f, 250.0f
    );

    sf::animals::Frog p3(
        sf::maths::Vector2D(200, 200), 
        sf::pixels(115, 18, 52, 200), 
        sf::pixels(115, 18, 52, 200), 
        100.0f, 150.0f, 20.0f, 50.0f, 250.0f
    );

    stbtt_fontinfo font;
    bool fontOK = sf::LoadFont(font, "C:/Users/Administrator/Documents/Github/Ball-riding/assets/Font/RussoOne-Regular.ttf");
    

    bool run = true;
    bool change = false;
    sf::maths::Vector2D jump(0.0f, 2.0f);
    sf::maths::Vector2D forward(2.0f, 0.0f);

    auto lastUpdateTime = std::chrono::high_resolution_clock::now();

    while (run) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        float deltaTime = elapsedTime.count();
        
        while (auto event = sf::Eventmanager::PollEvent()){
            
            if (event->GetId() == win32->GetId()) {

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::C) {
                        change = true;
                    }
                }

                if (const auto* ev = event->GetIf<sf::MouseClickEvent>()) {
                    std::cout << "clixk a : " << ev->GetPosX() << "; " << ev->GetPosY() << std::endl;
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::B) {
                        player.GetBody().ApplyDisplacement(jump, 170);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::D) {
                        player.GetBody().ApplyDisplacement(forward, 10);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::Q) {
                        player.GetBody().ApplyDisplacement(forward * -1, 10);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::G) {
                        player.AttackAnimation();
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::K) {
                        player.ApplyDamage(5);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::Up) {
                        p3.GetBody().ApplyDisplacement(jump, 170);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::Right) {
                        p3.GetBody().ApplyDisplacement(forward, 10);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::Left) {
                        p3.GetBody().ApplyDisplacement(forward * -1, 10);
                    }
                }

                if (const auto* ev = event->GetIf<sf::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == sf::Keycode::P) {
                        p3.ApplyDamage(5);
                    }
                }

            }
            if (const auto* ev = event->GetIf<sf::WindowClosedEvent>()) {
                run = false;
            }
        }

        if (!change) {
            sf::DrawImage(picture, win32->GetFrameBuffer());
            
            player.Update(win32->GetWidth() - 50.0f, win32->GetHeight() - 200.0f, 900.0f, deltaTime);
            p2.Update(win32->GetWidth() - 50.0f, win32->GetHeight() - 200.0f, 900.0f, deltaTime);
            p3.Update(win32->GetWidth() - 50.0f, win32->GetHeight() - 200.0f, 900.0f, deltaTime);
            p2.Render(win32->GetFrameBuffer());
            p3.Render(win32->GetFrameBuffer());
            player.Render(win32->GetFrameBuffer());
            win32->Present();
        }
        else {
            win32->Shutdown();
        }

        sf::sleep(17);
    }

    picture.FreeImage();
    delete win32;
    return 0;
}