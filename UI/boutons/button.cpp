#include "button.h"

namespace SF {
    
    Button::Button(const std::string label, int x, int y, int width, int height, int radius, int fontSize, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick)
    : m_label(label), m_X(x), m_y(y), m_width(width), m_height(height), m_radius(radius), m_colorNormal(colorNormal), m_colorPressed(colorPressed),
    m_colorHover(colorHover), m_onClick(onClick), m_state(buttonState::none), m_fontSize(fontSize) { }

    Button::~Button() { }

    bool Button::IsInside(float mouseX, float mouseY) {
        return (mouseX >= m_X && mouseX <= m_X + m_width &&
            mouseY >= m_y && mouseY <= m_y + m_height);
    }

    void Button::Update(Event& ev) {
        //verification de la position du curseur
        if (auto* e = ev.GetIf<MouseMoveEvent>()) {
            if (m_state != buttonState::pressed) {
                if (IsInside(e->GetPosX(), e->GetPosY())) {
                    m_state = buttonState::hover;
                }
                else {
                    m_state = buttonState::none;
                }
            }
            return;
        }

        if (auto* e = ev.GetIf<MouseClickEvent>()) {
            if (e->GetButton() == MouseButton::leftBtn && IsInside(e->GetPosX(), e->GetPosY())) {
                m_state = buttonState::pressed;
            }
            else if (!IsInside(e->GetPosX(), e->GetPosY())) {
                m_state = buttonState::none;
            }
            if (IsInside(e->GetPosX(), e->GetPosY())) {
                m_state = buttonState::hover;
            }
            else {
                m_state = buttonState::none;
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
        pixels color;
        switch (m_state) {
            case buttonState::none : 
                color = m_colorNormal;
                break;
            case buttonState::hover :
                {color = m_colorHover;}
                break;
            case buttonState::realeased :
                {color = m_colorNormal;}
                break;
            case buttonState::pressed :
                {color = m_colorPressed;}
                break;
            case buttonState::clicked :
                {color = pixels(45, 31, 69);}
                break;
        }
        DrawRoundedRect(m_X, m_y, m_width, m_height, m_radius, buffer, color);

        //on consid7re 22px par caracteres
        int lenght = static_cast<int>(m_label.size());
        std::cout << lenght << std::endl;
        int labelX = m_X + (m_width - lenght * (m_fontSize - 1) / 2) / 2;
        int labelY = m_y + m_height / 2 - 10;

        DrawText(font, m_label.c_str(), labelX, labelY, m_fontSize, pixels(255, 255, 255, 255), buffer);
    }

} // namespace SF
