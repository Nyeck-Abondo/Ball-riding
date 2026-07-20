#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"
#include "../../UI/boutons/button.h"
#include "../../UI/notification/notification.h"
#include "../../2D/shapes/ball.h"
#include "../../2D/shapes/ellipse.h"
#include "../../2D/shapes/enemies.h"
#include "../../UI/panel/panel.h"
#include "../../2D/shapes/Frog/legs.h"
#include <chrono>

int main() {
    SF::Window* win32 = SF::Window::Create("Ball-riding", 1920, 1080);
    win32->Initialize();

    SF::Notification notif(
        "Bienvenue Player !", 
        win32->GetWidth() / 2 - 300,
        500, 600, 70, 45, 
        SF::Image("C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/002-actif-1.png", 500, 75),
        SF::NotificationType::standardNotificationDown,
        SF::pixels(13, 9, 20),
        SF::pixels(58, 186, 75)
    );

    SF::Image picture(
        "C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/Swamp_landscape_for_game_202607082311.jpeg",
        SF::Vector2D(0, 0),
        1550, 800
    );
    
    SF::LoadImageFromAssets(picture);

    SF::Ellipse elhead(SF::Node(SF::Vector2D(500, 150)), 45, 30, 20);

    SF::Ball ball(SF::Vector2D(500, 200), 50, 8, 20);
    SF::Ball head(SF::Vector2D(500, 200), 43, 8, 30);
    SF::Legs leg(SF::Legs(SF::FootType::Left, ball.GetPoint(15)->mainPos, 1, 1, 30, 30));

    stbtt_fontinfo font;
    bool fontOK = SF::LoadFont(font, "C:/Users/Administrator/Documents/Github/Ball-riding/assets/Font/RussoOne-Regular.ttf");
    

    bool run = true;
    bool change = false;
    SF::Vector2D jump(0.0f, 2.0f);
    SF::Vector2D forward(2.0f, 0.0f);

    auto lastUpdateTime = std::chrono::high_resolution_clock::now();

    while (run) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;
        float deltaTime = elapsedTime.count();
        
        while (auto event = SF::Eventmanager::PollEvent()){
            
            if (event->GetId() == win32->GetId()) {

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::C) {
                        change = true;
                    }
                }

                if (const auto* ev = event->GetIf<SF::MouseClickEvent>()) {
                    std::cout << "clixk a : " << ev->GetPosX() << "; " << ev->GetPosY() << std::endl;
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::B) {
                        ball.ApplyDisplacement(jump, 170);
                    }
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::D) {
                        ball.ApplyDisplacement(forward, 10);
                    }
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::Q) {
                        ball.ApplyDisplacement(forward * -1, 10);
                    }
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::Up) {
                        head.ApplyDisplacement(jump, 170);
                    }
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::Right) {
                        head.ApplyDisplacement(forward, 10);
                    }
                }

                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::Left) {
                        head.ApplyDisplacement(forward * -1, 10);
                    }
                }
            }
            if (const auto* ev = event->GetIf<SF::WindowClosedEvent>()) {
                run = false;
            }
        }

        if (!change) {
            SF::DrawImage(picture, win32->GetFrameBuffer());
            
            //head.SetPointfixedPosition(SF::Vector2D(ball.GetCenter().mainPos.m_x + 5, ball.GetCenter().mainPos.m_y + ball.GetRadius() + 40.0f), 0);
            //head.SetPointfixedPosition(SF::Vector2D(ball.GetCenter().mainPos.m_x + 30, ball.GetCenter().mainPos.m_y + ball.GetRadius() + 30.0f), 20);
            ball.Update(900.0f, deltaTime, 395);

             head.Update(
                 900.0f, 
                 deltaTime, 
                 1100, 
                 SF::Vector2D(ball.GetCenter().mainPos.m_x - 35.0f, ball.GetCenter().mainPos.m_y + ball.GetRadius() - 70.0f), 
                 SF::Vector2D(ball.GetCenter().mainPos.m_x + 60, ball.GetCenter().mainPos.m_y + ball.GetRadius() - 80.0f),
                 0, 14
             );


            leg.Update(900.0f, deltaTime, ball.GetCenter().mainPos + 12);

            ball.Render(win32->GetFrameBuffer(), SF::pixels(13, 117, 74, 255));
            head.Render(win32->GetFrameBuffer(), SF::pixels(13, 117, 74, 255));
            //elhead.Render(win32->GetFrameBuffer(), SF::pixels(34, 145, 47, 240));
            leg.Render(win32->GetFrameBuffer(), SF::pixels(120, 9, 11));

            win32->Present();
        }
        else {
            win32->Shutdown();
        }

        SF::sleep(17);
    }

    picture.FreeImage();
    delete win32;
    return 0;
}