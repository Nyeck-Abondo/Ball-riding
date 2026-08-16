#include "ellipse.h"

namespace sf {
    
    Ellipse::Ellipse(Node position, int smallRadius, int largeRadius, int links)
    : m_center(position), m_radiusB(smallRadius), m_radiusA(largeRadius) {
        m_points.reserve(links);
        m_points.reserve(links);

        for (int i = 0; i < links; i++) {
            float angle = static_cast<float>(2.0f * M_PI * i) / (float)links;
            m_initialPosition.push_back(
                Node(
                    maths::Vector2D(
                        m_radiusA * std::cos(angle),
                        m_radiusB * std::sin(angle)
                    )
                )
            );

            m_points.push_back(
                std::make_unique<Node>(
                    Node(
                        maths::Vector2D(m_radiusA * std::cos(angle) + m_center.mainPos.m_x,
                        m_radiusB * std::sin(angle) + m_center.mainPos.m_y)
                    )
                )
            );
        }

        float circumference = M_PI * (3 *(m_radiusA + m_radiusB) - std::sqrt((3 * m_radiusA + m_radiusB) * (m_radiusA + 3 * m_radiusB)));
        m_areaIdeal = static_cast<float>(M_PI * m_radiusA * m_radiusB);
        m_chordLenght = circumference / static_cast<float>(links);

        std::cout << "points: " << m_points.size() << std::endl;
    }

    void Ellipse::VerletIntegretion(float clampX, float clampY, float gravity, float deltaTime) {
        for (auto& point : m_points) {
            maths::Vector2D copy(point->mainPos);

            point->mainPos.m_x = 2 * point->mainPos.m_x - point->oldPos.m_x + 0 * deltaTime * deltaTime;
            point->mainPos.m_y = 2 * point->mainPos.m_y - point->oldPos.m_y + gravity * deltaTime * deltaTime;

            if (point->mainPos.m_x > clampX) {
                point->mainPos.m_x = clampX;
                point->oldPos.m_x = clampX;
            }

            if (point->mainPos.m_x <= 20.0f) {
                point->mainPos.m_x = 20.0f;
                point->oldPos.m_x = 20.0f;
            }

            if (point->mainPos.m_y >= 710.0f) {
                point->mainPos.m_y = 710.0f;
                point->oldPos.m_y = 710.0f;
            }

            if (point->mainPos.m_y <= clampY) {
                point->mainPos.m_y = clampY;
                point->oldPos.m_y = clampY;
            }
            
            point->oldPos = copy;
        }
    }

    void Ellipse::ApplyCenterConstraint(float stifness) {
        int n = static_cast<int>(m_initialPosition.size());
        for (int i = 0; i < n; i++) {
            Node& current = *m_points[i];
            maths::Vector2D& initalpos = m_initialPosition[i].mainPos;

            maths::Vector2D target = m_center.mainPos + initalpos;
            maths::Vector2D correction = (target - current.mainPos) * stifness;

            current.AccumulateDisplacement(correction);
        }
    }

    void Ellipse::ApplyDisplacement(maths::Vector2D direction, int intensity) {
        for (auto& point : m_points) {
            point->AccumulateDisplacement(direction.WithMagnitude(intensity));
        }
    }

    void Ellipse::ApplyDilatationConstraint() {
        int n = static_cast<int>(m_points.size());
        float currentArea   = ShapeArea();
        float circumference = m_chordLenght * n;

        float areaError = m_areaIdeal - currentArea;
        float offset     = areaError / circumference;
        //const float maxOffset = 2.0f; // px par itération, à ajuster selon m_radius
        //offset = std::clamp(offset, -maxOffset, maxOffset);

        for (int i = 0; i < n; i++) {
            Node& current = *m_points[i];
            Node& previus = *m_points[i == 0 ? n - 1 : i - 1];
            Node& next = *m_points[(i + 1) % n];

            maths::Vector2D secant = (previus.mainPos - next.mainPos).Perpendicular();
            maths::Vector2D normal = secant.WithMagnitude(offset);
            current.AccumulateDisplacement(normal);
        }
    }

    void Ellipse::ApplyDistanceConstraint() {

        int n = static_cast<int>(m_points.size());
        for (int i = 0; i < n; i++) {
            Node& current = *m_points[i];
            //Node& previus = *m_points[i == 0 ? n - 1 : i - 1];
            Node& next = *m_points[i == 0 ? n - 1 : i - 1];

            //maths::Vector2D secant = (next.mainPos - previus.mainPos).Perpendicular();
            maths::Vector2D diff = next.mainPos - current.mainPos;

            float dist = diff.Norme();

            if (dist > m_chordLenght) {
                float disterr = (dist - m_chordLenght) / 2.0f;
                maths::Vector2D move = diff.WithMagnitude(disterr);

                current.AccumulateDisplacement(move);
                next.AccumulateDisplacement(move * -1.0f);
            }
        }
    }

    float Ellipse::ShapeArea() {
        float Area = 0.0f;
        int n = static_cast<int>(m_points.size());
        for (int i = 0; i < n; i ++) {
            float W = m_points[(i + 1) % n]->mainPos.m_x - m_points[i]->mainPos.m_x;
            float L = (m_points[(i + 1) % n]->mainPos.m_y + m_points[i]->mainPos.m_y) / 2.0f;

            Area += W * L; 
        }
        return std::fabs(Area);
    }

