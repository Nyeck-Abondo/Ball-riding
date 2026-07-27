#pragma once

#include <cmath>

namespace sf {
    namespace maths {

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
             * @brief opérateur de soustraction
             * @return la resultante de deux vecteurs
             */
            Vector2D operator-(const Vector2D& vect) const {
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

        float lerp(const Vector2D& vect0, const Vector2D& vect1, float x);
        
        Vector2D Rotation(const Vector2D& center, const Vector2D& point, float angle);

        Vector2D Translate(Vector2D vect, Vector2D other);

        Vector2D symetric(Vector2D vect, Vector2D other);

    } // namespace maths
} // namespace sf
