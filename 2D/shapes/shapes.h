#pragma once

#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "../render2D/algorithm2D.h"

namespace sf {
    
    class Shapes {
        public:
        virtual ~Shapes() = default;

        virtual std::unique_ptr<Node>& GetPoint(int index) = 0;
        virtual std::vector<std::unique_ptr<Node>>& GetAllPoints() = 0;

        virtual void ApplyDisplacement(maths::Vector2D direction, int intesity) = 0;

        /**
         * @brief Restrein la distance que peut prendre deux points l'un de l'autre à cause de l'application
         * de la moyenne des déplacements appliquées sur chacun d'eux en applicant un vecteur de deplacement
         * dans la direction opposée de leur mouvement pour les rapprocher ou encore les repousser
         */
        virtual void ApplyDistanceConstraint() = 0;

        /**
         * @brief Contrainte d'aire. Cette fonction impose une aire idéale que le corps mou doit conserver. Elle
         * applique sur chacun des points constituant la forme un vecteur normal à son déplacement dirigé vers
         * l'extérieur de la forme
         */
        virtual void ApplyDilatationConstraint() = 0;

        /**
         * @brief implémentation de l'intégration de verlet afin d'appliquer
         * un mouvement fluide au différentes particules d'un système d'étude
         * @param gravity la force de gravité appliquées à une particule
         * @param deltaTime c'est une variation très faible du temps
         */
        virtual void VerletIntegretion(float clampX, float clampY, float gravity, float deltaTime) = 0;

        /**
         * @brief calcule l'aire courant du corps mou durant sa déformation
         * @return l'aire du corps à un instant T
         */
        virtual float ShapeArea() = 0;

        /**
         * @brief S'occupe de rendu de la pate de l'animal. dessine les formes géométriques la composant
         * @param fb référence sur le tampon en mémoir utilisé pour afficher les pixels de la pate à l'écran
         * @param color la couleur que prend le membre durant la phase de rendu
         */
        virtual void Render(FrameBuffer& fb, pixels color) = 0;

        /**
         * @brief Mets à jour la position de la forme aucours du temps
         * @param gravity la force de gravité par défaut appliquée à la forme dans la simulation
         * @param deltaTime un intervalle de temps infime calculé entre chaque frame du jeu
         * @param iteration c'est le nombre de fois que les confraites physiques sont appliquées
         * à la forme générique par frames
         */
        virtual void Update(float clampX, float clampY, float gravity, float deltaTime, int iteration = 4) = 0;

        /**
         * @brief Rempli une forme quelconque
         * @param fb Tampon en mémoire chargé de passé les couleurs de pixels calculé par le CPU
         * à l'écran
         * @param color la couleur de remplissage de la forme
         */
        virtual void FillShape(FrameBuffer& fb, pixels color) = 0;
    };

} // namespace sf_shapes
