#pragma once
#include "../window.h"
#include <cstdint>
#include <windows.h>
#include <string>
#include "../../Window-manager/Window-manager.h"

namespace SF {
    
    class windowWin32 : public Window {
    private:
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_id;
        std::string m_title;
        bool m_IsOpen;

        FrameBuffer m_FrameBuffer;
        HWND m_hwnd;
        HINSTANCE m_hInstance;
        WNDCLASSEX m_wc;

        /**
         * @brief Procedure de fenetre
         */
        static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        
    public:
        //CONSTRUCTEURS ET DESTRUCTEURS
        windowWin32(const char* title, uint32_t width, uint32_t height);
        ~windowWin32() override;

        //METHODES
        /**
         * @brief Initialise la fenetre
         */
        bool Initialize() override;
        
        /**
         * @brief libere les ressources de la fenetre
         */
        void Shutdown() override;



        /**
         * @brief Recupere les evenements detectés
         */
        void ProcessEvent() override;

        /**
         * @brief Nettoie la fenêtre avec une couleur spécifique
         * @param color structure de pixel repésentant la couleur avec canal alpha
         * qui sera utilisée pour néttoyer l'écran
         */
        void clear(const pixels& color) override;

        /**
         * @brief Présente le tableau de pixel coloré dans la fenètre
         */
        void Present();

        /**
         * 2
         */
        uint32_t GetWidth() override;
        
        /**
         * 
         */
        uint32_t GetHeight() override;

        uint32_t GetId() override;
    };
    
} // namespace SF
