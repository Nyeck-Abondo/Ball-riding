#include "EventManager.h"

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

    

} // namespace SF
