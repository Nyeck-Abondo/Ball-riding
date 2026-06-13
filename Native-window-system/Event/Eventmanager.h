#pragma once

#include "event.h"
#include <memory>
#include <queue>
#include <mutex>
#include <vector>
#include <typeindex>
#include <algorithm>


/**
 * ===========================================
 * PATTERN PROXY POUR UTILISATION DE POOLeVENT
 * ===========================================
 */
namespace SF {
    class EventTracker {
        private:
        //evenement en cours de traitement
        Event* m_currentEvent;

        public:
        //CONSTRUCTEUR
        EventTracker();
        operator bool() {
            return m_currentEvent != nullptr;
        }

        /**
         * @brief operateur fleche pour acces au membre courant
         * @return le pointeur vers l'evenement courant
         */
        Event* operator->() {
            return m_currentEvent;
        }

        /**
         * @brief operateur d'indirection pour qcces direct au event
         * @return reference vers l'evenement courant
         */
        Event& operator*() {
            return *m_currentEvent;
        }

        /**
         * @brief passe a l'event suivant
         * @return la reference a l'eventtracker
         */
        EventTracker& operator++();

    };

    //==========================================================================
    //CLASSE: EventManager
    //DESCRIPTION: C'est le gestionnaire d'evenement global base sur le pattern
    //singleton et utilisant une file thread-safe
    //==========================================================================
    class Eventmanager {
    private:
        static std::mutex s_mutex;
        //evenement en cour de traitement
        static std::unique_ptr<Event> s_curentEvent;
        //File d'evenement repertorie
        static std::queue<std::unique_ptr<Event>> s_EventQueue;

    public:
        /**
         * @brief recherche en continue les evenements produits en rapport avec la fenetre
         * @return EventTracker
         */
        static EventTracker PollEvent();

        /**
         * @brief recupere l'evenement suivant dqns la file d'attente de traitement
         * @return le pointeur sur l'evenement aue l'on doit traiter
         */
        static Event* GetNextEvent();

        /**
         * @brief Retire l'evenement de la file d'attente
         * @return true si oui et false si non
         */
        static bool PollEvent(Event*& outEvent);

        /**
         * @brief Qjoute un evenement dans la file d'attente
         */
        static void PushEvent(Event* newEvent);

        /**
         * @brief Vide tous les evenements de la liste
         */
        static void ClearAllEvent();
    };

} // namespace SF
