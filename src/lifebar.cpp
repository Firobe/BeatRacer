#include "lifebar.h"

using namespace std;

LifeBar::LifeBar() {
	load("lifebar");
	addUniform("value", 1);
	setUniform("value", 0.);
	_value = 0.;
	_shaderNb = 3;
	scale(glm::vec3(800, 600, 0));
}

LifeBar::~LifeBar(){
}

void LifeBar::draw(Video& video){
	video.render2D(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

float LifeBar::getValue(){
    return _value;
}

void LifeBar::setValue(float newValue){
    _value = newValue;
}
