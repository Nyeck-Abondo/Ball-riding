#include "uiElement.h"

namespace SF {

    inline pixels BlendPixel(pixels src, pixels dst) {
        uint16_t a  = src.a;
        uint16_t ia = 255 - a;
        return pixels(
            (uint8_t)((src.r * a + dst.r * ia) / 255),
            (uint8_t)((src.g * a + dst.g * ia) / 255),
            (uint8_t)((src.b * a + dst.b * ia) / 255),
            255
        );
    }
    
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
                buffer[index] = BlendPixel(color, buffer[index]);
            }
        }
    }

    /**
     * @brief permet de charger une image dans le framebuffer
     * @return True si l'operation a reussie et false sinon
     */
    bool LoadImageFromAssets(Image& img) {
        int channels;
        unsigned char* data = stbi_load(img.location, &img.width, &img.height, &channels, 4);
        if(!data) {
            std::cout << "Echec duc chargement !!!!" << std::endl;
            return false;
        }

        img.pixel = new pixels[img.width * img.height];
        
        for (int i = 0; i < img.width * img.height; i++) {
            img.pixel[i] = pixels(data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]);
        }
        stbi_image_free(data);

        if (img.wantedHeight <= 0) img.wantedHeight = img.height;
        if(img.wantedWidth <= 0) img.wantedWidth = img.width;

        std::cout << "Cargement reussi !!!" << std::endl;
        return true;
    }


    /**
     * @brief Dessine une image presente a une position spécifique.
     * idéale pour les sprites d'images
     * @param img la structure contenant les informations de l'image
     * @param pos la position à laquelle on voudrait dessiner l'image
     * @param fb le frame buffer dans lequel on va passer les pixels de
     * l'image à dessiner
     */
    void DrawImageAt(Image& img, Vector2D pos, int sizeX, int sizeY, FrameBuffer& fb) {
        if (!img.pixel) return;
        pixels* buffer = fb.GetBackBuffer();
        
        //calcul du ration de l'image
        float scaleX = (float)sizeX / (float)img.wantedWidth;
        float scaleY = (float)sizeY / (float)img.wantedHeight;

        for (int y = 0; y < img.wantedHeight; y++) {
            int fy = (int)img.pos.m_y + y;
            if (fy < 0 || fy >= fb.GetBufferHeight()) continue;

            int localY = static_cast<int>(y * scaleY);
            if(localY >= sizeY) localY = sizeY - 1;

            int pxY = (int)pos.m_y + localY;
            if (pxY >= img.height) pxY = img.height - 1;

            for (int x = 0; x < img.wantedWidth; x++) {
                int fx = (int)img.pos.m_x + x;
                if (fx < 0 || fx >= fb.GetBufferWidth()) continue;

                int localX = static_cast<int>(x * scaleX);
                if(localX >= sizeX) localX = sizeX - 1;

                int pxX = (int)pos.m_x + localX;
                if (pxX >= img.width) pxX = img.width - 1;

                pixels src = img.pixel[pxY * img.width + pxX];
                int dstIndex = fy * fb.GetBufferWidth() + fx;
                buffer[dstIndex] = BlendPixel(src, buffer[dstIndex]);
            }
        }

    }

    /**
     * @brief Dessine une image à une position spécifique
     * @param img la structure contenant les informations de l'image
     * @param pos la position à laquelle on voudrait dessiner l'image
     * @param fb le frame buffer dans lequel on va passer les pixels de
     * l'image à dessiner
     */
    void DrawImage(Image& img, FrameBuffer& fb) {
    pixels* buffer = fb.GetBackBuffer();
    int fbWidth  = (int)fb.GetBufferWidth();
    int fbHeight = (int)fb.GetBufferHeight();
    
    //calcul du ration de l'affichage
    float scaleX = (float)img.width / (float)img.wantedWidth;
    float scaleY = (float)img.height / (float)img.wantedHeight;

    for (int y = 0; y < img.wantedHeight; y++) {
        int fy = (int)img.pos.m_y + y;
        if (fy < 0 || fy >= fbHeight) continue;

        int pxY = static_cast<int>(y * scaleY);
        //clamp pour le pixel de fin d'image
        if(pxY >= img.height) pxY = img.height - 1;

        for (int x = 0; x < img.wantedWidth; x++) {
            int fx = (int)img.pos.m_x + x;
            if (fx < 0 || fx >= fbWidth) continue;

            int pxX = static_cast<int>(x * scaleX);
            if (pxX >= img.width) pxX = img.width - 1;

            pixels src = img.pixel[pxY * img.width + pxX];
            int dstIndex = fy * fbWidth + fx;
            buffer[dstIndex] = BlendPixel(src, buffer[dstIndex]);
        }
    }
}

    /**
     * @brief Charge une police d'écriture en mémoire
     * @param font la structure stockant les informations de la police
     * à utiliser
     * @param location le chemin d'accès vers la police que l'on souhaite
     * utiliser
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
                    buffer[index] = BlendPixel(color, buffer[index]);
                }
            }
        cursorX += (int)(advW * scale);
        stbtt_FreeBitmap(bitmap, nullptr);
        }
    }

} // namespace SF
