#include "panel.h"

namespace sf {
    
    Panel::Panel(std::string title, maths::Vector2D pos, int width, int height, int radius, float fadeIn,
         float fontSize, pixels color, pixels titleColor, Image& picture, Image& icon, maths::Vector2D spritelocate,
        stbtt_fontinfo& font)
    : m_title(title), m_pos(pos), m_fadeInTime(fadeIn), m_fontSize(fontSize),
    m_color(color), m_titleColor(titleColor), m_picture(picture), m_icon(icon),
    m_width(width), m_height(height), m_radius(radius), m_spriteLocate(spritelocate),
    m_fontBtn(font) {
        m_currentPos.m_x = m_width * -1;
        m_currentPos.m_y = 0.0f;
        m_IsShow = false;
        m_icon.pos.m_x += m_currentPos.m_x;
    }

    void Panel::AddButton(const std::string label, maths::Vector2D pos, int width, int height, int radius,
        int fontSize, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick) {
        maths::Vector2D relativPos = pos + m_currentPos; 
        m_button.push_back(std::make_unique<Button>(label, relativPos, width, height, radius, fontSize, colorNormal,
        colorHover, colorPressed, onClick));
    }

    void Panel::Render(render::Renderer& renderer, stbtt_fontinfo& font) {
        renderer.DrawRoundedRectangle(
            m_currentPos,
            m_width, m_height,
            m_radius, 
            60,
            m_color
        );

        int lenght = m_title.size();
        int cursorX = m_currentPos.m_x + (m_width - (lenght * m_fontSize) / 2);
        int cursorY = m_currentPos.m_y + m_fontSize;
        DrawText(font, m_title.c_str(), cursorX, cursorY, m_fontSize, m_titleColor, renderer);

        for (auto& btn : m_button) {
            btn->Render(renderer, m_fontBtn);
        }

        DrawImageAt(m_icon, m_spriteLocate, 128, 128, renderer);
    }

    void Panel::Update(Event& event) {
        for (auto& btn : m_button) {
            btn->Update(event);
        }
    }

    void Panel::Animate() {
        if (m_currentPos.m_x < m_pos.m_x) {
            float delta = (int)(3000 / m_fadeInTime);

            for (auto& btn : m_button) {
                maths::Vector2D& btns = btn->GetPosition();
                btns.m_x += delta;
            }
            m_icon.pos.m_x += delta;
            m_currentPos.m_x = m_currentPos.m_x + delta;
        }
    }

} // namespace sf
