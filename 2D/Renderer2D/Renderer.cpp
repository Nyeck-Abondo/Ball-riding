#include "Renderer.h"

namespace sf {

    namespace render {
        
        void Renderer::SetView(Camera& camera) {
            m_view = camera;
        }

        void Renderer::DrawLine(maths::Vector2D p0, maths::Vector2D p1, pixels color) {
            pixels* buffer = m_window->GetFrameBuffer().GetBackBuffer();
            int width = (int)m_window->GetWidth();
            int height = (int)m_window->GetHeight();
            //convertion en position ecran
            p0 = m_view.WorldToScreenPos(p0);
            p1 = m_view.WorldToScreenPos(p1);

            if (std::abs(p1.m_x - p0.m_x) > std::abs(p1.m_y - p0.m_y)) {
                if (p0.m_x > p1.m_x) {
                    maths::Vector2D temp = p1;
                    p1 = p0;
                    p0 = temp;
                }
                std::vector<float> ys = maths::Interpolated(p0, p1);
                int x0 = (int)p0.m_x, x1 = (int)p1.m_x;
                for (int x = x0; x <= x1; x++) {
                    size_t idx = (size_t)(x - x0);
                    if (idx >= ys.size()) continue;
                    int y = (int)ys[idx];
                    if (x < 0 || x >= width || y < 0 || y >= height) continue;
                    int index = y * width + x;
                    buffer[index] = BlendPixel(color, buffer[index]);
                }
            } else {
                if (p0.m_y > p1.m_y) p1.Swap(p0);
                std::vector<float> xs = maths::Interpolated({p0.m_y, p0.m_x}, {p1.m_y, p1.m_x});
                int y0 = (int)p0.m_y, y1 = (int)p1.m_y;
                for (int y = y0; y <= y1; y++) {
                    size_t idx = (size_t)(y - y0);
                    if (idx >= xs.size()) continue;
                    int x = (int)xs[idx];
                    if (x < 0 || x >= width || y < 0 || y >= height) continue;
                    int index = y * width + x;
                    buffer[index] = BlendPixel(color, buffer[index]);
                }
            }
        }

        void Renderer::DrawEmptyTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) {
            DrawLine(p1, p2, color);
            DrawLine(p2, p3, color);
            DrawLine(p3, p1, color);
        }

