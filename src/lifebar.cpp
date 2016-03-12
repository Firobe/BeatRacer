#include "lifebar.h"
#include "video.h"

using namespace std;

LifeBar::LifeBar(glm::vec2 screen) : Model2D(screen), _back(screen){
    load("lifebar");
	_back.load("lifebar_back");
	_back.place(glm::vec2(0., screen.y - BAR_HEIGHT));
	_back.setSize(glm::vec2(screen.x, BAR_HEIGHT));
	setUniform("calpha", 0.6);
    _value = 100.;
    place(glm::vec2(0., screen.y - BAR_HEIGHT));
    setSize(glm::vec2(1., BAR_HEIGHT));
    }

LifeBar::~LifeBar() {
    }

float LifeBar::getValue() {
    return _value;
    }

void LifeBar::draw(Video& vid){
	_back.draw(vid);
	Model2D::draw(vid);
}

void LifeBar::setValue(float newValue) {
    _value = newValue < 0 ? 0 : (newValue > 100 ? 100 : newValue);
    setSize(glm::vec2(newValue * _screen.x / 100, BAR_HEIGHT));
    }
