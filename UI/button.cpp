#include "button.h"

namespace SF {
    
    Button::Button(const std::string label, int x, int y, int width, int height, int radius, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick)
    : m_label(label), m_X(x), m_y(y), m_width(width), m_height(height), m_radius(radius), m_colorNormal(colorNormal), m_colorPressed(colorPressed),
    m_colorHover(colorHover), m_onClick(onClick), m_state(buttonState::none) { }

    Button::~Button() { }

    bool Button::IsInside(float mouseX, float mouseY) {
        return ((mouseX == m_X && mouseY == m_y) || (mouseX == m_X && mouseY <= m_y + m_height) ||
         (mouseX <= m_X + m_width && mouseY == m_y) || (mouseX <= m_X + m_width && mouseY <= m_y + m_height));
    }

    void Button::Update(Event& ev) {
        //verification de la position du curseur
        if (auto* e = ev.GetIf<MouseMoveEvent>()) {
            if (m_state != buttonState::pressed) {
                if (IsInside(e->GetPosX(), e->GetPosY())) {
                    m_state = buttonState::hover;
                } else {
                    m_state = buttonState::none;
                }
            }
            return;
        }

        //verification de l'etat de pression du bouton
        if (auto* e = ev.GetIf<MouseClickEvent>()) {
            if (IsInside(e->GetPosX(), e->GetPosY())) {
                m_state = buttonState::pressed;
            }
            return;
        }

        if (auto* e = ev.GetIf<MouseReleaseEvent>()) {
            if (IsInside(e->GetPosX(), e->GetPosY())) {
                m_state = buttonState::clicked;
            }
        }
    }

    void Button::Render(FrameBuffer& buffer, stbtt_fontinfo& font) {
        switch (m_state) {
            case buttonState::none :
                DrawRoundedRect(m_X, m_y, m_width, m_height, 10, buffer, m_colorNormal);
                break;
            case buttonState::hover :
                DrawRoundedRect(m_X, m_y, m_width, m_height, 10,  buffer, m_colorHover);
                break;
            case buttonState::clicked :
                DrawRoundedRect(m_X, m_y, m_width, m_height, 10, buffer, m_colorNormal);
                break;
            case buttonState::pressed :
                DrawRoundedRect(m_X, m_y, m_width, m_height, 10, buffer, m_colorPressed);
                break;
        }

        //on consid7re 22px par caracteres
        int lenght = static_cast<int>(m_label.size());
        int labelX = m_X + (m_width - lenght * 22) - 15;
        int labelY = m_y + m_height / 2 - 10;

        DrawText(font, m_label.c_str(), labelX, labelY, 21, pixels(255, 255, 255, 255), buffer);
    }

} // namespace SF
