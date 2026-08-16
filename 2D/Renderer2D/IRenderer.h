#pragma once

#include "../render2D/algorithm2D.h"
#include "Camera2D/camera2D.h"

namespace sf {

    namespace render {
        
        class IRenderer2D {
            public:
            virtual ~IRenderer2D() = default;

            virtual void SetView(Camera& camera) = 0;

            //primitives simples
            virtual void DrawRectangle(maths::Vector2D pos, int width, int height, pixels color) = 0;
            virtual void DrawRoundedRectangle(maths::Vector2D pos, int width, int height, int radius, int precision, pixels color) = 0;
            virtual void DrawFillCircle(maths::Vector2D center, int radius, int precision, pixels color) = 0;
            virtual void DrawFillTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) = 0;
            virtual void DrawFillPolygon(std::vector<std::unique_ptr<Node>> points, pixels color) = 0;
            virtual void DrawFillPolygon(std::vector<Node> points, pixels color) = 0;
            virtual void DrawLine(maths::Vector2D p0, maths::Vector2D p1, pixels color) = 0;
            virtual void DrawEmptyTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) = 0;
        };

    } // namespace render
    
} // namespace sf
