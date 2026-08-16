#include "notification.h"

namespace sf {
    
    Notification::Notification(std::string text, int x, int y, int width, int height, int fontSize, Image icon, NotificationType type,
        pixels bgColor, pixels sidebarColor)
        : m_notifText(text), m_x(x), m_y(y), m_width(width), m_height(height),
        m_mainBgColor(bgColor), m_sidebarColor(sidebarColor), m_notifType(type),
        m_notifIcon(icon), m_fontSize(fontSize) {
            m_fadeInTime = 500.0f;
            m_showed = false;
            if (m_notifType == NotificationType::standardNotification || m_notifType == NotificationType::rewardNotification) {
                m_currentY = m_y - 150.0f;
            }
            else {
                m_currentY = m_y + 150.0f;
            }
        }

    bool Notification::IsShowed() {
        return m_currentY == m_y;
    }

    void Notification::Update(Event& event) {
        if (m_currentY < m_y) {
            m_currentY = m_currentY + (400 / m_fadeInTime);
        } else {
            m_showed = false;
        }

    }

    void Notification::Animation() {
        switch (m_notifType) {
        case NotificationType::standardNotification :
            if (m_currentY < m_y) m_currentY += (300.0f / m_fadeInTime) + 0.055f;
            if (m_currentY >= m_y) m_currentY = m_y;
            break;
        
        case NotificationType::standardNotificationDown :
            if (m_currentY > m_y) m_currentY -= (300.0f / m_fadeInTime) + 0.055f;
            if (m_currentY <= m_y) m_currentY = m_y;
            break;

        case NotificationType::connectNotification :
            if (m_currentY < m_y) m_currentY += (300.0f / m_fadeInTime) + 0.055f;
            if (m_currentY >= m_y) m_currentY = m_y;
            break;
        
        case NotificationType::collectNotification :
            if (m_currentY > m_y) m_currentY -= (300.0f / m_fadeInTime) + 0.055f;
            if (m_currentY <= m_y) m_currentY = m_y;
            break;

        default:
            if (m_currentY < m_y) m_currentY += (300.0f / m_fadeInTime) + 0.055f;
            if (m_currentY >= m_y) m_currentY = m_y;
            break;
        }
    }

    void Notification::Render(render::Renderer& renderer, stbtt_fontinfo& font) {
        renderer.DrawRoundedRectangle({static_cast<float>(m_x), static_cast<float>(m_currentY)}, m_width, m_height, 20, 10, m_mainBgColor);
        renderer.DrawRoundedRectangle({static_cast<float>(m_x + 15), static_cast<float>(m_currentY + 7)}, 10, m_height - 20, 3, 5, m_sidebarColor);

        int lenght = static_cast<int>(m_notifText.size());
        int labelX = m_x + std::abs(m_width - lenght * (m_fontSize - 1) / 2) / 2;
        int labelY = m_currentY + m_height / 2 - 19;
        DrawText(font, m_notifText.c_str(), labelX, labelY, m_fontSize, pixels(255, 255, 255), renderer);
    }
} // namespace sf
