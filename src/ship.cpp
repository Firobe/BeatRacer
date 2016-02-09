#include "ship.h"

using namespace std;

Ship::Ship(vector<glm::vec3>& map): _map(map) {
    _model.load("spaceship");
    _model.scale(glm::vec3(SHIP_SCALE));
    _roadPosition = glm::vec3(0., 0., SHIP_HEIGHT);
    _orientation = 0.;
    }

Ship::~Ship() {
    }

void Ship::draw(Video& v) {
    _model.draw(v);
    }

void Ship::turn(float angle) { //Angle in degrees
    _model.rotate((float)glm::radians(angle), glm::vec3(0., 0., 1.));
    _orientation += angle;
    }

void Ship::move(float x, float y) {
    _model.translate(glm::vec3(x, y, 0.));
    _roadPosition.x += sin(glm::radians(_orientation));
    _roadPosition.y += cos(glm::radians(_orientation));
    }
