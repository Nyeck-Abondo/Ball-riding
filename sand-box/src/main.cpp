#include "../../Native-window-system/Plateform/window.h"
#include "../../Native-window-system/sleep/sleep.h"

int main() {
    SF::Window* win32 = SF::Window::Create("Ball-riding", 1200, 600);
    win32->Initialize();
    SF::Window* win02 = SF::Window::Create("Second interface", 1080, 720);
    win02->Initialize();


    bool run = true;
    bool change = false;

    while (run) {
        while (auto event = SF::Eventmanager::PollEvent()){
            if (event->GetId() == win32->GetId()) {
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
            win32->Present();
        }
        else {
            win32->Shutdown();
        }
        
        win02->clear(SF::pixels(71, 73, 204, 200));
        win02->Present();
        

        SF::sleep(17);
    }

    win32->Shutdown();
    return 0;
}