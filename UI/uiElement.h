#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>

#include "../libs/stb_truetype.h"

#include "../libs/stb_image.h"
#include "../Native-window-system/Plateform/windows/FrameBuffer.h"
#include "../Native-window-system/Event/event.h"
#include "../2D/render2D/algorithm2D.h"

namespace SF {
    
    class UiElement {

        public:
            virtual ~UiElement() = default;
            
            //  METHODES COMMUNES
            virtual void Update(Event& event) = 0;
            virtual void Render(FrameBuffer& buffer, stbtt_fontinfo& font) = 0;

    };

    struct Image {
        pixels* pixel = nullptr;
        int width = 0;
        int height = 0;
        int wantedWidth = 0;
        int wantedHeight = 0;
        Vector2D pos;
        const char* location;

        //constructeur
        Image() {}
        Image(const char* locate, Vector2D position, int wWidth = 0, int wHeight = 0) 
        : location(locate), pos(position), wantedHeight(wHeight), wantedWidth(wWidth) {}

        void FreeImage() {
            delete[] pixel;
            pixel = nullptr;
        }
    };

    void DrawImage(Image& img, FrameBuffer& fb);
    void DrawImageAt(Image& img, Vector2D pos,  int sizeX, int sizeY, FrameBuffer& fb);
    void DrawRoundedRect(int x, int y, int width, int height, int radius, FrameBuffer& fb, pixels color);
    void DrawRect(int x, int y, int width, int height);

    /**
     * @brief alpha compositing de pixel
     */
    inline pixels BlendPixel(pixels src, pixels dst);
    /**
     * @brief permet de charger une image dans le framebuffer
     * @return True si l'operation a reussie et false sinon
     */
    bool LoadImageFromAssets(Image& img);

    /**
     * @brief Charge une police d'écriture en mémoire
     * @return true si l'opération est un succès, false sinon
     */
    bool LoadFont(stbtt_fontinfo& font, const char* location);

    void DrawText(stbtt_fontinfo& font, const char* text, int x, int y, float size, pixels color, FrameBuffer& fb);

} // namespace SF
