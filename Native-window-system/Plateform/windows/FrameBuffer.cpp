#include "FrameBuffer.h"

namespace SF  {
    
    FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) 
        : m_width(width), m_height(height) {
            m_frontBuffer = new pixels[m_width * m_height];
            m_backBuffer = new pixels[m_width * m_height];

            m_BitmapInfo.bmiHeader.biBitCount = 32;
            m_BitmapInfo.bmiHeader.biPlanes = 1;
            m_BitmapInfo.bmiHeader.biHeight = -m_height;
            m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            m_BitmapInfo.bmiHeader.biWidth = m_width;
            m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
    }

    FrameBuffer::~FrameBuffer() {
        delete[] m_backBuffer;
        delete[] m_frontBuffer;
    }

    void FrameBuffer::ClearBuffer(pixels color) {
        for (int i = 0; i < m_width * m_height; i++) {
            m_backBuffer[i] = color;
        }
    }

    void FrameBuffer::Present(HWND hwnd) {
        //Echange des buffers pour l'affichage
        pixels* temp = m_backBuffer;
        m_backBuffer = m_frontBuffer;
        m_frontBuffer = temp;

        HDC hdc = GetDC(hwnd);

        StretchDIBits(
            hdc,
            0, 0, m_width, m_height,
            0, 0, m_width, m_height,
            m_frontBuffer,
            &m_BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        ReleaseDC(hwnd, hdc);
    }

} // namespace SF
