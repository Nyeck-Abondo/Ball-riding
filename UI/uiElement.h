#pragma once

#include <cstdint>
#include <string>

#include "../libs/stb_truetype.h"

#include "../libs/stb_image.h"
#include "../Native-window-system/Plateform/windows/FrameBuffer.h"
#include "../Native-window-system/Event/event.h"

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
        int x = 0;
        int y = 0;
        const char* location;

        //constructeur
        Image() {}
        Image(const char* locate, int mx, int my) : location(locate), x(mx), y(my) {}

        void FreeImage() {
            delete[] pixel;
            pixel = nullptr;
        }
    };

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
    bool LoadImageFromAssets(Image img);

    /**
     * @brief Charge une police d'écriture en mémoire
     * @return true si l'opération est un succès, false sinon
     */
    bool LoadFont(stbtt_fontinfo& font, const char* location);

    void DrawText(stbtt_fontinfo& font, const char* text, int x, int y, float size, pixels color, FrameBuffer& fb);

} // namespace SF
