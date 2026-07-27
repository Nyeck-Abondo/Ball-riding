#include "legs.h"

namespace sf {
    
    Legs::Legs(FootType type, maths::Vector2D& position, int thighLenght,int Leglenght, int thighQuality, int legWeight) 
    : m_type(type), m_LegLenght(Leglenght), m_thighLenght(thighLenght), m_thigh(position, 6, 18, 20),
    m_legQuality(thighQuality) {
        m_ancre.resize(3);
        m_ancre[0] = Anchor(position, thighLenght);
        m_ancre[1] = Anchor(
            maths::Vector2D(m_ancre[0].m_pos.m_x + 3, m_ancre[0].m_pos.m_y + thighLenght),
            thighLenght
        );

        m_ancre[2] = Anchor(
            maths::Vector2D(m_ancre[1].m_pos.m_x, m_ancre[1].m_pos.m_y + Leglenght),
            Leglenght
        );
        FindThighNodePosition();
        m_angle = FindAngleThighLeg();
    }

    void Legs::IsColide() {
        if (m_ancre[2].m_pos.m_y >= 720.0f) {
            m_ancre[2].m_pos.m_y = 720.0f;
            m_ancre[2].m_oldPos.m_y = 720.0f;
        }
        if (m_ancre[1].m_pos.m_y >= 720 - m_LegLenght - 3.0f) {
            m_ancre[1].m_pos.m_y = 720 - m_LegLenght - 3.0f;
            m_ancre[1].m_oldPos.m_y = 720 - m_LegLenght - 3.0f;
        }
    }

    float Legs::FindAngleThighLeg() {
        int n = m_ancre.size();
        maths::Vector2D thigh = m_ancre[0].m_pos - m_ancre[1].m_pos;
        maths::Vector2D leg = m_ancre[n - 1].m_pos - m_ancre[1].m_pos;

        float dotProd = thigh.DotProduct(leg);

        return std::acos(dotProd / (thigh.Norme() * leg.Norme()));
    }

    void Legs::ApplyAngleConstraint() {
        int n = m_ancre.size();
        maths::Vector2D thigh = m_ancre[0].m_pos - m_ancre[1].m_pos;
        m_angle = FindAngleThighLeg();
        maths::Vector2D move = thigh.Perpendicular();

        if (m_angle > M_PI / 6.0f) {
            float angleErr = (m_angle - M_PI / 6.0f) / 2.0f;
            m_ancre[2].AccumulateDisplacement(move.WithMagnitude(2.5f));
            m_ancre[1].AccumulateDisplacement(move.WithMagnitude(1.5f) * -1.0f);
            std::cout << "Angle: " << m_angle << std::endl;
        }
    }

    void Legs::VerleIntegretion(float gravity , float deltaTime) {
        for (auto& ancre : m_ancre) {
            maths::Vector2D copy = ancre.m_pos;

            ancre.m_pos.m_x = 2 * ancre.m_pos.m_x - ancre.m_oldPos.m_x + 0 * deltaTime * deltaTime;
            ancre.m_pos.m_y = 2 * ancre.m_pos.m_y - ancre.m_oldPos.m_y + gravity * deltaTime * deltaTime;

            IsColide();
            ancre.m_oldPos = copy;
        }
    }

    void Legs::SetAnchorPosition(maths::Vector2D& position) {
        m_ancre[0].m_pos = maths::Vector2D(position.m_x, position.m_y);
        m_ancre[0].m_oldPos = maths::Vector2D(position.m_x, position.m_y);
    }

