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

    //PHYSICS MADAFAKA
    while (_roadPosition.x + deltaX >= _map[_curSegment][0]) {
        //How much until the next segment ?
        diff =  _map[_curSegment][0] - _roadPosition.x; //Absolute
        adv = diff / cos(glm::radians(_orientation)); //Relative

        //Moving to the next segment
        _model.translate(glm::vec3(adv, 0., 0.));
        _roadPosition.y += -adv * sin(glm::radians(_orientation)); //New Y-position


        //Entering in next segment
        _curSegment++;
        _roadPosition.x = - tan(_map[_curSegment][1]) * _roadPosition.y; //Adding delta-X caused by imbrication of segments
        _orientation -= glm::degrees(_map[_curSegment][1]); //Opposite orientation is added : new road-relative orientation

        //Bringing the ship at the frontier, facing the new segment
        _model.translate(glm::vec3(0., 0., -SHIP_HEIGHT));
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., -1.)); //

        //Applying new Y-rotation, ship is now in the new segment's plane
        _model.rotate((float)_map[_curSegment][2], glm::vec3(0., 1., 0.));

        //Restoring road-relative position and orientation
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., 1.)); //
        _model.translate(glm::vec3(0., 0., SHIP_HEIGHT));

        //Calculating remaining distance to move
        x -= adv; //Relative
        deltaX = x * cos(glm::radians(_orientation)); //Absolute


        cout << "Segment " << _curSegment << endl;
        }

    _model.translate(glm::vec3(x, 0., 0.)); //Movement of remaining distance
    _roadPosition.x += deltaX; //Adding remaining absolute distance
    _roadPosition.y += - x * sin(glm::radians(_orientation)); //Adding remaining Y-position
    }
