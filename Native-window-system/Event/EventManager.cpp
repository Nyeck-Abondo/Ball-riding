#include "Eventmanager.h"

namespace SF {
    
    //=======================================
    //INITIALISATION DES VARIABLES STATIQUES
    //=======================================

    //mutex pour le thread safe 
    std::mutex Eventmanager::s_mutex;
    //Evenement courant
    std::unique_ptr<Event> Eventmanager::s_curentEvent;
    //File d'attente d'evenement
    std::queue<std::unique_ptr<Event>> Eventmanager::s_EventQueue;

    /**
     * Recuperation du premier evenement de la file des la construction
     */
    EventTracker::EventTracker() {
        m_currentEvent = Eventmanager::GetNextEvent();
    }
    
    EventTracker& EventTracker::operator++() {
        m_currentEvent = Eventmanager::GetNextEvent();
        return *this;
    }

    EventTracker Eventmanager::PollEvent() {
        return EventTracker();
    }

    bool Eventmanager::PollEvent(Event*& outEvent) {
        //traitement des evenements des fenetres
        WindowManager::ProcessAllEvents();

        //recuperation thread safe des evenements
        {
            std::lock_guard<std::mutex> guard(s_mutex);
            if (!s_EventQueue.empty()) {
                outEvent = nullptr;
                return false;
            }

            s_curentEvent = std::move(s_EventQueue.front());
            outEvent = s_curentEvent.get();

            return true;
        }
    }

    void Eventmanager::PushEvent(Event* newEvent) {
        std::lock_guard<std::mutex> guard(s_mutex);
        s_EventQueue.push(std::unique_ptr<Event>(newEvent));
    }

    Event* Eventmanager::GetNextEvent() {
        //traitement des evenements des fenetres
        WindowManager::ProcessAllEvents();
        Event* result = nullptr;

        //recuperation thread safe de l'evenement
        {
            std::lock_guard<std::mutex> guard(s_mutex);
            if (!s_EventQueue.empty()) {
                s_curentEvent = std::move(s_EventQueue.front());
                s_EventQueue.pop();
                result = s_curentEvent.get();
            }
        }
        return result;
    }

} // namespace SF
