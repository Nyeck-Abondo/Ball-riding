#include "math2D.h"

namespace sf {
    
    namespace maths {

        float lerp(const Vector2D& vect0, const Vector2D& vect1, float x) {
            float x0 = vect0.m_x, x1 = vect1.m_x;
            float y0 = vect0.m_y, y1 = vect1.m_y;

            if (std::fabs(x1 - x0) < 1e-5f) return (y0 + y1) * 0.5f;

            float t = (x - x0) / (x1 - x0);
            return (1 - t) * y0 + t * y1;
        }

        Vector2D Rotation(const Vector2D& center, const Vector2D& point, float angle) {
            Vector2D som = point - center;
            return Vector2D(
                center.m_x + som.m_x * std::cos(angle) - som.m_y * std::sin(angle),
                center.m_y + som.m_x * std::sin(angle) + som.m_y * std::cos(angle)
            );
        }
        
    } // namespace maths
    

} // namespace sf
