#pragma once

#include "../../Maths/math2D.h"
#include "../../Native-window-system/Plateform/windows/FrameBuffer.h"
#include "../../UI/uiElement.h"

namespace sf {

    struct Rectangle {
        maths::Vector2D pos;
        int width, height;
        int outWith, outHeight;
        int radius;
        pixels innerColor, borderColor;

        Rectangle() { }
        Rectangle(maths::Vector2D position, int w, int h, int r, pixels iColor, pixels bColor) :
        pos(position), width(w), height(h), radius(r), innerColor(iColor), borderColor(bColor),
        outHeight(height), outWith(width) { 

        }

        void DrawRenctangle(int tickness, FrameBuffer& fb);
    };

    struct Node {
        maths::Vector2D mainPos;
        maths::Vector2D oldPos;
        maths::Vector2D displacement;
        pixels color;

        int mass;
        float averageDisplacement;

        Node(){}
        Node (maths::Vector2D vect, pixels mColor = pixels(255, 255, 255, 0), int weight = 3) 
            : oldPos(vect), color(mColor), mass(weight), mainPos(vect),
            displacement(0, 0), averageDisplacement(0.0f) {}

        /**
         * @brief Somme tous les vecteurs de déplacemment subis par un noeud
         */
        void AccumulateDisplacement(const maths::Vector2D& vect) {
            displacement += vect;
            averageDisplacement++;
        }

        /**
         * @brief assigne la moyenne des vecteurs de déplacement accumulés par le noeud
         */
        void ApplyDisplacement() {
            if (averageDisplacement > 0) {
                mainPos += displacement / averageDisplacement;
                displacement = maths::Vector2D(0, 0);
                averageDisplacement = 0;
            }
        }
    };

    
} // namespace sf
