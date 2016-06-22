#include "text.h"
#include "video.h"
#include "../libs/stb_truetype.h"

using namespace std;

Text::Text(glm::vec2 screen, float h) : Model2D(screen) {
    _texture.setHeight(h);
	_shaderNb = 4;
    }

Text::~Text() {
    }

void Text::loadTexture(string path) {
    _texture.setPath("res/fonts/" + path + ".ttf");
    _texture.load();
    }

void Text::draw(Video& video) {
    video.render2D(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
    }

void Text::draw(Video& video, glm::dmat4& modelMatrix) {
    video.render2D(_vaoID, _vertexNb, _texture, this, modelMatrix, _shaderNb);
    }


void Text::drawString(glm::vec2 pos, std::string str, Video& video) {
    float xpos = pos.x, ypos = pos.y;
    stbtt_aligned_quad tmp;

    for (char c : str) {
        tmp = _texture.charTexCoord(c, &xpos, &ypos);
        quadCoords(_mapModel, tmp.x0, tmp.x1, _screen.y - tmp.y0, _screen.y - tmp.y1);
        quadCoords(_mapTex, tmp.s0, tmp.s1, 1. - tmp.t0, 1. - tmp.t1);
        loadV();
        draw(video);
        }
    }
