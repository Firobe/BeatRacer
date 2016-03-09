#include "lifebar.h"
#include "video.h"

using namespace std;

LifeBar::LifeBar(glm::vec2 screen) : Model2D(screen){
	load("lifebar");
	_value = 0.;
	place(glm::vec2(0., screen.y - BAR_HEIGHT));
	setSize(glm::vec2(1., BAR_HEIGHT));
}

LifeBar::~LifeBar(){
}

float LifeBar::getValue(){
    return _value;
}

void LifeBar::setValue(float newValue){
    _value = newValue;
	setSize(glm::vec2(newValue, BAR_HEIGHT));
}
