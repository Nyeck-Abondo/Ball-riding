#pragma once

#include "uiElement.h"

namespace SF {
    
    class notification {
    private:
        int m_x, m_y;
        int m_width, m_height;
        std::string m_notifText;

    public:
        notification();
        ~notification();
    };

} // namespace SF
