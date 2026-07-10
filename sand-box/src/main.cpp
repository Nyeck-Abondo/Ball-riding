#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"
#include "../../UI/boutons/button.h"
#include "../../UI/notification/notification.h"
#include "../../2D/shapes/ball.h"
#include "../../2D/shapes/enemies.h"
#include "../../UI/panel/panel.h"
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

    SF::Image icon(
        "C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/icons/iconsDouble.png",
        SF::Vector2D(75, 20),
        64, 64
    );

    SF::Image icon2(
        "C:/Users/Administrator/Documents/Github/Ball-riding/assets/image/icons/card_subtract.png",
        SF::Vector2D(150, 850),
        64, 64
    );
    
    SF::LoadImageFromAssets(icon);
    SF::LoadImageFromAssets(icon2);
    SF::LoadImageFromAssets(picture);

    stbtt_fontinfo notifFont;
    SF::LoadFont(notifFont, "C:/Users/Administrator/Documents/Github/Ball-riding/assets/Font/Inter_28pt-Medium.ttf");

    SF::Panel sidePanel(
        "Menu",
        SF::Vector2D(0, 0),
        200, win32->GetHeight(), 2, 600, 70,
        SF::pixels(31, 31, 31, 200), SF::pixels(255, 255, 255, 200),
        icon, icon,
        SF::Vector2D(128, 1152),
        notifFont
    );

    sidePanel.AddButton(
        "resume", SF::Vector2D(0, 150),
        sidePanel.GetWidth(), 100, 0, 30,
        SF::pixels(35, 110, 33, 200), SF::pixels(50, 158, 47, 200), 
        SF::pixels(61, 191, 173, 57), [] () {
            std::cout << "OK" << std::endl;
        }
    );

    sidePanel.AddButton(
        "resume", SF::Vector2D(0, 250),
        sidePanel.GetWidth(), 100, 0, 30,
        SF::pixels(35, 110, 33, 200), SF::pixels(50, 158, 47, 200), 
        SF::pixels(61, 191, 173, 57), [] () {
            std::cout << "OK" << std::endl;
        }
    );

    sidePanel.AddButton(
        "resume", SF::Vector2D(0, 350),
        sidePanel.GetWidth(), 100, 0, 30,
        SF::pixels(35, 110, 33, 200), SF::pixels(50, 158, 47, 200), 
        SF::pixels(61, 191, 173, 57), [] () {
            std::cout << "OK" << std::endl;
        }
    );

    sidePanel.AddButton(
        "resume", SF::Vector2D(0, 450),
        sidePanel.GetWidth(), 100, 0, 30,
        SF::pixels(35, 110, 33, 200), SF::pixels(50, 158, 47, 200), 
        SF::pixels(61, 191, 173, 57), [] () {
            std::cout << "OK" << std::endl;
        }
    );

    SF::Button start(
        "START NEW GAME",
        SF::Vector2D(win32->GetWidth() / 2 - 100, win32->GetHeight() / 2 - 50),
         250, 100, 20, 19,
        SF::pixels(97, 56, 150, 200), SF::pixels(132, 76, 204, 200), 
        SF::pixels(112, 65, 173, 255),
        [&notif] () {
            notif.Animation();
            std::cout << "Login confirmed" << std::endl;
    });

    SF::Ball ball(SF::Vector2D(500, 200), 50, 8, 30);
    SF::Ball head(SF::Vector2D(500, 200), 36, 8, 30);
    SF::Ball adv(SF::Ball(SF::Vector2D(500, 200), 50, 8, 30));
    SF::Enemy redFrog(&adv, 50, 200, 3000);

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
                start.Update(*event);
                sidePanel.Update(*event);
                //sidePanel.Update(*event);


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
            }
            if (const auto* ev = event->GetIf<SF::WindowClosedEvent>()) {
                run = false;
            }
        }

        notif.Animation();
        if (!change) {
            win32->clear(SF::pixels(43, 43, 43, 255));
            if (fontOK) {
                start.Render(win32->GetFrameBuffer(), font);
            }
            SF::DrawImage(picture, win32->GetFrameBuffer());
            //SF::DrawImageAt(icon, SF::Vector2D(0, 128), 0, 0, win32->GetFrameBuffer());
            notif.Render(win32->GetFrameBuffer(), font);
            ball.Update(900.0f, deltaTime, 395);
            head.Update(900.0f, deltaTime, 250);
            redFrog.Update(ball, deltaTime);
            ball.Render(win32->GetFrameBuffer(), SF::pixels(34, 145, 47, 200));
            head.Render(win32->GetFrameBuffer(), SF::pixels(34, 200, 47, 200));
            redFrog.Render(win32->GetFrameBuffer(), SF::pixels(143, 26, 52, 200));
            sidePanel.Animate();
            sidePanel.Render(win32->GetFrameBuffer(), font);
            SF::DrawImage(icon2, win32->GetFrameBuffer());

            win32->Present();
        }
        else {
            win32->Shutdown();
        }

        SF::sleep(17);
    }

    picture.FreeImage();
    icon.FreeImage();
    delete win32;
    return 0;
}