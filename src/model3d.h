#ifndef MODEL3D__H
#define MODEL3D__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

#include "model.h"
#include <string>

class Video;

class Model3D : public Model {
    public:
        Model3D();
        ~Model3D();
        virtual void draw(Video&); //Render the model
        virtual void draw(Video&, glm::dmat4&); //Render the model
        virtual void loadV();
        virtual void loadModel(std::string);
    };

#endif
