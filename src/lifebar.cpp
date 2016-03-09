#include "lifebar.h"
#include "video.h"

using namespace std;

LifeBar::LifeBar(glm::vec2 screen) : Model2D(screen){
	load("lifebar");
	_value = 0.;
	//place(glm::vec2(200, 200));
	setSize(glm::vec2(100., 100.));
}

LifeBar::~LifeBar(){
}

float LifeBar::getValue(){
    return _value;
}

void LifeBar::setValue(float newValue){
    _value = newValue;
}