    void Legs::ApplyDistanceConstraint(maths::Vector2D fixedPosition) {
        //SetAnchorPosition(fixedPosition);
        int n = m_ancre.size();

        Anchor& current = m_ancre[0];
        Anchor& next = m_ancre[1];
        Anchor& foot = m_ancre[n - 1];

        maths::Vector2D diff = next.m_pos - current.m_pos;
        maths::Vector2D diff2 = foot.m_pos - next.m_pos;

        float dist = diff.Norme();
        float dist2 = diff2.Norme();

        if (dist > current.m_radius) {
            float distErr = (dist - current.m_radius) / 2.0f;
            maths::Vector2D move = diff.WithMagnitude(distErr);
            // current.AccumulateDisplacement(move);
            next.AccumulateDisplacement(move * -1.0f);
        } else {
            float distErr = (dist - current.m_radius) / 2.0f;
            maths::Vector2D move = diff.WithMagnitude(distErr);
            // current.AccumulateDisplacement(move);
            next.AccumulateDisplacement(move * -1.0f);
        }
        
        if (dist2 > foot.m_radius) {
            float distErr = (dist2 - next.m_radius) / 2.0f;
            maths::Vector2D move = diff2.WithMagnitude(distErr);
            next.AccumulateDisplacement(move);
            foot.AccumulateDisplacement(move * -1.0f);
        }
    }

    void Legs::FindThighNodePosition() {
        maths::Vector2D thigDir = m_ancre[0].m_pos - m_ancre[1].m_pos;
        thigDir.Normalise();
        maths::Vector2D thigNormalDir = thigDir.Perpendicular();
        for (int i = 0, p = 0; i < m_legQuality; i++) {
            p > m_legQuality - 10.0f ? p = m_legQuality - 10.0f : p++;
            maths::Vector2D pointOnThig = m_ancre[0].m_pos + thigDir / (m_thighLenght - i);
            maths::Vector2D thigPoint = pointOnThig + thigNormalDir * (m_legQuality - p);
            m_thighNode.push_back(thigPoint);
        }
    }

    void Legs::Update(float gravity , float deltaTime, maths::Vector2D fixedPosition) {
        SetAnchorPosition(fixedPosition);
        VerleIntegretion(gravity, deltaTime);
        ApplyAngleConstraint();

        // m_thigh.Update(
        //     15.0f, deltaTime, 400,
        //     m_ancre[0].m_pos, m_ancre[1].m_pos,
        //     9, 0
        // );
        for (int i = 0; i <= 9; i++) {
            ApplyDistanceConstraint(fixedPosition);
        }
        for (auto& ancre : m_ancre) {
            ancre.ApplyDisplacement();
        }

        maths::Vector2D thighVec = m_ancre[1].m_pos - m_ancre[0].m_pos;
        maths::Vector2D thighDir = thighVec;
        thighDir.Normalise();
        maths::Vector2D normal = thighDir.Perpendicular();

        // point de contrôle : milieu du segment, décalé perpendiculairement
        maths::Vector2D mid = (m_ancre[0].m_pos + m_ancre[1].m_pos) / 2.0f;
        maths::Vector2D control = mid + normal * m_legQuality; // ajuste le "ventre" de la courbe

        for (int i = 0; i < m_legQuality; i++) {
            float t = static_cast<float>(i) / (m_legQuality - 1);
            m_thighNode[i] = BezierPoint(t, m_ancre[0].m_pos, control, m_ancre[1].m_pos);
        }
    }

    void Legs::Render(FrameBuffer& fb ,pixels color) {
        for (int i = 0; i < m_ancre.size() - 1; i++) {
            DrawLine(m_ancre[i].m_pos, m_ancre[i +1].m_pos, color, fb);
        }
        m_thigh.Render(fb, pixels(25, 204, 95, 200));

        // maths::Vector2D thighDir = m_ancre[1].m_pos - m_ancre[0].m_pos;
        // thighDir.Normalise();
        // maths::Vector2D thighPerp = thighDir.Perpendicular();

        // maths::Vector2D pointOnThigh = m_ancre[0].m_pos + thighDir * m_thighLenght / 9.0f;
        // maths::Vector2D pointOnThigh01 = m_ancre[0].m_pos + thighDir * m_thighLenght / 2.0f;

        // // Point perpendiculaire à cet endroit, décalé d'une certaine largeur
        // maths::Vector2D legPointThight = pointOnThigh + thighPerp * 15.0f;
        // maths::Vector2D legPoint01 = pointOnThigh01 + thighPerp  * 13.0f + thighDir * 5.0f;

        for (int i = 0; i < m_thighNode.size() - 1; i++)
        DrawLine(m_thighNode[i+1], m_thighNode[i] , pixels(230, 60, 15, 255), fb);
    }
} // namespace sf
