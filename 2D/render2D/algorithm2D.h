#pragma once
#define M_PI		3.14159265358979323846

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include "Type2D.h"

namespace sf {


    /**
     * @brief Compose la couleur de fond et la couleur d'un objet affiché à l'écran
     * @param src le pixel de la forme à afficher à l'écran
     * @param dst le pixel de en arrière plan par dessus lequel on va dessiner la forme
     * courante
     * @return un pixel avec une couleur mélangeant habilement la couleur en arrière plan
     * et celle au premier plan
     */
    pixels BlendPixel(pixels src, pixels dst);

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

    void DrawFillCircle(maths::Vector2D center, int radius, pixels color,FrameBuffer& fb);
    void DrawLine(const std::unique_ptr<Node>& node, const std::unique_ptr<Node>& other, FrameBuffer& bf);
    void DrawLine(maths::Vector2D& other, maths::Vector2D& vect, pixels color, FrameBuffer& bf);
    
} // namespace sf