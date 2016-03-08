#include "lifebar.h"

using namespace std;

LifeBar::LifeBar() {
	load("lifebar");
	addUniform("value", 1);
	setUniform("value", 0.);
	_value = 0.;
	
	scale(glm::vec3(100, 100, 0.));
}

LifeBar::~LifeBar(){
}

void LifeBar::draw(Video& video){
	video.render2D(_vaoID, _vertexNb, _texture, this, _modelMatrix, _shaderNb);
}

