#include "ship.h"

using namespace std;

Ship::Ship(vector<glm::vec3>& map): _map(map) {
    _model.load("spaceship");
    _model.translate(glm::vec3(0., 0., SHIP_HEIGHT));
    _roadPosition = glm::vec3(0., 0., SHIP_HEIGHT);
    _orientation = 0.;
    _speed.x = 0.;
    _abSpeed = 0;
    _speed.y = 0.;
    _inertiaAngle = 0.;
    _absPos = 0;
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

void Ship::thrust(float x) {
    _speed += glm::vec2(x * cos(glm::radians(_orientation)), x * sin(glm::radians(_orientation)));
    }

float Ship::getSpeed() {
    return _abSpeed;
    }

void Ship::manage() {
    _model.translate(glm::vec3(0., 0., -_roadPosition.z));
    _counter += SHIP_ROUTINE_STEP;

    if (_counter >= 360)
        _counter -= 360;

    _roadPosition.z = SHIP_HEIGHT + SHIP_ROUTINE_AMPLITUDE * sin(glm::radians(_counter));
    _model.translate(glm::vec3(0., 0., _roadPosition.z));

    float hypo = sqrt(_speed.x * _speed.x + _speed.y * _speed.y);

    if (hypo != 0.) {
        _inertiaAngle = glm::degrees(asin(_speed.y / hypo));
        move(hypo);
        _speed = glm::vec2(hypo * cos(glm::radians(_inertiaAngle)), hypo * sin(glm::radians(_inertiaAngle)));
        glm::vec2 dec(_speed);
        dec *= DECCELERATION;
        _speed -= dec;
        }
    }

void Ship::move(float x) { //x MUST be positive
    float deltaX = x * cos(glm::radians(_inertiaAngle));
    float diff, adv;
    _abSpeed = 0;

    //PHYSICS MADAFAKA
    while (_roadPosition.x + deltaX >= _map[_curSegment][0]) {
        //How much until the next segment ?
        diff =  _map[_curSegment][0] - _roadPosition.x; //Absolute
        _abSpeed += diff;
        adv = diff / cos(glm::radians(_inertiaAngle)); //Relative

        //Moving to the next segment
        //We are not moving forward but towards inertia point
        _model.rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., 1.));
        _model.translate(glm::vec3(adv, 0., 0.));
        //Restoring initial orientation
        _model.rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., -1.));
        _roadPosition.y += -adv * sin(glm::radians(_inertiaAngle)); //New Y-position


        //Entering in next segment
        _curSegment++;
        _roadPosition.x = - tan(_map[_curSegment][1]) * _roadPosition.y; //Adding delta-X caused by imbrication of segments
        _orientation -= glm::degrees(_map[_curSegment][1]); //Opposite orientation is added : new road-relative orientation
        _inertiaAngle -= glm::degrees(_map[_curSegment][1]); //Opposite orientation is added : new road-relative orientation

        //Bringing the ship at the frontier, facing the new segment
        _model.translate(glm::vec3(0., 0., -_roadPosition.z));
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., -1.)); //

        //Applying new Y-rotation, ship is now in the new segment's plane
        _model.rotate((float)_map[_curSegment][2], glm::vec3(0., 1., 0.));

        //Restoring road-relative position and orientation
        _model.rotate((float)glm::radians(_orientation), glm::vec3(0., 0., 1.)); //
        _model.translate(glm::vec3(0., 0., _roadPosition.z));

        //Calculating remaining distance to move
        x -= adv; //Relative
        deltaX = x * cos(glm::radians(_inertiaAngle)); //Absolute
        }

    _model.rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., 1.));
    _model.translate(glm::vec3(x, 0., 0.)); //Movement of remaining distance
    _model.rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., -1.));
    _abSpeed += deltaX;
    _absPos += deltaX;
    _roadPosition.x += deltaX; //Adding remaining absolute distance
    _roadPosition.y += - x * sin(glm::radians(_inertiaAngle)); //Adding remaining Y-position
    }

float Ship::getAbsPos() {
    return _absPos;
    }
