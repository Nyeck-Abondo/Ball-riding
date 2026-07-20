#pragma once

#include "../../render2D/algorithm2D.h"

namespace SF {
    //==================================================    
    // ENUM: FootType
    // DESCRIPTION: Elle défini le type de pied, gauche ou
    // droit de l'animal
    //==================================================
    enum FootType {
        Left,
        rigth
    };

    //====================================================     
    // STRUCT : Anchor
    // DESCRIPTION : Point spécial doté d'un rayon, permettant de représenter une structure complexe
    // d'un animal par des segments reliés les uns au autres.
    //==========================================================
    struct Anchor {
        Vector2D m_pos;
        Vector2D m_oldPos;
        int m_radius;
        Vector2D displacement;
        int average = 0;

        Anchor() {}
        Anchor(Vector2D pos, int radius) : m_pos(pos), m_oldPos(pos), m_radius(radius) {}

        /**
         * @brief somme les vecteur de déplacements appliqué à une ancre spécifique
         */
        void AccumulateDisplacement(const Vector2D& move) {
            displacement += move;
            average++;
        }

        /**
         * @brief applique la moyenne des vecteurs de déplacement acumulé
         * sur une ancre
         */
        void ApplyDisplacement() {
            if (average > 0) {
                m_pos += displacement / average;
                displacement = Vector2D(0, 0);
                average = 0;
            }
        }
    };
    
    //=====================================================================================================
    // CLASS : Legs
    // DESCRIPTION : Membre inférieur des animaux bipédiques et quadrupédiques. Elle permet la création
    // simplifié d'une pate inférieur qu'elle soit droite ou gauche selon le type choisi
    //=====================================================================================================
    class Legs {
        private:
        FootType m_type;
        Anchor m_thighAncre;
        Anchor m_legAncre;
        Anchor m_footAncre;
        float m_angle;
        float m_chordLenght;
        std::vector<std::unique_ptr<Node>> m_node;
        std::vector<std::unique_ptr<Anchor>> m_ancre;

        public:
        Legs(FootType type, Vector2D& position, int thighLenght,int Leglenght, int thighWeight, int legWeight);
        ~Legs() {}

        /**
         * @brief Trouve l'angle entre la cuisse de l'animal et sa jambe
         * @return l'angle entre la cuisse et la jambe en radiant
         */
        float FindAngleThighLeg();

        /**
         * @brief Applique la contrainte de rotation sur les articulations
         */
        void ApplyAngleConstraint();

        /**
         * @brief implémentation de l'intégration de verlet afin d'appliquer
         * un mouvement fluide au différentes particules d'un système d'étude
         * @param gravity la force de gravité appliquées à une particule
         * @param deltaTime c'est une variation très faible du temps
         */
        void VerleIntegretion(float gravity , float deltaTime);

        /**
         * @brief applique des contraintes de distantces entre les ancres constituant
         * les articulations de l'animal
         * @param fixedPosition position à laquelle l'articulation de la cuisse est fixée
         * sur le corp de l'animal
         */
        void ApplyDistanceConstraint(Vector2D fixedPosition);

        /**
         * @brief Donne une position fixe à l'ancre d'une articulation tout le long de la 
         * simulation / durée de vie de l'animal
         * @param position le point où l'ancre doit être placée
         */
        void SetAnchorPosition(Vector2D& position);

        /**
         * @brief met à jour la position de la pâte tout le long de la simulatoin
         * @param gravity la force de graité appliquée à chaque particules du système
         * @param deltaTime le plus petit intervalle de temps nécessaire à la mise à jour temporelle
         * du mouvement de la pâte
         * @param fixedPosition la position fixe de l'ancre représentant l'articulation reliée directement
         * au corps de l'animal
         */
        void Update(float gravity , float deltaTime, Vector2D fixedPosition);

        /**
         * @brief S'occupe de rendu de la pate de l'animal. dessine les formes géométriques la composant
         * @param fb référence sur le tampon en mémoir utilisé pour afficher les pixels de la pate à l'écran
         * @param color la couleur que prend le membre durant la phase de rendu
         */
        void Render(FrameBuffer& fb ,pixels color);
    };

} // namespace SF
