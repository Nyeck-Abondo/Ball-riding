#include "ball.h"

namespace sf {
    
    Ball::Ball(Node position, int radius, int weight, int links) 
        : m_center(position), m_radius(radius), m_weight(weight) {
            
            m_points.reserve(links);
            for(int i = 0; i < links; i++) {
                float angle = (2.0f * static_cast<float>(M_PI) * i) / links;
                m_points.push_back(std::make_unique<Node>(
                        maths::Vector2D(m_radius * std::cos(angle) + m_center.mainPos.m_x, 
                        m_radius * std::sin(angle) + m_center.mainPos.m_y)
                    )
                );
            }

            float circumference = 2.0f * M_PI * m_radius;
            m_areaIdeal = static_cast<float>(M_PI) * m_radius * m_radius;
            m_chordLenght = circumference / static_cast<float>(links);
            m_dstCenter = static_cast<float>(m_radius);

    }

    /**
     * @brief implémentation de l'intégration de verlet afin d'appliquer
     * un mouvement fluide au différentes particules d'un système d'étude
     * @param gravity la force de gravité appliquées à une particule
     * @param deltaTime c'est une variation très faible du temps
     */
    void Ball::VerletIntegretion(float clampX, float clampY ,float gravity, float deltaTime) {
        for (auto& node : m_points) {
            maths::Vector2D copy (node->mainPos);

            node->mainPos.m_x = 2 * node->mainPos.m_x - node->oldPos.m_x + 0 * deltaTime * deltaTime;
            node->mainPos.m_y = 2 * node->mainPos.m_y - node->oldPos.m_y + gravity * deltaTime * deltaTime;

            if (node->mainPos.m_y >= 710.0f) {
                node->mainPos.m_y = 710.0f;
                node->oldPos.m_y = 710.0f;
            }

            node->oldPos = copy;
        }

        maths::Vector2D copy(m_center.mainPos);
    }

    void Ball::ApplyDisplacement(maths::Vector2D direction, int intensity) {
        for (auto& point : m_points) {
            point->AccumulateDisplacement(direction.WithMagnitude(intensity));
        }
    }

    /**
     * @brief Contrainte d'aire. Cette fonction impose une aire idéale que le corps mou doit conserver. Elle
     * applique sur chacun des points constituant la forme un vecteur normal à son déplacement dirigé vers
     * l'extérieur de la forme
     */
    void Ball::ApplyDilatationConstraint() {
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

    /**
     * @brief Restrein la distance que peut prendre deux points l'un de l'autre à cause de l'application
     * de la moyenne des déplacements appliquées sur chacun d'eux en applicant un vecteur de deplacement
     * dans la direction opposée de leur mouvement pour les rapprocher ou encore les repousser
     */
    void Ball::ApplyDistanceConstraint() {

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

    /**
     * @brief Détermine la position du centre virtuel parfait de la forme
     * @return la positon exacte du centre parfait du polygone
     */
    maths::Vector2D Ball::CentroidPsoition() {
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

    /**
     * @brief calcule l'aire courant du corps mou durant sa déformation
     * @return l'aire du corps à un instant T
     */
    float Ball::ShapeArea() {
        float Area = 0.0f;
        int n = static_cast<int>(m_points.size());
        for (int i = 0; i < n; i ++) {
            float W = m_points[(i + 1) % n]->mainPos.m_x - m_points[i]->mainPos.m_x;
            float L = (m_points[(i + 1) % n]->mainPos.m_y + m_points[i]->mainPos.m_y) / 2.0f;

            Area += W * L; 
        }
        return std::fabs(Area);
    }

    /**
     * @brief donne une position fixe à un point de la forme
     * @param pos la position qu'on veut attribuer au point
     * @param index l'index du noeud de la forme auquel on veut attribuer
     */
    void Ball::SetPointfixedPosition(maths::Vector2D pos, int index) {
        m_points[index]->mainPos = pos;
        m_points[index]->oldPos = pos;
    }

    void Ball::Update(float clampX, float clampY, float gravity, float deltaTime, int iteration , maths::Vector2D pos, maths::Vector2D pos2, int index, int index2) {
        VerletIntegretion(clampX, clampY, gravity, deltaTime);
        
        for (int iter = 0; iter < iteration; iter++) {
            ApplyDistanceConstraint();
            ApplyDilatationConstraint();

            for (auto& point : m_points) point->ApplyDisplacement();
            m_center.ApplyDisplacement();
            SetPointfixedPosition(pos, index);
            SetPointfixedPosition(pos2, index2);
        }
        
        maths::Vector2D newCentroid = CentroidPsoition();
        maths::Vector2D delta = newCentroid - m_center.mainPos;
        m_center.mainPos += delta;
        m_center.oldPos += delta;
    }

    void Ball::Update(float clampX, float clampY, float gravity, float deltaTime, int iteration) {
        float dst = m_points[0]->oldPos.Distance(m_points[1]->oldPos) + 5.0f;
        float dstcenter = m_center.oldPos.Distance(m_points[0]->oldPos);
        float floor = 700.0f;

        float CurrentArea = ShapeArea();
        m_center.mainPos.m_y += std::sqrt(2 * gravity);

        VerletIntegretion(clampX, clampY, gravity, deltaTime);
        
        for (int iter = 0; iter < iteration; iter++) {
            ApplyDistanceConstraint();
            ApplyDilatationConstraint();

            for (auto& point : m_points) point->ApplyDisplacement();
            m_center.ApplyDisplacement();
        }
        maths::Vector2D newCentroid = CentroidPsoition();
        maths::Vector2D delta = newCentroid - m_center.mainPos;
        m_center.mainPos += delta;
        m_center.oldPos += delta;
    }

    void Ball::FillShape(render::Renderer& renderer, pixels color) {
        int n = static_cast<int>(m_points.size());
        if (n < 3) return;

        std::vector<Node> screenPos;
        screenPos.reserve(m_points.size());
        for (const auto& p : m_points) screenPos.push_back(*p);
        //convertion en coordonnees ecran
        for (auto& p : screenPos) {
            p.mainPos = renderer.GetCamera().WorldToScreenPos(p.mainPos);
        }

        // Bornes verticales du polygone
        float minY = screenPos[0].mainPos.m_y;
        float maxY = screenPos[0].mainPos.m_y;
        for (auto& p : screenPos) {
            minY = std::min(minY, p.mainPos.m_y);
            maxY = std::max(maxY, p.mainPos.m_y);
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
                const maths::Vector2D& p1 = screenPos[i].mainPos;
                const maths::Vector2D& p2 = screenPos[(i + 1) % n].mainPos;

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

    /**
     * @brief S'occupe de rendu de la pate de l'animal. dessine les formes géométriques la composant
     * @param fb référence sur le tampon en mémoir utilisé pour afficher les pixels de la pate à l'écran
     * @param color la couleur que prend le membre durant la phase de rendu
     */
    void Ball::Render(render::Renderer& renderer, pixels color) {

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
