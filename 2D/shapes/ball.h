#pragma once

#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../render2D/algorithm2D.h"


namespace SF {
    
    class Ball {
        private:
        int m_radius;
        int m_weight;
        Node m_center;
        std::vector<std::unique_ptr<Node>> m_points;

        float m_chordLenght;
        float m_areaIdeal;
        float m_dstCenter;

        public:
        Ball(Node position, int radius, int weight, int links);
        ~Ball(){ }

        //GETTERS
        Node GetCenter() { return m_center; }
        int GetRadius() { return m_radius; }
        std::unique_ptr<Node>& GetPoint(int index) { return m_points[index]; }
        Vector2D GetBorderPointTowards(Vector2D from) {
            Vector2D dir = from - m_center.mainPos;
            dir.Normalise();
            return m_center.mainPos + dir * static_cast<float>(m_radius);
        }

        //METHODES PUBLIQUES
        void NodeConstraint(Node node, const std::unique_ptr<Node>& other, float dstIdeal);
        void ApplyDistanceConstraint();
        void ApplyDilatationConstraint();
        void ApplyCenterConstraint();
        void VerletIntegretion(float gravity, float deltaTime);
        float BallArea();
        void Render(FrameBuffer& fb, pixels color);
        void Update(float gravity, float deltaTime, int iteration = 4);
        void FillBall(FrameBuffer& fb, pixels color);
    };

} // namespace SF
