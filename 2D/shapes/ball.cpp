#include "ball.h"

namespace SF {
    
    Ball::Ball(Node position, int radius, int weight, int links) 
        : m_center(position), m_radius(radius), m_weight(weight) {
            
            m_points.reserve(links);
            for(int i = 0; i < links; i++) {
                float angle = (2.0f * static_cast<float>(M_PI) * i) / links;
                m_points.push_back(std::make_unique<Node>(
                        Vector2D(m_radius * std::cos(angle) + m_center.mainPos.m_x, 
                        m_radius * std::sin(angle) + m_center.mainPos.m_y)
                    )
                );
            }

            float circumference = 2.0f * M_PI * m_radius;
            m_areaIdeal = static_cast<float>(M_PI) * m_radius * m_radius;
            m_chordLenght = circumference / static_cast<float>(links);
            m_dstCenter = static_cast<float>(m_radius);

            std::cout << "points: " << m_points.size() << std::endl;
    }

    void Ball::VerletIntegretion(float gravity, float deltaTime) {
        for (auto& node : m_points) {
            Vector2D copy (node->mainPos);

            node->mainPos.m_x = 2 * node->mainPos.m_x - node->oldPos.m_x + 0 * deltaTime * deltaTime;
            node->mainPos.m_y = 2 * node->mainPos.m_y - node->oldPos.m_y + gravity * deltaTime * deltaTime;

            if (node->mainPos.m_x > 1750.0f) {
                node->mainPos.m_x = 1750.0f;
                node->oldPos.m_x = 1750.0f;
            } 

            if (node->mainPos.m_x <= 20.0f) {
                node->mainPos.m_x = 20;
                node->mainPos.m_x = 20;
            }

            if (node->mainPos.m_y >= 710.0f) {
                node->mainPos.m_y = 710.0f;
                node->oldPos.m_y = 710.0f; // annule la vitesse verticale au contact du sol
            }

            node->oldPos = copy;
        }

        Vector2D copy(m_center.mainPos);

        m_center.mainPos.m_x = 2 * m_center.mainPos.m_x - m_center.oldPos.m_x + 0 * deltaTime * deltaTime;
        m_center.mainPos.m_y = 2 * m_center.mainPos.m_y - m_center.oldPos.m_y + gravity * deltaTime * deltaTime;

        if (m_center.mainPos.m_y >= 600.0f) {
            m_center.mainPos.m_y = 600.0f;
            m_center.oldPos.m_y = 600.0f;
        }

        if (m_center.mainPos.m_x > 1200.0f) {
                m_center.mainPos.m_x = 1198;
                m_center.oldPos.m_x = 1198;
            }

        m_center.oldPos = copy;
    }

    void Ball::ApplyDisplacement(Vector2D direction, int intensity) {
        for (auto& point : m_points) {
            point->AccumulateDisplacement(direction.WithMagnitude(intensity));
        }
    }

    void Ball::ApplyDilatationConstraint() {
        int n = static_cast<int>(m_points.size());
        float currentArea   = BallArea();
        float circumference = m_chordLenght * n;

        float areaError = m_areaIdeal - currentArea;
        float offset     = areaError / circumference;
        //const float maxOffset = 2.0f; // px par itération, à ajuster selon m_radius
        //offset = std::clamp(offset, -maxOffset, maxOffset);

        for (int i = 0; i < n; i++) {
            Node& current = *m_points[i];
            Node& previus = *m_points[i == 0 ? n - 1 : i - 1];
            Node& next = *m_points[(i + 1) % n];

            Vector2D secant = (previus.mainPos - next.mainPos).Perpendicular();
            Vector2D normal = secant.WithMagnitude(offset);
            current.AccumulateDisplacement(normal);
        }
    }

    void Ball::ApplyDistanceConstraint() {

        int n = static_cast<int>(m_points.size());
        for (int i = 0; i < n; i++) {
            Node& current = *m_points[i];
            //Node& previus = *m_points[i == 0 ? n - 1 : i - 1];
            Node& next = *m_points[i == 0 ? n - 1 : i - 1];

            //Vector2D secant = (next.mainPos - previus.mainPos).Perpendicular();
            Vector2D diff = next.mainPos - current.mainPos;

            float dist = diff.Norme();

            if (dist > m_chordLenght) {
                float disterr = (dist - m_chordLenght) / 2.0f;
                Vector2D move = diff.WithMagnitude(disterr);

                current.AccumulateDisplacement(move);
                next.AccumulateDisplacement(move * -1.0f);
            }
        }
    }

