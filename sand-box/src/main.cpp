#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"
#include "../../UI/boutons/button.h"
#include "../../UI/notification/notification.h"
#include "../../2D/shapes/ball.h"
#include <chrono>

int main() {
    SF::Window* win32 = SF::Window::Create("Ball-riding", 1200, 600);
    win32->Initialize();

    SF::Notification notif("Bienvenue Player", 
        win32->GetWidth() / 2 - 300, 450, 600, 70,  
        SF::Image("C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/002-actif-1.png", 500, 75),
        SF::NotificationType::standardNotificationDown,
        SF::pixels(13, 9, 20),
        SF::pixels(58, 186, 75)
    );

    SF::Button start(
        "START", 
        win32->GetWidth() / 2 - 100, win32->GetHeight() / 2 - 50, 250, 100, 20,
        SF::pixels(97, 56, 150, 200), SF::pixels(132, 76, 204, 200), 
        SF::pixels(112, 65, 173, 255),
        [&notif] () {
            notif.Animation();
            std::cout << "Login confirmed" << std::endl;
    });

    SF::Vector2D vect(12, 100), vect2(1000, 70);

    SF::Ball ball(SF::Vector2D(500, 200), 50, 8, 20);


    stbtt_fontinfo font;
    bool fontOK = SF::LoadFont(font, "C:/Windows/Fonts/segoeui.ttf");


    bool run = true;
    bool change = false;

    auto lastUpdateTime = std::chrono::high_resolution_clock::now();

    while (run) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        float deltaTime = elapsedTime.count();
        
        while (auto event = SF::Eventmanager::PollEvent()){
            
            if (event->GetId() == win32->GetId()) {
                start.Update(*event);


                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::C) {
                        change = true;
                    }
                }

                if (const auto* ev = event->GetIf<SF::MouseClickEvent>()) {
                    std::cout << "clixk a : " << ev->GetPosX() << "; " << ev->GetPosY() << std::endl;
                }

                if (const auto* key = event->GetIf<SF::KeyPressedEvent>()) {
                    if (key->GetKeyCode() == SF::Keycode::B) {
                        change = false;
                    }
                    if (key->GetKeyCode() == SF::Keycode::A) {
                        ball.SetCenterXPosition(-1.2535f);
                    }
                    if (key->GetKeyCode() == SF::Keycode::D) {
                        ball.SetCenterXPosition(1.2535f);
                    }
                }
            }
            if (const auto* ev = event->GetIf<SF::WindowClosedEvent>()) {
                run = false;
            }
        }

        notif.Animation();
        if (!change) {
            win32->clear(SF::pixels(23, 17, 36, 255));
            if (fontOK) {
                start.Render(win32->GetFrameBuffer(), font);
            }
            notif.Render(win32->GetFrameBuffer(), font);
            ball.Update(900.0f, deltaTime, 19);
            ball.Render(win32->GetFrameBuffer(), SF::pixels(34, 145, 47, 200));
            win32->Present();
        }
        else {
            win32->Shutdown();
        }

        SF::sleep(17);
    }

    delete win32;
    return 0;
}