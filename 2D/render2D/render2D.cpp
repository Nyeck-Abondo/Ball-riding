#include "algorithm2D.h"

namespace sf {
    
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
                buffer[index] = BlendPixel(pixels(node->color), buffer[index]);

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

    void DrawFillCircle(maths::Vector2D center, int radius, pixels color, FrameBuffer& fb) {
        pixels* buffer = fb.GetBackBuffer();
        for (int y = center.m_y - radius; y < center.m_y + radius; y++) {
            for (int x = center.m_x - radius; x < center.m_x + radius; x++) {
                int dx = x - center.m_x;
                int dy = y - center.m_y;
                if ((dx * dx + dy * dy) <= radius * radius) {
                    int index = y * fb.GetBufferWidth() + x;
                    buffer[index] = BlendPixel(color, buffer[index]);
                }
            }
        }
    }

    void DrawLine(maths::Vector2D& other, maths::Vector2D& vect, pixels color, FrameBuffer& bf) {
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
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    int index = y * width + x;
                    buffer[index] = BlendPixel(color, buffer[index]);

                    if (p >= 0) {
                        y += uintY;
                        p -= 2 * dx;
                    }
                    x += uintx;
                    p += 2 * dy;
                }
                
            }
        } else {
            int p = 2 * dy - dx;

            for (int i = 0; i <= dy; i++) {
                if (x >= 0 && x < width && y >= 0 && y < height) {
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
    }

        /**
     * @name DrawRoundedRect
     * @brief Dessine un rectangle au sommets arondis
     * @param x la position sur les abscisses
     * @param y la position sur les ordonnées
     * @param width la larageur du rectangle
     * @param height la hauteur du rectangle
     * @param radius le rayon de coubure des sommets
     * @param fb la reférence au tampon en mémoire utilisé pour l'affichage du
     * rectangle au coin arrondi
     * @param color la couleur unie du rectangle
     */
    void DrawRoundedRect(int x, int y, int width, int height, int radius, FrameBuffer& fb, pixels color) {
        pixels* buffer = fb.GetBackBuffer();
        int bufW = (int)fb.GetBufferWidth();
        int bufH = (int)fb.GetBufferHeight();

        for (int dy = 0; dy < height; dy++) {
            for (int dx = 0; dx < width; dx++) {
                int fx = x + dx;   // coordonnée ABSOLUE dans le framebuffer
                int fy = y + dy;

                if (fx < 0 || fx >= bufW) continue;
                if (fy < 0 || fy >= bufH) continue;

                // gestion des coins arrondis (optionnelle pour l'instant)
                int cornerX = -1, cornerY = -1;
                if (dx < radius && dy < radius) { cornerX = radius; cornerY = radius; }
                else if (dx < radius && dy >= height - radius) { cornerX = radius; cornerY = height - radius; }
                else if (dx >= width - radius && dy < radius) { cornerX = width - radius; cornerY = radius; }
                else if (dx >= width - radius && dy >= height - radius) { cornerX = width - radius; cornerY = height - radius; }

                if (cornerX != -1) {
                    int ddx = dx - cornerX;
                    int ddy = dy - cornerY;
                    if (ddx * ddx + ddy * ddy > radius * radius) continue;
                }

                int index = fy * bufW + fx;
                buffer[index] = BlendPixel(color, buffer[index]);
            }
        }
    }

    void Rectangle::DrawRenctangle(int tickness, FrameBuffer& fb) {
        if (tickness > 0)
        DrawRoundedRect(pos.m_x - tickness / 2.f, pos.m_y - tickness / 2.f, outWith + tickness, outHeight + tickness, radius, fb, borderColor);
        DrawRoundedRect(pos.m_x, pos.m_y, width, height, radius, fb, innerColor);
    }


} // namespace sf
