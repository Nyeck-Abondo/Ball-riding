#include "Window-manager.h"
#include "../window.h"

namespace sf {
    
    //==================================================
    //INITIALISATION DES VARIABLES STATIQUES
    //==================================================
    std::vector<Window*> WindowManager::m_windowRegister;
    std::mutex WindowManager::m_mutex;
    uint32_t WindowManager::m_nextWindowId = 0;

    void WindowManager::UnregisterWindow(Window* mainWindow) {
        std::lock_guard<std::mutex> guard(m_mutex);
        auto it = std::find(m_windowRegister.begin(), m_windowRegister.end(), mainWindow);
        if (it != m_windowRegister.end()) {
            m_windowRegister.erase(it);
        }
    }

    void WindowManager::RegisterWindow(Window* mainWindow) {
        std::lock_guard<std::mutex> guard(m_mutex);
        m_windowRegister.push_back(mainWindow);
    }

    Window* WindowManager::GetWindow(uint32_t id) {
        std::lock_guard<std::mutex> guard(m_mutex);
        for(auto& it : m_windowRegister) {
            if (it->GetId() == id) {
                return it;
            }
        }
        return nullptr;
    }

    void WindowManager::ProcessAllEvents() {
        std::lock_guard<std::mutex> guard(m_mutex);
        for (auto& win : m_windowRegister) {
            if (win->IsOpen()) {
                win->ProcessEvent();
            }
        }
    }

} // namespace sf
