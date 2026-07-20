#include "legs.h"

namespace SF {
    
    Legs::Legs(FootType type, Vector2D& position, int thighLenght,int Leglenght, int thighWeight, int legWeight) 
    : m_type(type), m_thighAncre(position, thighWeight) {
        m_legAncre = Anchor(m_thighAncre.m_pos, m_thighAncre.m_radius - thighLenght - 5);
        m_footAncre = Anchor(m_legAncre.m_pos, m_legAncre.m_radius - Leglenght - 5);
        m_ancre.resize(thighLenght + Leglenght);
        m_node.resize(thighLenght * 2);
        m_angle = FindAngleThighLeg();

        m_ancre[0] = std::make_unique<Anchor>(Anchor(position, thighLenght / 2));
        for (int i = 1; i < thighLenght - 1; i++) {
            int r = std::max(1, thighWeight - i);
            m_ancre[i] = std::make_unique<Anchor>(
                    Anchor(
                    Vector2D(m_ancre[i - 1]->m_pos.m_x + r - 3, m_thighAncre.m_pos.m_y),
                    r
                )
            );
        }
        m_ancre[thighLenght - 1] = std::make_unique<Anchor>(m_legAncre);
        for (int i = thighLenght; i < Leglenght - 1 + thighLenght; i++) {
            int r = std::max(1, thighWeight - i);
            m_ancre[i] = std::make_unique<Anchor>(
                    Anchor(
                    Vector2D(m_ancre[i - 1]->m_pos.m_x + r - 3, m_thighAncre.m_pos.m_y),
                    r
                )
            );
        }
        m_ancre[thighLenght + Leglenght - 1] = std::make_unique<Anchor>(Anchor(m_legAncre.m_pos, m_legAncre.m_radius - Leglenght - 5));
    }

    float Legs::FindAngleThighLeg() {
        Vector2D thigh = m_thighAncre.m_pos - m_legAncre.m_pos;
        Vector2D leg = m_footAncre.m_pos - m_legAncre.m_pos;

        float dotProd = thigh.DotProduct(leg);

        return std::acos(dotProd / (thigh.Norme() * leg.Norme()));
    }

    void Legs::ApplyAngleConstraint() {
        Vector2D thigh = m_thighAncre.m_pos - m_legAncre.m_pos;
        Vector2D leg = m_footAncre.m_pos - m_legAncre.m_pos;
        leg.Normalise();
        Vector2D move = leg;
        m_angle = FindAngleThighLeg();

        if (m_angle > M_PI / 6.0f) {
            for (auto& ancre : m_ancre) {
                ancre->AccumulateDisplacement(move);
            }
        }
    }

    void Legs::VerleIntegretion(float gravity , float deltaTime) {
        for (auto& ancre : m_ancre) {
            Vector2D copy = ancre->m_pos;

            ancre->m_pos.m_x = 2 * ancre->m_pos.m_x - ancre->m_oldPos.m_x + 0 * deltaTime * deltaTime;
            ancre->m_pos.m_y = 2 * ancre->m_pos.m_y - ancre->m_oldPos.m_y + gravity * deltaTime * deltaTime;

            ancre->m_oldPos = copy;
        }
    }

    void Legs::SetAnchorPosition(Vector2D& position) {
        m_ancre[0]->m_pos = Vector2D(position.m_x - 12.0f, position.m_y);
        m_ancre[0]->m_oldPos = Vector2D(position.m_x - 12.0f, position.m_y);
    }

    void Legs::ApplyDistanceConstraint(Vector2D fixedPosition) {
        SetAnchorPosition(fixedPosition);
        int n = m_ancre.size();

        for (int i = 0; i < n - 1; i++) {
            Anchor& current = *m_ancre[i];
            Anchor& next = *m_ancre[i + 1];

            Vector2D diff = next.m_pos - current.m_pos;

            float dist = diff.Norme();

            if (dist > current.m_radius) {
                float distErr = (dist - current.m_radius) / 2.0f;
                Vector2D move = diff.WithMagnitude(distErr);

                if (i == 0) {
                    next.AccumulateDisplacement(move * -1.0f);
                }
                else {
                    current.AccumulateDisplacement(move);
                    next.AccumulateDisplacement(move * -1.0f);
                }
            }
        }
    }

    void Legs::Update(float gravity , float deltaTime, Vector2D fixedPosition) {
        SetAnchorPosition(fixedPosition);
        VerleIntegretion(gravity, deltaTime);
        ApplyDistanceConstraint(fixedPosition);

        for (auto& ancre : m_ancre) {
            ancre->ApplyDisplacement();
        }
    }

    void Legs::Render(FrameBuffer& fb ,pixels color) {
        for (int i = 0; i < m_ancre.size() - 1; i++) {
            DrawLine(m_ancre[i]->m_pos, m_ancre[i +1]->m_pos, color, fb);
        }
    }
} // namespace SF
