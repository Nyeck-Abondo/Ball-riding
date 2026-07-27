#pragma once

#include "shapes.h"

namespace sf {
    
    class Ellipse : public Shapes {
        private:
        int m_radiusA, m_radiusB;
        Node m_center;
        std::vector<std::unique_ptr<Node>> m_points;
        std::vector<Node> m_initialPosition;

        float m_chordLenght;
        float m_areaIdeal;

        public:
        Ellipse(Node position, int smallRadius, int largeRadius, int links);
        ~Ellipse() { };

        //GETTERS
        Node GetCenter() { return m_center; }
        int GetSmallRadius() { return m_radiusB; }
        int GetLargeRadius() { return m_radiusA; }
        std::unique_ptr<Node>& GetPoint(int index) override { return m_points[index]; }
        std::vector<std::unique_ptr<Node>>& GetAllPoints() override { return m_points; }

        //METHODES PUBLIQUES
        maths::Vector2D CentroidPsoition();
        void SetPointfixedPosition(maths::Vector2D pos, int index);

        void ApplyDisplacement(maths::Vector2D direction, int intesity) override;
        
        /**
         * @brief Restrein la distance que peut prendre deux points l'un de l'autre à cause de l'application
         * de la moyenne des déplacements appliquées sur chacun d'eux en applicant un vecteur de deplacement
         * dans la direction opposée de leur mouvement pour les rapprocher ou encore les repousser
         */
        void ApplyDistanceConstraint()override;

        /**
         * @brief Contrainte d'aire. Cette fonction impose une aire idéale que le corps mou doit conserver. Elle
         * applique sur chacun des points constituant la forme un vecteur normal à son déplacement dirigé vers
         * l'extérieur de la forme
         */
        void ApplyDilatationConstraint()override;

        void ApplyCenterConstraint(float stifness = 0.5f);

        /**
         * @brief implémentation de l'intégration de verlet afin d'appliquer
         * un mouvement fluide au différentes particules d'un système d'étude
         * @param gravity la force de gravité appliquées à une particule
         * @param deltaTime c'est une variation très faible du temps
         */
        void VerletIntegretion(float clampX, float clampY, float gravity, float deltaTime) override;

        /**
         * @brief calcule l'aire courant du corps mou durant sa déformation
         * @return l'aire du corps à un instant T
         */
        float ShapeArea() override;

        /**
         * @brief S'occupe de rendu de la pate de l'animal. dessine les formes géométriques la composant
         * @param fb référence sur le tampon en mémoir utilisé pour afficher les pixels de la pate à l'écran
         * @param color la couleur que prend le membre durant la phase de rendu
         */
        void Render(FrameBuffer& fb, pixels color) override;
        void Update(float clampX, float clampY, float gravity, float deltaTime, int iteration = 4) override;
        void Update(float clampX, float clampY, float gravity, float deltaTime, int iteration , maths::Vector2D pos, maths::Vector2D pos2, int index, int index2);
        void FillShape(FrameBuffer& fb, pixels color) override;
    };

} // namespace sf
