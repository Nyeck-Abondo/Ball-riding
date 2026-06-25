#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"
#include "../../UI/button.h"
#include <iostream>

int main() {
    SF::Window* win32 = SF::Window::Create("Ball-riding", 1200, 600);
    win32->Initialize();
    SF::Window* win02 = SF::Window::Create("Second interface", 1080, 720);
    win02->Initialize();

    SF::Button start("START", win32->GetWidth() / 2 - 100, win32->GetHeight() / 2 - 50, 250, 100, 10, SF::pixels(97, 56, 150, 200), SF::pixels(132, 76, 204, 200), SF::pixels(112, 65, 173, 255), [] {
        std::cout << "Login confirmed" << std::endl;
    });

    stbtt_fontinfo font;
    bool fontOK = SF::LoadFont(font, "C:/Windows/Fonts/segoeui.ttf");


    bool run = true;
    bool change = false;

    while (run) {
        while (auto event = SF::Eventmanager::PollEvent()){
            if (event->GetId() == win32->GetId()) {
                start.Update(*event);
                if (const auto* ev = event->GetIf<SF::KeyPressedEvent>()) {
                    if (ev->GetKeyCode() == SF::Keycode::C) {
                        change = true;
                    }
                }

                if (const auto* key = event->GetIf<SF::KeyPressedEvent>()) {
                    if (key->GetKeyCode() == SF::Keycode::B) {
                        change = false;
                    }
                }
            }
            if (const auto* ev = event->GetIf<SF::WindowClosedEvent>()) {
                run = false;
            }
        }
        if (!change) {
            win32->clear(SF::pixels(91, 48, 194, 200));
            if (fontOK) {
                start.Render(win32->GetFrameBuffer(), font);
            }
            win32->Present();
        }
        else {
            win32->Shutdown();
        }
        
        win02->clear(SF::pixels(71, 73, 204, 200));
        win02->Present();
        

        SF::sleep(17);
    }

    delete win32;
    delete win02;
    return 0;
}