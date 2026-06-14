#pragma once

#include <mutex>
#include <vector>
#include <cstdint>

namespace SF {
    class Window;
    
    class WindowManager {

        private:
            static std::vector<Window*> m_windowRegister;
            static std::mutex m_mutex;
            static uint32_t m_nextWindowId;
        
        public:
            /**
             * @brief recupère tous les messages en lien avec les évènements
             * de la fenêtre
             */
            static void ProcessAllEvents();

            /**
             * @brief Enregistre la fenetre courante dans le gestionnaire
             * @param mainWindow pointeur vers la fenetre a enregistrer
             */
            static void RegisterWindow(Window* mainWindow);

            /**
             * @brief Retire la fenetres de la liste de celles abonnees au gestionnaire
             * @param mainWindow pointeur vers la fenetre a reitrer
             */
            static void UnregisterWindow(Window* mainWindow);

            /**
             * @brief recupere la fenetre par son identifiant si elle est presente dans
             * la liste des fenetre du gestionnaire de fenetre
             * @param id est l'identifiant unique de la fenetre a recuperer
             */
            static Window* GetWindow(uint32_t id);
    };

} // namespace SF
