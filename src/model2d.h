#ifndef MODEL2D__H
#define MODEL2D__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

#include "model.h"
#include <string>

class Video;

class Model2D : public Model {
    public:
        Model2D(glm::vec2);
        ~Model2D();
        virtual void draw(Video&); //Render the model
        virtual void draw(Video&, glm::mat4&); //Render the model
		void setSize(glm::vec2);
		void place(glm::vec2);
    protected:
        virtual void loadModel(std::string);
        virtual void loadV();

		glm::vec2 _screen, _curScale;
    };

void quadCoords(float* data, float x1, float x2, float y1, float y2, bool revert = false);
#endif
