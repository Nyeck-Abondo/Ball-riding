#pragma once

#include <cstdint>

namespace SF {
    enum class EventType {
        KeyUp, windowResize, mouseRelease,
        mouseMove, mouseclick, keydown
    };

    enum class Keycode {
        unknow = 0,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V,
        W, X, Y, Z, Escape, Space, Enter, Left, Right, Up, Down
    };

    enum class MouseButton {leftBtn = 0, RightBtn, middle};

    //========================================================
    //  CLASSE EVENEMENTS: Event
    //  DESCRIPTION: classe abraite pour les evenements du SF
    //========================================================
    class Event
    {
    protected:
        EventType m_Type;
        bool m_handled;
    public:
        Event(EventType type) : m_Type(type), m_handled(false) {}
        virtual ~Event() = default;

        //GETTERS
        EventType GetType() { return m_Type; }
        bool IsHandle() { return m_handled; }

        //SETTERS
        void SetHandle() { m_handled = true; }
        template <typename T> bool IsType() { return m_Type == T::GetStaticType(); }
        template <typename T> T& As() { return *static_cast<T*>(this); }
        template <typename T> T& As() const { return *static_cast<const T*>(this); }

        //METHODE COMMUNE
        template <typename T>
        const T GetIf() {
            if (IsType(T)) {
                As(T);
            }
            return nullptr;
        }
    };

    /**
     * ==================================================================
     * CREATION DES CLASSES ENFANT DECRIVANT LES DIFFERENTS EVENEMENTS
     * INTERVENANT DANS LES FENETRES DU JEU
     * ==================================================================
     */

    /**
      * EVENEMENT DE REDIMENSIONNEMENT DE FENETRE
      */
    class WindowResizeEvent : public Event {
        private:
        uint32_t m_width, m_height;

        public:
        //constructeur
        WindowResizeEvent(uint32_t nW, uint32_t nH): Event(EventType::windowResize), m_width(nW), m_height(nH) {}

        //GETTERS
        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }

        EventType GetStaticType() { return EventType::windowResize; }
    };

    /**
     * EVENEMENT DE PRESSION DE TOUCHE DU CLAVIER
     */
    class KeyPressedEvent : public Event {
        private:
        //code de la touche enfoncee
        Keycode keyCode;
        //indicateur de repetition auto
        bool repeated;

        public:
        //constructeur
        KeyPressedEvent(Keycode key, bool repeat = false)
            : Event(EventType::keydown), keyCode(key), repeated(repeat) {}

        //GETTERS
        Keycode GetKeyCode() { return keyCode; }
        bool IsRepeated() { return repeated; }
        static EventType GetStaticType() { return EventType::keydown; }
    };

    /**
     * RELACHEMENT DE LA TOUCHE
     */
    class KeyReleaseEvent : public Event {
        private:
        Keycode KeyCode;

        public:
        //constructeur
        KeyReleaseEvent(Keycode key): Event(EventType::KeyUp), KeyCode(key) {}

        //GETTERS
        Keycode GetKeyCode() { return KeyCode; }
        //METHODES
        static EventType GetStaticType() { return EventType::KeyUp; }
    };

    /**
     * MOUVEMENT DE LA SOURI
     */
    class MouseMoveEvent: public Event {
        private:
        float posX;
        float posY;

        public:
        MouseMoveEvent(float x, float y)
            :Event(EventType::mouseMove), posX(x), posY(y) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPoxY() { return posY; }

        static EventType GetEventType() { return EventType::mouseMove; }
    };

    class MouseClickEvent : public Event {
        private:
        float posX;
        float posY;
        MouseButton button;

        public:
        //constructeur
        MouseClickEvent(float x, float y, MouseButton btn) 
            :Event(EventType::mouseclick), posX(x), posY(y), button(btn) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPoxY() { return posY; }
        MouseButton GetButton() { return button; }

        static EventType GetStaticType() { return EventType::mouseclick; }
    };

    class MouseReleaseEvent : public Event {
        private:
        float posX;
        float posY;
        MouseButton button;

        public:
        //constructeur
        MouseReleaseEvent(float x, float y, MouseButton btn) 
            :Event(EventType::mouseRelease), posX(x), posY(y), button(btn) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPoxY() { return posY; }
        MouseButton GetButton() { return button; }

        static EventType GetStaticType() { return EventType::mouseRelease; }
    };

} // namespace SF
