#ifndef UNICODE
#define UNICODE
#endif

#include "windowWin32.h"

namespace SF {

    const wchar_t* CLASSNAME = L"Ball-Riding";
    uint32_t idCounter = 0;

    windowWin32::windowWin32(const char* title, uint32_t width, uint32_t height) 
        : m_title(title), m_Width(width), m_Height(height), m_IsOpen(false),
        m_id(idCounter++), m_FrameBuffer(m_Width, m_Height) {
            m_hInstance = GetModuleHandle(nullptr);
            m_wc = {};
        }

    windowWin32::~windowWin32() {
        Shutdown();
    }

    bool windowWin32::Initialize() {
        m_wc.cbSize = sizeof(WNDCLASSEX);
        m_wc.hCursor = LoadCursor(nullptr, IDC_ARROW),
        m_wc.hInstance = m_hInstance;
        m_wc.lpfnWndProc = WinProc;
        m_wc.lpszClassName = CLASSNAME;
        m_wc.style = CS_HREDRAW | CS_VREDRAW;

        RegisterClassEx(&m_wc);

        // Conversion du titre UTF-8 vers UTF-16
        int titleLen = MultiByteToWideChar(CP_UTF8, 0, m_title.c_str(), -1, nullptr, 0);
        wchar_t* wTitle = new wchar_t[titleLen];
        MultiByteToWideChar(CP_UTF8, 0, m_title.c_str(), -1, wTitle, titleLen);

        //creationde la fenetre
        m_hwnd = CreateWindowEx (
            0,
            CLASSNAME,
            wTitle,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT,
            m_Width, m_Height,
            nullptr, nullptr,
            m_hInstance,
            this
        );
        m_IsOpen = true;

        delete[] wTitle;

        if(!m_hwnd) return false;

        WindowManager::RegisterWindow(this);
        return true;
    }

    bool windowWin32::IsOpen() {
        if (m_IsOpen) {
            return true;
        }
        return false;
    }

    void windowWin32::SetOpen() {
        m_IsOpen = false;
    }

    void windowWin32::clear(const pixels& color) {
        m_FrameBuffer.ClearBuffer(color);
    }

    void windowWin32::ProcessEvent() {
        MSG msg;
        while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void windowWin32::Shutdown() {
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
        WindowManager::UnregisterWindow(this);
    }

    void windowWin32::Present() {
        m_FrameBuffer.Present(m_hwnd);
    }

    uint32_t windowWin32::GetWidth() {
        return m_Width;
    }

    uint32_t windowWin32::GetHeight() {
        return m_Height;
    }

    uint32_t windowWin32::GetId() {
        return m_id;
    }

    LRESULT CALLBACK windowWin32::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        windowWin32* window;

        //recuperation de l'instance win32
        if (uMsg == WM_CREATE) {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            window = reinterpret_cast<windowWin32*>(cs->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        } else {
            window = reinterpret_cast<windowWin32*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }

        if (!window) return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        
        switch (uMsg) {
            case WM_CLOSE: {
                Eventmanager::PushEvent(new WindowClosedEvent(window->GetId()));
                window->SetOpen();
                return 0;
            }

            case WM_MOUSEMOVE: {
                float mx = (float)GET_X_LPARAM(lParam);
                float my = (float)GET_Y_LPARAM(lParam);

                Eventmanager::PushEvent(new MouseMoveEvent(mx, my, window->GetId()));
            return 0;
            }
                

            case WM_LBUTTONDOWN: {
                float mx = (float)GET_X_LPARAM(lParam);
                float my = (float)GET_Y_LPARAM(lParam);

                Eventmanager::PushEvent(new MouseClickEvent(mx, my, MouseButton::leftBtn, window->GetId()));
                return 0;
            }
                

            case WM_RBUTTONDOWN: {
                float mx = (float)GET_X_LPARAM(lParam);
                float my = (float)GET_Y_LPARAM(lParam);

                Eventmanager::PushEvent(new MouseClickEvent(mx, my, MouseButton::RightBtn, window->GetId()));
                return 0;
            }

            case WM_SIZE: {
                unsigned int width = LOWORD(lParam);
                unsigned int height = HIWORD(lParam);
                window->m_Width = width;
                window->m_Height = height;
                Eventmanager::PushEvent(new WindowResizeEvent(width, height, window->GetId()));
                PostQuitMessage(0);
                return 0;
            }
                
            case WM_KEYDOWN: {
                Keycode key = Keycode::unknow;
                if (wParam >= 'A' && wParam <= 'Z') {
                    key = (Keycode) ((int)Keycode::A + (wParam - 'A'));
                }
                Eventmanager::PushEvent(new KeyPressedEvent(key, window->GetId()));
                break;
            }
                
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

}