#pragma once

#include <cmath>
#include <vector>

namespace sf {
    namespace maths {

        template <typename T>
        struct Vector2DT {
            T m_x;
            T m_y;

            Vector2DT(T posX = 0, T posY = 0) : m_x(posX), m_y(posY) { }

            /**
             * @brief opérateur de soustraction
             * @return la resultante de deux vecteurs
             */
            Vector2DT operator-(Vector2DT& vect) {
                return Vector2DT(this->m_x - vect.m_x, this->m_y - vect.m_y);
            }

            /**
             * @brief opérateur de soustraction
             * @return la resultante de deux vecteurs
             */
            Vector2DT operator-(const Vector2DT& vect) const {
                return Vector2DT(this->m_x - vect.m_x, this->m_y - vect.m_y);
            }

            /**
             * @brief opérateur d'addition
             * @return la somme de vecteurs
             */
            Vector2DT operator+(Vector2DT vect) {
                return Vector2DT(this->m_x + vect.m_x, this->m_y + vect.m_y);
            }

            /**
             * @brief opérateur de division
             * @return division d'un vecteur par un scalaire
             */
            Vector2DT operator/(T scalar) {
                return Vector2DT(this->m_x / scalar, this->m_y / scalar);
            }

            /**
             * @brief opérateur de multiplication
             * @return le produit d'un vecteur par un scalaire
             */
            Vector2DT operator*(T scalar) {
                return Vector2DT(this->m_x * scalar, this->m_y * scalar);
            }

            void operator= (const Vector2DT& v) {
                m_x = v.m_x;
                m_y = v.m_y;
            }

            /**
             * @brief opérateur d'addition
             * @return somme de deux vecteurs
             */
            Vector2DT& operator+= (const Vector2DT& v) { 
                m_x += v.m_x;
                m_y += v.m_y;
                return *this;
            }

            /**
             * @brief Calcule la distance entre deux points
             */
            T Distance(Vector2DT& other) {
                return std::sqrt((this->m_x - other.m_x) * (this->m_x - other.m_x) + ((this->m_y - other.m_y) * (this->m_y - other.m_y)));
            }

            /**
             * @brief calcule la norme d'un vecteur
             * @return la norme du vecteur
             */
            T Norme() {
                return std::sqrt((this->m_x * this->m_x) + (this->m_y * this->m_y));
            }

            /**
             * @brief normalise un vecteur
             */
            void Normalise() {
                T n = Norme();
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
            T DotProduct(Vector2DT& vect) {
                return this->m_x * vect.m_x + this->m_y * vect.m_y;
            }

            /**
             * @brief transforme le vecteur courant en son vecteur normal
             * @return renvoie le vecteur normal du vecteur qui l'appelle
             */
            Vector2DT Perpendicular() { return Vector2DT(-1.0f *m_y, m_x); }
            
            /**
             * @brief calcule un vecteur collinéaire au vecteur normalisé courant
             * @return renvoie une vecteur collinéaire
             */
            Vector2DT WithMagnitude(T mag) const {
                Vector2DT v(m_x, m_y);
                v.Normalise();
                return v * mag;
            }

            void Swap(Vector2DT& P0) {
                Vector2DT temp;
                temp = P0;
                P0 = *this;
                *this = temp;
            }
        };

        //types d'usage courants
        using Vector2D      =   Vector2DT<float>;
        using Vector2Dd     =   Vector2DT<double>;
        using Vector2Di     =   Vector2DT<int>;
        using Vector2Dld    =   Vector2DT<long double>;
        
        template <typename T>
        float lerp(const T& vect0, const T& vect1, float x) {
            float x0 = vect0.m_x, x1 = vect1.m_x;
            float y0 = vect0.m_y, y1 = vect1.m_y;

            if (std::fabs(x1 - x0) < 1e-5f) return (y0 + y1) * 0.5f;

            float t = (x - x0) / (x1 - x0);
            return (1 - t) * y0 + t * y1;
        }

        template <typename T>
        T lerpEx(const T& vect0, const T& vect1, float speed, float dt) {
            float x0 = vect0.m_x, x1 = vect1.m_x;
            float y0 = vect0.m_y, y1 = vect1.m_y;

            float t = 1.0f - std::exp(-speed * dt);

            return {x0 + (x1 - x0) * t, y0 + (y1 - y0) * t};
        }
        
        template <typename T>
        T Rotation(const T& center, const T& point, float angle);

        template <typename T>
        T Translate(T vect, T other);

        template <typename T>
        T symetric(T vect, T other);

        std::vector<float> Interpolated (Vector2D i, Vector2D j);

    } // namespace maths
} // namespace sf