        void Renderer::DrawFillTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) {
            pixels* buffer = m_window->GetFrameBuffer().GetBackBuffer();
            int width = (int)m_window->GetWidth();
            int height = (int)m_window->GetHeight();

            //convertion en position ecran
            p1 = m_view.WorldToScreenPos(p1);
            p2 = m_view.WorldToScreenPos(p2);
            p3 = m_view.WorldToScreenPos(p3);

            //tri des points de sorte aue p1.m_y <= p2.m_y <= p3.m_y
            if (p2.m_y < p1.m_y) p1.Swap(p2);
            if (p3.m_y < p1.m_y) p3.Swap(p1);
            if (p3.m_y < p2.m_y) p3.Swap(p2);

            //calcul des coordonnes x des dotes du triangle
            std::vector<float> x01 = maths::Interpolated({p1.m_y, p1.m_x}, {p2.m_y, p2.m_x});
            std::vector<float> x12 = maths::Interpolated({p2.m_y, p2.m_x}, {p3.m_y, p3.m_x});
            std::vector<float> x02 = maths::Interpolated({p1.m_y, p1.m_x}, {p3.m_y, p3.m_x});

            //concatenation des cotes courts
            x01.pop_back();
            std::vector<float> x012;
            x012.reserve(x01.size() + x12.size());
            x012.insert(x012.end(), x01.begin(), x01.end());
            x012.insert(x012.end(), x12.begin(), x12.end());

            if (x012.empty() || x02.empty()) return;

            float m = std::floorf(x012.size() / 2.f);
            std::vector<float> xleft;
            std::vector<float> xrigth;
            if (x02[m] < x012[m]) {
                xleft = x02;
                xrigth = x012;
            } else {
                xleft = x012;
                xrigth = x02;
            }

            int yStart = (int)p1.m_y;
            int yEnd   = (int)p3.m_y;

            for (int y = yStart; y <= yEnd; y++) {
                if (y < 0 || y >= height) continue; // clip vertical

                size_t row = (size_t)(y - yStart);
                if (row >= xleft.size() || row >= xrigth.size()) continue;

                int xStart = (int)(xleft)[row];
                int xEnd   = (int)(xrigth)[row];

                // clamp horizontal aux bornes réelles du buffer
                if (xStart < 0) xStart = 0;
                if (xEnd >= width) xEnd = width - 1;
                if (xStart > xEnd) continue;

                int rowBase = y * width;
                for (int x = xStart; x <= xEnd; x++) {
                    buffer[rowBase + x] = BlendPixel(color, buffer[rowBase + x]);
                }
            }
        }

        void Renderer::DrawRectangle(maths::Vector2D pos, int width, int height, pixels color) {
            maths::Vector2D topRight {pos.m_x + width, pos.m_y};
            maths::Vector2D bottomLeft {pos.m_x, pos.m_y + height};
            maths::Vector2D bottomRight = {bottomLeft.m_x + width, bottomLeft.m_y};

            //rendu
            DrawFillTriangle(pos, topRight, bottomRight, color);
            DrawFillTriangle(pos, bottomLeft, bottomRight, color);
        }

        void Renderer::DrawRoundedRectangle(maths::Vector2D pos, int width, int height, int radius, int precision, pixels color) {
            std::vector<maths::Vector2D> rectPoints;
            rectPoints.reserve(precision * 4 + 4);
            maths::Vector2D topLeftCenter = {pos.m_x + radius, pos.m_y + radius};
            maths::Vector2D topRightCenter {topLeftCenter.m_x + width - radius * 2, topLeftCenter.m_y};
            maths::Vector2D bottomLeftCenter {topLeftCenter.m_x, topLeftCenter.m_y + height - radius * 2};
            maths::Vector2D bottomRightCenter {bottomLeftCenter.m_x + width - radius * 2, bottomLeftCenter.m_y};
            
            //REMLPISSAGE DU TABLEAU DES POINTS DU RECTANTGLE POUR LE REMPLISSAGE PAR EVANTAIL DE TRIANGLE PLEIN
            for (int i = 0; i < precision; i++) {
                float angle = (i * M_PI / 2.f) / precision;
                rectPoints.push_back({bottomRightCenter.m_x + radius * std::cosf(angle), bottomRightCenter.m_y + radius * std::sinf(angle)});
            }
            for (int i = 0; i < precision; i++) {
                float angle = (i * M_PI / 2.f) / precision + M_PI / 2.f;
                rectPoints.push_back({bottomLeftCenter.m_x + radius * std::cosf(angle), bottomLeftCenter.m_y + radius * std::sinf(angle)});
            }
            for (int i = 0; i < precision; i++) {
                float angle = (i * M_PI / 2.f) / precision + M_PI;
                rectPoints.push_back({topLeftCenter.m_x + radius * std::cosf(angle), topLeftCenter.m_y + radius * std::sinf(angle)});
            }
            for (int i = 0; i < precision; i++) {
                float angle = (i * M_PI / 2.f) / precision + 3.f * M_PI / 2.f;
                rectPoints.push_back({topRightCenter.m_x + radius * std::cosf(angle), topRightCenter.m_y + radius * std::sinf(angle)});
            }

            //REMPLISSAGE DE LA FORME GRASSE AUX TRIANGLE PLEINS
            int n = rectPoints.size();
            for (int i = 0; i < n - 2; i++) {
                maths::Vector2D next = rectPoints[i + 1];
                maths::Vector2D third = rectPoints[i + 2];
                maths::Vector2D p0 = rectPoints[0];

                DrawFillTriangle(p0, next, third, color);
            }
        }

        void Renderer::DrawFillCircle(maths::Vector2D center, int radius, int precision, pixels color) {
            std::vector<maths::Vector2D> points;
            points.reserve(precision);
            for (int i = 0; i < precision; i++) {
                float angle = (2 * M_PI * i) / precision;
                points.push_back({center.m_x + radius * std::cosf(angle), center.m_y + radius * std::sinf(angle)});
            }
            int n = points.size();
            for (int i = 0; i < n - 2; i++) {
                maths::Vector2D next = points[i + 1];
                maths::Vector2D third = points[i + 2];
                maths::Vector2D p0 = points[0];

                DrawFillTriangle(p0, next, third, color);
            }
        }

        void Renderer::DrawFillPolygon(std::vector<std::unique_ptr<Node>> points, pixels color) {
            int n = points.size();
            for (int i = 0; i < n - 2; i++) {
                Node& next = *points[i + 1];
                Node& third = *points[i + 2];
                Node& p0 = *points[0];
                DrawFillTriangle(p0.mainPos, next.mainPos, third.mainPos, color);
            }
        }

        void Renderer::DrawFillPolygon(std::vector<Node> points, pixels color) {
            int n = points.size();
            for (int i = 0; i < n - 2; i++) {
                Node& next = points[i + 1];
                Node& third = points[i + 2];
                Node& p0 = points[0];
                DrawFillTriangle(p0.mainPos, next.mainPos, third.mainPos, color);
            }
        }

    } // namespace render
    
} // namespace sf
