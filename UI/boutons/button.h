#pragma once

#include "../uiElement.h"
#include "../Native-window-system/Event/Eventmanager.h"
#include <functional>

namespace SF {

    class Notification;

    enum class buttonState {
        none = 0, hover, clicked, pressed, realeased
    };
    
    class Button : public UiElement{
        private:
            buttonState m_state;
            Vector2D m_pos;
            int m_width, m_height, m_radius;
            int m_fontSize;
            std::string m_label;
            pixels m_colorNormal, m_colorHover, m_colorPressed;
            std::function<void()> m_onClick;

        public:

        //constructeur
        Button() {}
        Button(const std::string label, Vector2D pos, int width, int height, int radius, int fontSize, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick);
        ~Button();

        //GETTERS
        Vector2D& GetPosition() { return m_pos; }
        int GetWidth() { return m_width; }
        int GetHeight() { return m_height; }

        //methodes

        /**
         * @brief vérifie si la souris se trouve dans l'aire du bouton
         * @return true si le curseur se trouve sur le bouton et false sinon
         */
        bool IsInside(float mouseX, float mouseY);

        /**
         * @brief Modifie la couleur du bouton selon les états que celui ci peut bien avoir
         */
        void Render(FrameBuffer& buffer, stbtt_fontinfo& font) override;

        /**
         * @brief modifie l'état du bouton entre normal, hover et clicked
         */
        void Update(Event& event) override;
    };

} // namespace SF
