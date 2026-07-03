#pragma once

#include "../uiElement.h"
#include <string>

namespace SF {
    
    enum NotificationType {
        standardNotification,
        standardNotificationDown,
        connectNotification,
        collectNotification,
        rewardNotification
    };

    class Notification : public UiElement {
    private:
        int m_x, m_y;
        float m_currentY;
        int m_width, m_height;
        float m_fadeInTime;
        
        std::string m_notifText;
        pixels m_mainBgColor, m_sidebarColor;
        Image m_notifIcon;
        NotificationType m_notifType;
        bool m_showed;

    public:
        //constructeurs / destructeurs
        Notification() {}
        Notification(std::string text, int x, int y, int width, int height, Image icon, NotificationType type = NotificationType::standardNotification,
            pixels bgColor = pixels(37, 32, 48), pixels sidebarColor = pixels(119, 60, 163));
        ~Notification(){}

        /**
         * @brief Verifie que la notification est totalement apparue
         */
        bool IsShowed();

        /**
         * @brief Modifie la couleur du bouton selon les états que celui ci peut bien avoir
         */
        void Render(FrameBuffer& buffer, stbtt_fontinfo& font) override;

        void Animation();
        /**
         * @brief modifie l'état du bouton entre normal, hover et clicked
         */
        void Update(Event& event) override;
    };

} // namespace SF
