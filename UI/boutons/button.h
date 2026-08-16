#pragma once

#include "../uiElement.h"
#include "../../Native-window-system/Event/Eventmanager.h"
#include <functional>

namespace sf {

    enum class buttonState {
        none = 0, hover, clicked, pressed, realeased
    };
    
    class Button : public UiElement{
        private:
            buttonState m_state;
            maths::Vector2D m_pos;
            int m_width, m_height, m_radius;
            int m_fontSize;
            std::string m_label;
            pixels m_colorNormal, m_colorHover, m_colorPressed;
            std::function<void()> m_onClick;

        public:

        //constructeur
        Button() {}
        Button(const std::string label, maths::Vector2D pos, int width, int height, int radius, int fontSize, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick);
        ~Button();

        //GETTERS
        
        maths::Vector2D& GetPosition() { return m_pos; }
        int GetWidth() { return m_width; }
        int GetHeight() { return m_height; }

        //methodes

        /**
         * @brief vérifie si la souris se trouve dans l'aire du bouton
         * @param mouseX la position du curseur sur l'abscisse
         * @param mouseY la position du curseur sur les ordonnées
         * @return true si le curseur se trouve sur le bouton et false sinon
         */
        bool IsInside(float mouseX, float mouseY);

        /**
         * @brief Modifie la couleur du bouton selon les états que celui ci peut bien avoir
         * @param buffer le tampon en mémoire pour l'affichage des pixels à l'écran
         * @param font la police utilisée pour l'écriture du contenu du bouton
         */
        void Render(render::Renderer& renderer, stbtt_fontinfo& font) override;

        /**
         * @brief modifie l'état du bouton entre normal, hover et clicked
         * @param event l'évènment qui permet le changement d'état du bouton
         */
        void Update(Event& event) override;
    };

} // namespace sf
