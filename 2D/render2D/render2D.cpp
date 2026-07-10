#include "algorithm2D.h"

namespace SF {
    
    pixels BlendPixel(pixels src, pixels dst) {
        uint16_t a  = src.a;
        uint16_t ia = 255 - a;
        return pixels(
            (uint8_t)((src.r * a + dst.r * ia) / 255),
            (uint8_t)((src.g * a + dst.g * ia) / 255),
            (uint8_t)((src.b * a + dst.b * ia) / 255),
            255
        );
    }

    void DrawLine(const std::unique_ptr<Node>& node, const std::unique_ptr<Node>& other, FrameBuffer& bf) {
        pixels* buffer = bf.GetBackBuffer();
        int width  = bf.GetBufferWidth();
        int height = bf.GetBufferHeight();

        int x0 = node->mainPos.m_x, y0 = node->mainPos.m_y;
        int x1 = other->mainPos.m_x,  y1 = other->mainPos.m_y;

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        if (dx > 10000 || dy > 10000) return;
            
        int uintx = (node->mainPos.m_x < other->mainPos.m_x) ? 1 : -1;
        int uintY = (node->mainPos.m_y < other->mainPos.m_y) ? 1 : -1;

        int x = node->mainPos.m_x;
        int y = node->mainPos.m_y;

        if (dx > dy) {
            int p = 2 * dy - dx;

            for (int i = 0; i <= dx; i++) {
                int index = y * width + x;
                if (x < 0 || x >= width || y < 0 || y >= height) continue;
                buffer[index] = pixels(node->color);

                if (p >= 0) {
                    y += uintY;
                    p -= 2 * dx;
                }
                x += uintx;
                p += 2 * dy;
            }
        } else {
            int p = 2 * dy - dx;

            for (int i = 0; i <= dy; i++) {
                int index = y * width + x;
                if (x < 0 || x >= width || y < 0 || y >= height) continue;
                buffer[index] = BlendPixel(pixels(node->color), buffer[index]);

                if (p >= 0) {
                    x += uintx;
                    p -= 2 * dy;
                }
                y += uintY;
                p += 2 * dx;
            }
        }

    }

        

    void DrawLine(Vector2D& other, Vector2D& vect, pixels color, FrameBuffer& bf) {
        pixels* buffer = bf.GetBackBuffer();
        int width  = bf.GetBufferWidth();
        int height = bf.GetBufferHeight();

        int x0 = other.m_x, y0 = other.m_y;
        int x1 = vect.m_x,  y1 = vect.m_y;

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        if (dx > 10000 || dy > 10000) return;
            
        int uintx = (other.m_x < vect.m_x) ? 1 : -1;
        int uintY = (other.m_y < vect.m_y) ? 1 : -1;

        int x = other.m_x;
        int y = other.m_y;

        if (dx > dy) {
            int p = 2 * dy - dx;

            for (int i = 0; i <= dx; i++) {
                int index = y * width + x;
                buffer[index] = pixels(color);

                if (p >= 0) {
                    y += uintY;
                    p -= 2 * dx;
                }
                x += uintx;
                p += 2 * dy;
            }
        } else {
            int p = 2 * dy - dx;

            for (int i = 0; i <= dy; i++) {
                int index = y * width + x;
                buffer[index] = BlendPixel(pixels(color), buffer[index]);

                if (p >= 0) {
                    x += uintx;
                    p -= 2 * dy;
                }
                y += uintY;
                p += 2 * dx;
            }
        }
    }

} // namespace SF
