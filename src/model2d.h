#ifndef MODEL2D__H
#define MODEL2D__H

#include "model.h"
#include <string>
#include <vector>

class Video;

class Model2D : public Model {
    public:
        Model2D(glm::vec2);
        ~Model2D();
        virtual void draw(Video&); //Render the model
        virtual void draw(Video&, glm::dmat4&); //Render the model
		void setSize(glm::vec2);
		void place(glm::vec2);
    protected:
        virtual void loadModel(std::string);
        virtual void loadV();

		glm::vec2 _screen, _curScale;
    };

void quadCoords(std::vector<float>& data, float x1, float x2, float y1, float y2, bool revert = false);
#endif