    float Ball::BallArea() {
        float Area = 0.0f;
        int n = static_cast<int>(m_points.size());
        for (int i = 0; i < n; i ++) {
            float W = m_points[(i + 1) % n]->mainPos.m_x - m_points[i]->mainPos.m_x;
            float L = (m_points[(i + 1) % n]->mainPos.m_y + m_points[i]->mainPos.m_y) / 2.0f;

            Area += W * L; 
        }
        return std::fabs(Area);
    }

    void Ball::Update(float gravity, float deltaTime, int iteration) {
        float dst = m_points[0]->oldPos.Distance(m_points[1]->oldPos) + 5.0f;
        float dstcenter = m_center.oldPos.Distance(m_points[0]->oldPos);
        float floor = 700.0f;

        float CurrentArea = BallArea();
        std::cout << "Aire : " << CurrentArea << std::endl;
        //m_center.mainPos.m_y += std::sqrt(2 * gravity);

        VerletIntegretion(gravity, deltaTime);
        
        for (int iter = 0; iter < iteration; iter++) {
            ApplyDistanceConstraint();
            ApplyDilatationConstraint();
            //ApplyCenterConstraint();

            for (auto& point : m_points) point->ApplyDisplacement();
            m_center.ApplyDisplacement();
        }
        std::cout << "Aire: " << BallArea() << " (cible: " << m_areaIdeal << ")" << std::endl;
        std::cout << "Ball Position: (" << m_center.mainPos.m_x << ", " << m_center.mainPos.m_y << ")" << std::endl;
    }

    void Ball::FillBall(FrameBuffer& fb, pixels color) {
        int n = static_cast<int>(m_points.size());
        if (n < 3) return;

        // Bornes verticales du polygone
        float minY = m_points[0]->mainPos.m_y;
        float maxY = m_points[0]->mainPos.m_y;
        for (auto& p : m_points) {
            minY = std::min(minY, p->mainPos.m_y);
            maxY = std::max(maxY, p->mainPos.m_y);
        }

        int width  = static_cast<int>(fb.GetBufferWidth());
        int height = static_cast<int>(fb.GetBufferHeight());
        int yStart = std::max(0, (int)std::floor(minY));
        int yEnd   = std::min(height - 1, (int)std::ceil(maxY));

        pixels* buffer = fb.GetBackBuffer();
        std::vector<float> xIntersections;

        for (int y = yStart; y <= yEnd; y++) {
            xIntersections.clear();
            float scanY = y + 0.5f; // centre du pixel : évite les cas limites sur les sommets

            for (int i = 0; i < n; i++) {
                const Vector2D& p1 = m_points[i]->mainPos;
                const Vector2D& p2 = m_points[(i + 1) % n]->mainPos;

                if (p1.m_y == p2.m_y) continue; // arête horizontale ignorée

                bool crosses = (scanY >= p1.m_y && scanY < p2.m_y) ||
                            (scanY >= p2.m_y && scanY < p1.m_y);
                if (!crosses) continue;

                float t = (scanY - p1.m_y) / (p2.m_y - p1.m_y);
                xIntersections.push_back(p1.m_x + t * (p2.m_x - p1.m_x));
            }

            if (xIntersections.size() < 2) continue;
            std::sort(xIntersections.begin(), xIntersections.end());

            for (size_t i = 0; i + 1 < xIntersections.size(); i += 2) {
                int xStart = std::max(0, (int)std::ceil(xIntersections[i]));
                int xEnd   = std::min(width - 1, (int)std::floor(xIntersections[i + 1]));

                for (int x = xStart; x <= xEnd; x++) {
                    
                    buffer[y * width + x] = BlendPixel(color, buffer[y * width + x]);
                }
            }
        }
    }

    void Ball::Render(FrameBuffer& fb, pixels color) {

        FillBall(fb, color);
        for (int i = 0; i < m_points.size(); i++) {
            if (i == m_points.size() - 1) {
                DrawLine(m_points[i], m_points[0], fb);
                break;
            }
            DrawLine(m_points[i], m_points[i +1], fb);
        }
    }
    
} // namespace SF
