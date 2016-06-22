#ifndef TEXT__H
#define TEXT__H

#include "model2d.h"
#include "bitmap.h"

#include <string>

class Video;

class Text : public Model2D{
    public:
        Text(glm::vec2, float);
        ~Text();
		void drawString(glm::vec2, std::string, Video&);
		void draw(Video& video);
		void draw(Video& video, glm::dmat4& modelMatrix);
    private:
        void loadTexture(std::string);
		Bitmap _texture;
    };

#endif
