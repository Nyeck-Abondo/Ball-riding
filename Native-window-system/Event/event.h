#pragma once

#include <cstdint>

namespace sf {
    enum class EventType {
        KeyUp, windowResize, mouseRelease,
        mouseMove, mouseclick, keydown, windowClosed
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
        uint32_t id;
        bool m_handled;
    public:
        Event(EventType type, uint16_t Id) : m_Type(type), m_handled(false), id(Id) {}
        virtual ~Event() = default;

        //GETTERS
        EventType GetType() { return m_Type; }
        uint16_t GetId() { return id; }
        bool IsHandle() { return m_handled; }

        //SETTERS
        void SetHandle() { m_handled = true; }
        template <typename T> bool IsType() { return m_Type == T::GetStaticType(); }
        template <typename T> T& As() { return *static_cast<T*>(this); }
        template <typename T> const T& As() const { return *static_cast<const T*>(this); }

        //METHODE COMMUNE
        template <typename T>
        const T* GetIf() const {
            if (IsType<T>()) {
                return& As<T>();
            }
            return nullptr;
        }

        template <typename T>
        T* GetIf() {
            if (IsType<T>()) {
                return& As<T>();
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
     * EVENEMENT DE FERMETURE DE LA FENETRE
     */
    class WindowClosedEvent : public Event {
        public:
        WindowClosedEvent(uint32_t id): Event(EventType::windowClosed, id) {}

        static EventType GetStaticType() { return EventType::windowClosed; }
    };

    /**
      * EVENEMENT DE REDIMENSIONNEMENT DE FENETRE
      */
    class WindowResizeEvent : public Event {
        private:
        uint32_t m_width, m_height;

        public:
        //constructeur
        WindowResizeEvent(uint32_t nW, uint32_t nH, uint32_t id): Event(EventType::windowResize, id), m_width(nW), m_height(nH) {}

        //GETTERS
        uint32_t GetWidth() { return m_width; }
        uint32_t GetHeight() { return m_height; }

        static EventType GetStaticType() { return EventType::windowResize; }
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
        KeyPressedEvent(Keycode key, uint32_t id,bool repeat = false)
            : Event(EventType::keydown, id), keyCode(key), repeated(repeat) {}

        //GETTERS
        Keycode GetKeyCode() const { return keyCode; }
        bool IsRepeated() const { return repeated; }
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
        KeyReleaseEvent(Keycode key, uint32_t id): Event(EventType::KeyUp, id), KeyCode(key) {}

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
        MouseMoveEvent(float x, float y, uint32_t id)
            :Event(EventType::mouseMove, id), posX(x), posY(y) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPosY() { return posY; }

        float GetPosX() const { return posX; }
        float GetPosY() const { return posY; }

        static EventType GetStaticType() { return EventType::mouseMove; }
    };

    class MouseClickEvent : public Event {
        private:
        float posX;
        float posY;
        MouseButton button;

        public:
        //constructeur
        MouseClickEvent(float x, float y, MouseButton btn, uint32_t id) 
            :Event(EventType::mouseclick, id), posX(x), posY(y), button(btn) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPosY() { return posY; }
        float GetPosX() const { return posX; }
        float GetPosY() const { return posY; }
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
        MouseReleaseEvent(float x, float y, MouseButton btn, uint32_t id) 
            :Event(EventType::mouseRelease, id), posX(x), posY(y), button(btn) {}
        
        //GETTERS
        float GetPosX() { return posX; }
        float GetPosY() { return posY; }
        float GetPosX() const { return posX; }
        float GetPosY() const { return posY; }
        MouseButton GetButton() { return button; }

        static EventType GetStaticType() { return EventType::mouseRelease; }
    };

} // namespace sf