    maths::Vector2D Ellipse::CentroidPsoition() {
        int n = static_cast<int>(m_points.size());
        float signedArea = 0.0f;
        float cx = 0.0f, cy = 0.0f;

        for (int i = 0; i < n; i++) {
            const maths::Vector2D& p0 = m_points[i]->mainPos;
            const maths::Vector2D& p1 = m_points[(i + 1) % n]->mainPos;

            float cross = p0.m_x * p1.m_y - p1.m_x * p0.m_y;
            signedArea += cross;
            cx += (p0.m_x + p1.m_x) * cross;
            cy += (p0.m_y + p1.m_y) * cross;
        }

        signedArea *= 0.5f;

        if (std::fabs(signedArea) < 1e-5f) {
            // figure dégénérée (aire ~0) : repli sur la moyenne simple
            maths::Vector2D avg(0, 0);
            for (auto& p : m_points) avg += p->mainPos;
            return avg / static_cast<float>(n);
        }

        cx /= (6.0f * signedArea);
        cy /= (6.0f * signedArea);
        return maths::Vector2D(cx, cy);
    }


    void Ellipse::SetPointfixedPosition(maths::Vector2D pos, int index) {
        m_points[index]->mainPos = pos;
        m_points[index]->oldPos = pos;
    }


    void Ellipse::Update(float clampX, float clampY, float gravity, float deltaTime, int iteration , maths::Vector2D pos, maths::Vector2D pos2, int index, int index2) {
        float dst = m_points[0]->oldPos.Distance(m_points[1]->oldPos) + 5.0f;
        float dstcenter = m_center.oldPos.Distance(m_points[0]->oldPos);
        float floor = 700.0f;

        float CurrentArea = ShapeArea();
        std::cout << "Aire : " << CurrentArea << std::endl;
        m_center.mainPos.m_y += std::sqrt(2 * gravity);

        VerletIntegretion(clampX, clampY, gravity, deltaTime);        
        for (int iter = 0; iter < iteration; iter++) {
            ApplyDistanceConstraint();
            ApplyDilatationConstraint();
            ApplyCenterConstraint(0.15f);

            for (auto& point : m_points) point->ApplyDisplacement();
            m_center.ApplyDisplacement();
            SetPointfixedPosition(pos, index);
            SetPointfixedPosition(pos2, index2);
            maths::Vector2D newCentroid = CentroidPsoition();
            maths::Vector2D delta = newCentroid - m_center.mainPos;
            m_center.mainPos += delta;
            m_center.oldPos += delta;
        }
        
        std::cout << "Aire: " << ShapeArea() << " (cible: " << m_areaIdeal << ")" << std::endl;
        std::cout << "Ball Position: (" << m_center.mainPos.m_x << ", " << m_center.mainPos.m_y << ")" << std::endl;
    }


    void Ellipse::Update(float clampX, float clampY, float gravity, float deltaTime, int iteration) {
        float dst = m_points[0]->oldPos.Distance(m_points[1]->oldPos) + 5.0f;
        float dstcenter = m_center.oldPos.Distance(m_points[0]->oldPos);
        float floor = 700.0f;

        float CurrentArea = ShapeArea();
        std::cout << "Aire : " << CurrentArea << std::endl;
        //m_center.mainPos.m_y += std::sqrt(2 * gravity);

        VerletIntegretion(clampX, clampY, gravity, deltaTime);
        
        for (int iter = 0; iter < iteration; iter++) {
            ApplyDistanceConstraint();
            ApplyDilatationConstraint();
            ApplyCenterConstraint(3.0f);

            for (auto& point : m_points) point->ApplyDisplacement();
            m_center.ApplyDisplacement();

            maths::Vector2D newCentroid = CentroidPsoition();
            maths::Vector2D delta = newCentroid - m_center.mainPos;
            m_center.mainPos += delta;
            m_center.oldPos += delta;
        }
        std::cout << "Aire: " << ShapeArea() << " (cible: " << m_areaIdeal << ")" << std::endl;
        std::cout <<  "Ellipse Position: (" << m_center.mainPos.m_x << ", " << m_center.mainPos.m_y << ")" << std::endl;
    }

    
    void Ellipse::FillShape(render::Renderer& renderer, pixels color) {
        int n = static_cast<int>(m_points.size());
        if (n < 3) return;

        // Bornes verticales du polygone
        float minY = m_points[0]->mainPos.m_y;
        float maxY = m_points[0]->mainPos.m_y;
        for (auto& p : m_points) {
            minY = std::min(minY, p->mainPos.m_y);
            maxY = std::max(maxY, p->mainPos.m_y);
        }

        int width  = static_cast<int>(renderer.GetWindow().GetWidth());
        int height = static_cast<int>(renderer.GetWindow().GetHeight());
        int yStart = std::max(0, (int)std::floor(minY));
        int yEnd   = std::min(height - 1, (int)std::ceil(maxY));

        pixels* buffer = renderer.GetWindow().GetFrameBuffer().GetBackBuffer();
        std::vector<float> xIntersections;

        for (int y = yStart; y <= yEnd; y++) {
            xIntersections.clear();
            float scanY = y + 0.5f; // centre du pixel : évite les cas limites sur les sommets

            for (int i = 0; i < n; i++) {
                const maths::Vector2D& p1 = m_points[i]->mainPos;
                const maths::Vector2D& p2 = m_points[(i + 1) % n]->mainPos;

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

    void Ellipse::Render(render::Renderer& renderer, pixels color) {

        FillShape(renderer, color);
        for (int i = 0; i < m_points.size(); i++) {
            if (i == m_points.size() - 1) {
                renderer.DrawLine(m_points[i]->mainPos, m_points[0]->mainPos, color);
                break;
            }
            renderer.DrawLine(m_points[i]->mainPos, m_points[i +1]->mainPos, color);
        }
    }


} // namespace sf
