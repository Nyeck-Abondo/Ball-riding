#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    
    case WM_DESTROY: 
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"BALL-RIDING";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    //enregistrement de la classe
    RegisterClassEx(&wc);

    //creationd de la fenetre
    HWND hWnd = CreateWindowEx(
        0,
        L"BALL-RIDING",
        L"MOTEUR DE RENDU LOGICIEL",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        nullptr, nullptr,
        hInstance,
        nullptr
    );

    if (hWnd == nullptr) {
        std::cerr << "Erreur de creation de la fenetre" << std::endl;
        return -1;
    }

    //boucle de messages en temps reels
    MSG msg = {};
    bool running = true;

    while (running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
    }

    return 0;   
}