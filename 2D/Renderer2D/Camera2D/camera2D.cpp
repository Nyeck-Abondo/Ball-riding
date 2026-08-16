#include "camera2D.h"

namespace sf {

    namespace render {

        maths::Vector2D Camera::WorldToScreenPos(maths::Vector2D worldPos) {
            float screenX = (worldPos.m_x - m_position.m_x) * m_zoom + m_size.m_x / 2.f;
            float screenY = (worldPos.m_y - m_position.m_y) * m_zoom + m_size.m_y / 2.f;
            return maths::Vector2D(screenX, screenY);
        }

        maths::Vector2D Camera::ScreenToWorldPos(maths::Vector2D screenPos) {
            float worldX = (screenPos.m_x - m_size.m_x / 2.f) / m_zoom + m_position.m_x;
            float worldY = (screenPos.m_y - m_size.m_y / 2.f) / m_zoom + m_position.m_y;
            return {worldX, worldY};
        }

        maths::Vector2D Camera::GetVisibleTopCorner() {
            float posX = m_position.m_x - (m_size.m_x / 2.f / m_zoom);
            float posY = m_position.m_y - (m_size.m_y / 2.f / m_zoom);

            return {posX, posY};
        }

        bool Camera::IsInCameraRange(maths::Vector2D worldPos) {
            maths::Vector2D top = GetVisibleTopCorner();
            return (
                (worldPos.m_x < top.m_x + m_size.m_x && worldPos.m_x > top.m_x) &&
                (worldPos.m_y > top.m_y && worldPos.m_y < top.m_y + m_size.m_y)
            );
        }

    } // namespace render   
    
} // namespace sf
