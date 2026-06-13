#include "../../Native-window-system/Plateform/window.h"

int main() {
    SF::Window* win32 = SF::Window::Create("Ball-riding", 1200, 600);
    win32->Initialize();
    bool run = true;

    while (run) {
        win32->ProcessEvent();
        win32->clear(SF::pixels(91, 48, 194, 200));
        win32->Present();
    }
    win32->Shutdown();
}