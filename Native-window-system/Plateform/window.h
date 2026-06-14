#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define SF_PLATEFORM_WINDOWS 1
#endif

#if defined(__linux__) && !defined(__ANDROID__)
    #define SF_PLATFORM_LINUX 1
#endif

#include <cstdint>
#include "../event/event.h"
#include "../Event/Eventmanager.h"
#include "windows/FrameBuffer.h"

namespace SF {
    class Window {
    
    public:
        virtual ~Window() = default;

        //METHOD
        /**
         * @brief Methode pour creer tous les tpes de fenetre
         * @param title c4est le nom de la fenetre
         * @param width largeur de la fenetre
         * @param height hauteur de la fenetre
         */
        static Window* Create(const char* Title, uint32_t width, uint32_t height);

        /**
         * @brief initialise la fenetre
         */
        virtual bool Initialize() = 0;

        /**
         * @brief libere les ressources de la fenetre
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Verifie si la fenetre est encore ouverte
         * @return true si la fenetre est ouverte et false sinon
         */
        virtual bool IsOpen() = 0;

        /**
         * traite les evenements en lien avec la fenetre
         */
        virtual void ProcessEvent() = 0;

        /**
         * @brief Nettoie la fenêtre avec une couleur spécifique
         * @param color structure de pixel repésentant la couleur avec canal alpha
         * qui sera utilisée pour néttoyer l'écran
         */
        virtual void clear(const pixels& color) = 0;

        /**
         * @brief Présente le tableau de pixel coloré dans la fenètre
         */
        virtual void Present() = 0;

        /**
         * donne un etat d'ouverture et de fermeture
         */
        virtual void SetOpen() = 0;
        /**
         * @brief recupere la largeur de la fenetre
         */
        virtual uint32_t GetWidth() = 0;

        /**
         * @brief recupere la hauteur de la fenetre
         */
        virtual uint32_t GetHeight() = 0;

        virtual uint32_t GetId() = 0;

    };
    
} // namespace SF
