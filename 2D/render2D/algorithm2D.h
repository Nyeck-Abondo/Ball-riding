#pragma once
#define M_PI		3.14159265358979323846

#include <cmath>
#include <vector>
#include <memory>
#include <iostream>
#include "../../Native-window-system/Plateform/windows/FrameBuffer.h"

namespace SF {

    struct Vector2D {
        float m_x;
        float m_y;

        Vector2D(float posX = 0, float posY = 0) : m_x(posX), m_y(posY) { }

        /**
         * @brief opérateur de soustraction
         * @return la resultante de deux vecteurs
         */
        Vector2D operator-(Vector2D& vect) {
            return Vector2D(this->m_x - vect.m_x, this->m_y - vect.m_y);
        }

        /**
         * @brief opérateur d'addition
         * @return la somme de vecteurs
         */
        Vector2D operator+(Vector2D vect) {
            return Vector2D(this->m_x + vect.m_x, this->m_y + vect.m_y);
        }

        /**
         * @brief opérateur de division
         * @return division d'un vecteur par un scalaire
         */
        Vector2D operator/(float scalar) {
            return Vector2D(this->m_x / scalar, this->m_y / scalar);
        }

        /**
         * @brief opérateur de multiplication
         * @return le produit d'un vecteur par un scalaire
         */
        Vector2D operator*(float scalar) {
            return Vector2D(this->m_x * scalar, this->m_y * scalar);
        }

        /**
         * @brief opérateur d'addition
         * @return somme de deux vecteurs
         */
        Vector2D& operator+=(const Vector2D& v) { 
            m_x += v.m_x;
            m_y += v.m_y;
            return *this;
        }

        /**
         * @brief Calcule la distance entre deux points
         */
        float Distance(Vector2D& other) {
            return std::sqrt((this->m_x - other.m_x) * (this->m_x - other.m_x) + ((this->m_y - other.m_y) * (this->m_y - other.m_y)));
        }

        /**
         * @brief calcule la norme d'un vecteur
         * @return la norme du vecteur
         */
        float Norme() {
            return std::sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y));
        }

        /**
         * @brief normalise un vecteur
         */
        void Normalise() {
            float n = Norme();
            if (n < 1e-5f) { 
                m_x = 0.0f;
                m_y = 0.0f;
                return;
            }
            m_x /= n;
            m_y /= n;
        }

        /**
         * @brief Calcule le produit sclaire de deux vecteurs
         * @return Renvoie le porduit scalaire de deux vecteurs
         */
        float DotProduct(Vector2D& vect) {
            return this->m_x * vect.m_x + this->m_y * vect.m_y;
        }

        /**
         * @brief transforme le vecteur courant en son vecteur normal
         * @return renvoie le vecteur normal du vecteur qui l'appelle
         */
        Vector2D Perpendicular() { return Vector2D(-1.0f *m_y, m_x); }
        
        /**
         * @brief calcule un vecteur collinéaire au vecteur normalisé courant
         * @return renvoie une vecteur collinéaire
         */
        Vector2D WithMagnitude(float mag) const {
            Vector2D v(m_x, m_y);
            v.Normalise();
            return v * mag;
        }
    };

    struct Node {
        Vector2D mainPos;
        Vector2D oldPos;
        Vector2D displacement;
        pixels color;

        int mass;
        float averageDisplacement;

        Node(){}
        Node (Vector2D vect, pixels mColor = pixels(255, 255, 255, 0), int weight = 3) 
            : oldPos(vect), color(mColor), mass(weight), mainPos(vect),
            displacement(0, 0), averageDisplacement(0.0f) {}

        /**
         * @brief Somme tous les vecteurs de déplacemment subis par un noeud
         */
        void AccumulateDisplacement(const Vector2D& vect) {
            displacement += vect;
            averageDisplacement++;
        }

        /**
         * @brief assigne la moyenne des vecteurs de déplacement accumulés par le noeud
         */
        void ApplyDisplacement() {
            if (averageDisplacement > 0) {
                mainPos += displacement / averageDisplacement;
                displacement = Vector2D(0, 0);
                averageDisplacement = 0;
            }
        }
    };

    /**
     * @brief Compose la couleur de fond et la couleur d'un objet affiché à l'écran
     * @param src le pixel de la forme à afficher à l'écran
     * @param dst le pixel de en arrière plan par dessus lequel on va dessiner la forme
     * courante
     * @return un pixel avec une couleur mélangeant habilement la couleur en arrière plan
     * et celle au premier plan
     */
    pixels BlendPixel(pixels src, pixels dst);

    
    void DrawLine(const std::unique_ptr<Node>& node, const std::unique_ptr<Node>& other, FrameBuffer& bf);
    void DrawLine(Vector2D& other, Vector2D& vect, pixels color, FrameBuffer& bf);
    
} // namespace SF