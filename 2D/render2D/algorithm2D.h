#pragma once

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include "../../Native-window-system/Plateform/windows/FrameBuffer.h"

namespace SF {

    struct Vector2D {
        float m_x;
        float m_y;

        Vector2D(float posX = 0, float posY = 0) : m_x(posX), m_y(posY) { }

        Vector2D operator-(Vector2D& vect) {
            return Vector2D(this->m_x - vect.m_x, this->m_y - vect.m_y);
        }

        Vector2D operator+(Vector2D vect) {
            return Vector2D(this->m_x + vect.m_x, this->m_y + vect.m_y);
        }

        Vector2D operator/(float scalar) {
            return Vector2D(this->m_x / scalar, this->m_y / scalar);
        }

        Vector2D operator*(float scalar) {
            return Vector2D(this->m_x * scalar, this->m_y * scalar);
        }

        Vector2D& operator+=(const Vector2D& v) { 
            m_x += v.m_x;
            m_y += v.m_y;
            return *this;
        }

        float Distance(Vector2D& other) {
            return std::sqrt((this->m_x - other.m_x) * (this->m_x - other.m_x) + ((this->m_y - other.m_y) * (this->m_y - other.m_y)));
        }

        float Norme() {
            return std::sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y));
        }

        void Normalise() {
            float n = Norme();
            if (n < 1e-5f) { 
                m_x = 0.0f;
                m_y = 0.0f;
                return;
            }
            m_x /= n;
            m_y /= n;
        }

        Vector2D Perpendicular() { return Vector2D(-1.0f *m_y, m_x); }
        
        Vector2D WithMagnitude(float mag) const {
            Vector2D v(m_x, m_y);
            v.Normalise();
            return v * mag;
        }
    };

    struct Node {
        Vector2D mainPos;
        Vector2D oldPos;
        Vector2D displacement;
        pixels color;

        int mass;
        float averageDisplacement;

        Node(){}
        Node (Vector2D vect, pixels mColor = pixels(255, 255, 255, 0), int weight = 3) 
            : oldPos(vect), color(mColor), mass(weight), mainPos(vect),
            displacement(0, 0), averageDisplacement(0.0f) {}

        void AccumulateDisplacement(const Vector2D& vect) {
            displacement += vect;
            averageDisplacement++;
        }

        void ApplyDisplacement() {
            if (averageDisplacement > 0) {
                mainPos += displacement / averageDisplacement;
                displacement = Vector2D(0, 0);
                averageDisplacement = 0;
            }
        }
    };

    pixels BlendPixel(pixels src, pixels dst);
    void DrawLine(const std::unique_ptr<Node>& node, const std::unique_ptr<Node>& other, FrameBuffer& bf);
    void DrawLine(Vector2D& other, Vector2D& vect, pixels color, FrameBuffer& bf);
    
} // namespace SF