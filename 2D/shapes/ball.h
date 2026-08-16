#pragma once

#include "shapes.h"


namespace sf {
    
    class Ball : public Shapes {
        private:
        int m_radius;
        int m_weight;
        Node m_center;
        std::vector<std::unique_ptr<Node>> m_points;

        float m_chordLenght;
        float m_areaIdeal;
        float m_dstCenter;

        public:
        Ball() {}
        Ball(Node position, int radius, int weight, int links);
        ~Ball(){ }

        //GETTERS
        /**
         * @brief Recupère le centre de la forme
         * @return le centre la forme
         */
        Node GetCenter() { return m_center; }

        /**
         * @brief recupère le rayon de la forme
         * @return le rayon de la forme
         */
        int GetRadius() { return m_radius; }

        /**
         * @brief Recupère un point spécifique de la forme
         * @return renvoie un pointeur sur le point spécifique de la forme
         */
        std::unique_ptr<Node>& GetPoint(int index) override { return m_points[index]; }

        /**
         * @brief permet d'accéder à l'ensemble des points de la forme
         * @return le tableau dynamique sur l'ensemble des points de la forme
         */
        std::vector<std::unique_ptr<Node>>& GetAllPoints() override { return m_points; }

        //METHODES PUBLIQUES

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

        /**
         * @brief Détermine la position du centre virtuel parfait de la forme
         * @return la positon exacte du centre parfait du polygone
         */
        maths::Vector2D CentroidPsoition();

        /**
         * @brief donne une position fixe à un point de la forme
         * @param pos la position qu'on veut attribuer au point
         * @param index l'index du noeud de la forme auquel on veut attribuer
         */
        void SetPointfixedPosition(maths::Vector2D pos, int index);

        /**
         * @brief implémentation de l'intégration de verlet afin d'appliquer
         * un mouvement fluide au différentes particules d'un système d'étude
         * @param gravity la force de gravité appliquées à une particule
         * @param deltaTime c'est une variation très faible du temps
         */
        void VerletIntegretion(float clampX, float clampY ,float gravity, float deltaTime) override;

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
        void Render(render::Renderer& renderer, pixels color) override;


        void Update(float clampX, float clampY, float gravity, float deltaTime, int iteration = 4) override;
        void Update(float clampX, float clampY, float gravity, float deltaTime, int iteration , maths::Vector2D pos, maths::Vector2D pos2, int index, int index2);
        void FillShape(render::Renderer& renderer, pixels color) override;
    };

} // namespace sf
