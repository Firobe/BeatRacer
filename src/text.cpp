#include "text.h"
#include "video.h"

using namespace std;

Text::Text(glm::vec2 screen, float h) : Model2D(screen){
	_bitmap.setHeight(h);
}

Text::~Text(){
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
