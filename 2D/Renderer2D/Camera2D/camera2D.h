#pragma once

#include "../../render2D/algorithm2D.h"

namespace sf {

    namespace render {

        class Camera {
            private:
            float m_zoom;
            maths::Vector2D m_size;
            maths::Vector2D m_position;

            public:
            //CONSTRUCTEUR ET DESTRUCTEURS DE LA CAMERA
            Camera(maths::Vector2D size, maths::Vector2D pos) 
            : m_size(size), m_position(pos), m_zoom(1.f) {}
            ~Camera() {}

            //GETTERS
            maths::Vector2D GetPosition() { return m_position; }
            maths::Vector2D GetSize() { return m_size; }
            float GetZoom() { return m_zoom; }
            
            //SETTERS
            void SetPosition(maths::Vector2D newPos) { m_position = newPos; }
            void SetSize(maths::Vector2D newSize) { m_size = newSize; }
            void SetZoom(float zoom) {
                if (zoom > 1 || zoom < 0) { 
                    m_zoom = 1.f;
                    return;
                }
                m_zoom = zoom;
            }

            maths::Vector2D WorldToScreenPos(maths::Vector2D worldPos);

            maths::Vector2D ScreenToWorldPos(maths::Vector2D screenPos);

            maths::Vector2D GetVisibleTopCorner();

            bool IsInCameraRange(maths::Vector2D worldPos);
        };
        
    } // namespace render
    
} // namespace sf
