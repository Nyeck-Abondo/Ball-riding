#pragma once

#include "IRenderer.h"
#include "../../Native-window-system/Plateform/window.h"

namespace sf {

    namespace render {

        class Renderer : public IRenderer2D {
            private:
            Camera m_view;
            Window* m_window;

            public:
            
            Renderer(Window* window) : m_window(window), m_view({window->GetFrameBuffer().GetBufferWidth(), window->GetFrameBuffer().GetBufferHeight()}) {}
            ~Renderer() {}

            //GETTERS
            Window& GetWindow() { return *m_window; }
            Camera GetCamera() { return m_view; }


            void SetView(Camera& camera) override;
            void DrawRectangle(maths::Vector2D pos, int width, int height, pixels color) override;
            void DrawRoundedRectangle(maths::Vector2D pos, int width, int height, int radius, int precision, pixels color) override;
            void DrawFillCircle(maths::Vector2D center, int radius, int precision, pixels color) override;
            void DrawFillTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) override;
            void DrawFillPolygon(std::vector<std::unique_ptr<Node>> points, pixels color) override;
            void DrawFillPolygon(std::vector<Node> points, pixels color) override;
            void DrawLine(maths::Vector2D p0, maths::Vector2D p1, pixels color) override;
            void DrawEmptyTriangle(maths::Vector2D p1, maths::Vector2D p2, maths::Vector2D p3, pixels color) override;
        };
        
    } // namespace render
     
} // namespace sf
