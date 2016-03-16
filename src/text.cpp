#include "text.h"

using namespace std;

Text::Text(glm::vec2 screen, float h) : Model2D(screen){
	_texture.setHeight(h);
}

Text::~Text(){
}

void Text::loadTexture(string path) {
	_texture.setPath("res/fonts/" + path + ".ttf");
	_texture.load();
}

