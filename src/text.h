#ifndef TEXT__H
#define TEXT__H

#include "model2d.h"
#include "bitmap.h"

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

class Text : public Model2D{
    public:
        Text(glm::vec2, float);
        ~Text();
    private:
        void loadTexture(std::string);
		Bitmap _texture;
    };

#endif
