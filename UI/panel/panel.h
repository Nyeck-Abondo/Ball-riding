#pragma once

#include "../uiElement.h"
#include "../boutons/button.h"

namespace SF {
    
    class Panel : public UiElement {
        private:
        int m_x, m_y;
        float m_currentX, m_currentY;
        float m_fadeInTime;

        std::string m_title;
        Image picture, m_icon;
        Button m_btn01, m_btn02, m_btn03;

        public:
            void Update(Event& event) override;
            void Render(FrameBuffer& buffer, stbtt_fontinfo& font) override;
    };

} // namespace SF
