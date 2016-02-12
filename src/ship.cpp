#include "ship.h"

using namespace std;

Ship::Ship(vector<glm::vec3>& map): _map(map) {
    _model.load("spaceship");
    _model.translate(glm::vec3(0., 0., SHIP_HEIGHT));
    _roadPosition = glm::vec3(0., 0., SHIP_HEIGHT);
    _orientation = 0.;
    _curSegment = 0;
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

void Ship::move(float x) {
    float deltaX = x * cos(glm::radians(_orientation));
    float diff, adv;

    while (_roadPosition.x + deltaX >= _map[_curSegment][0]) {
        diff =  _map[_curSegment][0] - _roadPosition.x;
        adv = diff / cos(glm::radians(_orientation));
        x -= adv;
        _roadPosition.x = 0;
        _roadPosition.y += -adv * sin(glm::radians(_orientation));
        _curSegment++;
        _orientation -= glm::degrees(_map[_curSegment][1]);
        deltaX = x * cos(glm::radians(_orientation));

        //Ship follows the shape of the road
        _model.translate(glm::vec3(adv, 0., -SHIP_HEIGHT));
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., -1.));
        _model.rotate((float)_map[_curSegment][2], glm::vec3(0., 1., 0.));
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., 1.));
        _model.translate(glm::vec3(0., 0., SHIP_HEIGHT));

        cout << "Segment " << _curSegment << endl;
        }

    _roadPosition.x += deltaX;
    _model.translate(glm::vec3(x, 0., 0.));
    _roadPosition.y += - x * sin(glm::radians(_orientation));
    cout << "orientation " << _orientation << " ; x " << _roadPosition.x << " ; y " << _roadPosition.y << endl;
    }
