#include "uiElement.h"

namespace SF {
    
    void DrawRoundedRect(int x, int y, int width, int height, int radius, FrameBuffer& fb, pixels color) {
        pixels* buffer = fb.GetBackBuffer();
        int bufW = (int)fb.GetBufferWidth();
        int bufH = (int)fb.GetBufferHeight();

        for (int dy = 0; dy < height; dy++) {
            for (int dx = 0; dx < width; dx++) {
                int fx = x + dx;   // coordonnée ABSOLUE dans le framebuffer
                int fy = y + dy;

                if (fx < 0 || fx >= bufW) continue;
                if (fy < 0 || fy >= bufH) continue;

                // gestion des coins arrondis (optionnelle pour l'instant)
                int cornerX = -1, cornerY = -1;
                if (dx < radius && dy < radius) { cornerX = radius; cornerY = radius; }
                else if (dx < radius && dy >= height - radius) { cornerX = radius; cornerY = height - radius; }
                else if (dx >= width - radius && dy < radius) { cornerX = width - radius; cornerY = radius; }
                else if (dx >= width - radius && dy >= height - radius) { cornerX = width - radius; cornerY = height - radius; }

                if (cornerX != -1) {
                    int ddx = dx - cornerX;
                    int ddy = dy - cornerY;
                    if (ddx * ddx + ddy * ddy > radius * radius) continue;
                }

                int index = fy * bufW + fx;
                buffer[index] = color;
            }
        }
    }

    /**
     * @brief permet de charger une image dans le framebuffer
     * @return True si l'operation a reussie et false sinon
     */
    bool LoadImageFromAssets(Image img, const char* location) {
        int channels;
        unsigned char* data = stbi_load(location, &img.width, &img.height, &channels, 4);
        if(!data) return false;

        img.pixel = new pixels[img.width * img.height];
        
        for (int i = 0; i <= img.width * img.height; i++) {
            img.pixel[i] = pixels(data[i], data[i + 1], data[i + 2], data[i + 3]);
        }
        stbi_image_free(data);
        return true;
    }

    /**
     * @brief Charge une police d'écriture en mémoire
     * @return true si l'opération est un succès, false sinon
     */
    bool LoadFont(stbtt_fontinfo& font, const char* location) {
    FILE* f = fopen(location, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);          // <-- AJOUT INDISPENSABLE : on revient au début

    unsigned char* buffer = new unsigned char[size];
    size_t read = fread(buffer, 1, size, f);
    fclose(f);

    if (read != (size_t)size) {     // sécurité supplémentaire
        delete[] buffer;
        return false;
    }

    return stbtt_InitFont(&font, buffer, 0);
}

    void DrawText(stbtt_fontinfo& font, const char* text, int x, int y, float size, pixels color, FrameBuffer& fb) {
        pixels* buffer = fb.GetBackBuffer();
        float scale = stbtt_ScaleForPixelHeight(&font, size);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
        ascent = (int)(ascent * scale);

        int cursorX = x;
        for (int i = 0; text[i] != '\0'; i++) {
            int bw, bh, xoff, yoff;
            unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, text[i], &bw, &bh, &xoff, &yoff);

            int advW, lsb;
            stbtt_GetCodepointHMetrics(&font, text[i], &advW, &lsb);

            for (int j = 0; j < bh; j++) {
                for (int k = 0; k < bw; k++) {
                    int fx = cursorX + xoff + k;
                    int fy = y + ascent + yoff+ j;
                    if (fx < 0 || fx >= fb.GetBufferWidth())  continue;
                    if (fy < 0 || fy >= fb.GetBufferHeight()) continue;
                    uint8_t coverage = bitmap[j * bw + k];
                    if (coverage == 0) continue;
                    pixels src = color;
                    src.a = (uint8_t)((color.a * coverage) / 255);
                    int index = fy * fb.GetBufferWidth() + fx;
                    buffer[index] = color;
                }
            }
        cursorX += (int)(advW * scale);
        stbtt_FreeBitmap(bitmap, nullptr);
        }
    }

} // namespace SF
