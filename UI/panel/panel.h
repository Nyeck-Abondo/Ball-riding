#pragma once

#include "../uiElement.h"
#include "../boutons/button.h"

namespace sf {
    
    class Panel : public UiElement {
        private:
        maths::Vector2D m_pos;
        maths::Vector2D m_currentPos;
        maths::Vector2D m_spriteLocate;
        int m_fontSize;
        int m_width, m_height;
        int m_radius;
        bool m_IsShow;
        float m_fadeInTime;

        std::string m_title;
        Image &m_picture, &m_icon;
        stbtt_fontinfo& m_fontBtn;
        std::vector<std::unique_ptr<Button>> m_button;
        pixels m_color;
        pixels m_titleColor;

        public:
            Panel(std::string title, maths::Vector2D pos, int width, int height, int radius
                ,float fadeIn, float fontSize, pixels color,pixels titleColor,Image& picture,
                Image& icon, maths::Vector2D spritelocate, stbtt_fontinfo& font);
            ~Panel() { }

            //GETTERS
            maths::Vector2D GetPosition() { return m_pos; }
            bool GetShowed() { return m_IsShow; }
            int GetWidth() { return m_width; }
            int GetHeight() { return m_height; }
            bool SetShowed() { return m_IsShow ? m_IsShow = false: m_IsShow = false; }

            void Update(Event& event) override;
            void Animate();
            void Render(render::Renderer& renderer, stbtt_fontinfo& font) override;
            void AddButton(const std::string label, maths::Vector2D pos, int width, int height, int radius, int fontSize, pixels colorNormal, pixels colorHover, pixels colorPressed, std::function<void()> onClick);
    };

} // namespace sf
