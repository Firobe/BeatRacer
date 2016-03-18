#include "text.h"
#include "video.h"
#include "../libs/stb_truetype.h"
#include <iostream>

using namespace std;

Text::Text(glm::vec2 screen, float h) : Model2D(screen) {
    _bitmap.setHeight(h);
	_shaderNb = 4;
    }

Text::~Text() {
    }

void Text::loadTexture(string path) {
    _bitmap.setPath("res/fonts/" + path + ".ttf");
    _bitmap.load();
    }

void Text::draw(Video& video) {
    video.render2D(_vaoID, _vertexNb, _bitmap, this, _modelMatrix, _shaderNb);
    }

void Text::draw(Video& video, glm::mat4& modelMatrix) {
    video.render2D(_vaoID, _vertexNb, _bitmap, this, modelMatrix, _shaderNb);
    }


void Text::drawString(glm::vec2 pos, std::string str, Video& video) {
    float xpos = pos.x, ypos = pos.y;
    stbtt_aligned_quad tmp;

    for (unsigned int i = 0 ; i < str.size() ; i++) {
        tmp = _bitmap.charTexCoord(str[i], &xpos, &ypos);
        quadCoords(_mapModel, tmp.x0, tmp.x1, _screen.y - tmp.y0, _screen.y - tmp.y1);
        quadCoords(_mapTex, tmp.s0, tmp.s1, 1. - tmp.t0, 1. - tmp.t1);
        loadV();
        draw(video);
        }
    }
