#pragma once
#include <windows.h>
#include <cstdint>


namespace sf {

    struct pixels {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;

        pixels(): r(0), g(0), b(0), a(255) {}
        pixels(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255U) :
            r(red), g(green), b(blue), a(alpha) {}
    };

    class FrameBuffer {
        private:
        uint32_t m_width;
        uint32_t m_height;
        pixels* m_frontBuffer;
        pixels* m_backBuffer;

        BITMAPINFO m_BitmapInfo = {};

        public:
        //CONSTRUCTEUR
        FrameBuffer(uint32_t width, uint32_t height);
        ~FrameBuffer();

        //GETTERS
        pixels* GetBackBuffer() { return m_backBuffer; }
        uint32_t GetBufferWidth() { return m_width; }
        uint32_t GetBufferHeight() { return m_height; }

        //SETTERS
        void SetFrameDimension(uint32_t newWidth, uint32_t newHeight);
        void SetBackBuffer(int index, pixels color) { m_backBuffer[index] = color; }

        //METHODES
        /**
         * @brief Efface l'ecran avec la couleur passe en param
         * @param color la couleur qui sera affichee a l'ecran
         */
        void ClearBuffer(pixels color);

        /**
         * @brief echange les buffers avant affichage
         * @param hwnd le handle de la fenetre windows
         */
        void Present(HWND hwnd);
    };

    
} // namespace sf
