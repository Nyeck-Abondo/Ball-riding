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

    //============================================================
    // CLASS: UiElemenT
    // DESCRIPTION: Interface décrivant le comportement générique
    // des objets de l'interface utilisateur
    //===========================================================
    class UiElement {

        public:
            virtual ~UiElement() = default;
            
            //  METHODES COMMUNES
            virtual void Update(Event& event) = 0;
            virtual void Render(FrameBuffer& buffer, stbtt_fontinfo& font) = 0;

    };

    /**
     * @struct Image
     * @brief structure aui stoque les pixels constituant les 
     * images
     */
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

        /**
         * @brief libère les ressources allouées par le chargement de l'image
         * en mémoire
         */
        void FreeImage() {
            delete[] pixel;
            pixel = nullptr;
        }
    };

    /**
     * @name DrawImage
     * @brief Affiche une image préalablement chargée en mémoire
     * @param img la structure qui acceuillera les informations de
     * l'image chargée en mémoire.
     * @param fb référnce au tampon en mémoire alloué pour l'affichage
     * des images images chargées en mémoire
     */
    void DrawImage(Image& img, FrameBuffer& fb);

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
    void DrawImageAt(Image& img, Vector2D pos,  int sizeX, int sizeY, FrameBuffer& fb);

    /**
     * @name DrawRoundedRect
     * @brief Dessine un rectangle au sommets arondis
     * @param x la position sur les abscisses
     * @param y la position sur les ordonnées
     * @param width la larageur du rectangle
     * @param height la hauteur du rectangle
     * @param radius le rayon de coubure des sommets
     * @param fb la reférence au tampon en mémoire utilisé pour l'affichage du
     * rectangle au coin arrondi
     * @param color la couleur unie du rectangle
     */
    void DrawRoundedRect(int x, int y, int width, int height, int radius, FrameBuffer& fb, pixels color);

    /**
     * @name DrawRoundedRect
     * @brief Dessine un rectangle simple
     * @param x la position sur les abscisses
     * @param y la position sur les ordonnées
     * @param width la larageur du rectangle
     * @param height la hauteur du rectangle
     * @param fb la reférence au tampon en mémoire utilisé pour l'affichage du
     * rectangle
     * @param color la couleur unie du rectangle
     */
    void DrawRectangle(int x, int y, int width, int height, FrameBuffer& fb);

    /**
     * @brief permet de charger une image dans le framebuffer
     * @param img la structure Image qui acceuillera les différents pixels de l'image
     * @return True si l'operation a reussie et false sinon
     */
    bool LoadImageFromAssets(Image& img);

    /**
     * @brief Charge une police d'écriture en mémoire
     * @param font structure stockant les informations de la de la police true type (ttf)
     * @param location le chemin d'accès vers la police à charger en mémoire
     * @return true si l'opération est un succès, false sinon
     */
    bool LoadFont(stbtt_fontinfo& font, const char* location);

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
    void DrawText(stbtt_fontinfo& font, const char* text, int x, int y, float size, pixels color, FrameBuffer& fb);

} // namespace SF
