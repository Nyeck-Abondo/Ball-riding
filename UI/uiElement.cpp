#include "uiElement.h"

namespace sf {

    /**
     * @brief permet de charger une image dans le framebuffer
     * @param img la structure Image qui acceuillera les différents pixels de l'image
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
     * @name DrawImage
     * @brief Affiche une image ciblée dans une sprite de dimensions connue
     * @param img la structure qui acceuillera les informations de
     * l'image chargée en mémoire.
     * @param pos la position de l'image ciblée dans la sprite d'image
     * @param sizeX la taille désirée de l'image ciblée
     * @param sizeY la hauteur désirée pour l'image ciblée
     * @param fb référnce au tampon en mémoire alloué pour l'affichage
     * des images images chargées en mémoire
     */
    void DrawImageAt(Image& img, maths::Vector2D pos, int sizeX, int sizeY, FrameBuffer& fb) {
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
     * @name DrawImage
     * @brief Affiche une image préalablement chargée en mémoire
     * @param img la structure qui acceuillera les informations de
     * l'image chargée en mémoire.
     * @param fb référnce au tampon en mémoire alloué pour l'affichage
     * des images images chargées en mémoire
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
     * @param font structure stockant les informations de la de la police true type (ttf)
     * @param location le chemin d'accès vers la police à charger en mémoire
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

    /**
     * @brief Dessine le texte à un endroit précis de la fenêtre
     * @param font la police chargée en mémoire grâce à LoadFont
     * @param text la chaine de caractère constituant le message ou le texte
     * voulant être affiché dans la fenêtre
     * @param x la position sur les abscisses
     * @param y la position sur les ordonnées
     * @param size la taille de la police souhaité par l'utilisateur
     * @param color la couleur du texte
     * @param fb la reférence au tampon en mémoire utilisé pour l'affichage du
     * rectangle 
     */
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

} // namespace sf
