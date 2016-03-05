#include "ship.h"

using namespace std;

Ship::Ship(vector<glm::vec3>& map): _map(map) {
    load("spaceship");
	_shaderNb = 1;
    translate(glm::vec3(0., 0., SHIP_HEIGHT));
    _roadPosition = glm::vec3(0., 0., SHIP_HEIGHT);
    _orientation = 0.;
    _speed.x = 0.;
    _abSpeed = 0;
    _speed.y = 0.;
    _inertiaAngle = 0.;
    _absPos = 0;
    _counter = 0;
    _curSegment = 0;

	//To send to shaders
	addUniform("speed", 1);
    }

Ship::~Ship() {
    }

glm::vec3 Ship::getVertical() {
    return glm::vec3(_modelMatrix[2]);
    }

void Ship::turn(float angle) { //Angle in degrees
    rotate((float)glm::radians(angle), glm::vec3(0., 0., 1.));
    _orientation += angle;
    }

void Ship::thrust(float x) {
    _speed += glm::vec2(x * cos(glm::radians(_orientation)), x * sin(glm::radians(_orientation)));
    }

float Ship::getSpeed() {
    return _abSpeed;
    }

void Ship::manage() {
    translate(glm::vec3(0., 0., -_roadPosition.z));
    _counter += SHIP_ROUTINE_STEP;

    if (_counter >= 360)
        _counter -= 360;

    _roadPosition.z = SHIP_HEIGHT + SHIP_ROUTINE_AMPLITUDE * sin(glm::radians(_counter));
    translate(glm::vec3(0., 0., _roadPosition.z));

    float hypo = sqrt(_speed.x * _speed.x + _speed.y * _speed.y);
	if(hypo > SPEED_CAP){
		_speed *= SPEED_CAP / hypo;
		hypo = SPEED_CAP;
	}

    if (hypo != 0.) {
        _inertiaAngle = glm::degrees(asin(_speed.y / hypo));
        move(hypo);
        _speed = glm::vec2(hypo * cos(glm::radians(_inertiaAngle)), hypo * sin(glm::radians(_inertiaAngle)));
        glm::vec2 dec(_speed);
        dec *= DECCELERATION;
        _speed -= dec;
        }

    putOnRoad(); //Bringing the bastard back
	
	//Setting uniforms
	setUniform("speed", _abSpeed);
	_uniform[0] = _abSpeed;
	if(_uniform[0] > 1.)
		_uniform[0] = 1.;
    }

void Ship::move(float x) { //x MUST be positive
    float deltaX = x * cos(glm::radians(_inertiaAngle));
    float diff, adv;
    _abSpeed = 0; //We are going to evaluate the X-distance the ship is making during this move

    //PHYSICS MADAFAKA
    while (_roadPosition.x + deltaX >= _map[_curSegment][0]) {
        //How much until the next segment ?
        diff =  _map[_curSegment][0] - _roadPosition.x; //Absolute
        _abSpeed += diff;
        adv = diff / cos(glm::radians(_inertiaAngle)); //Relative

        //Moving to the next segment
        //We are not moving forward but towards inertia point
        rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., 1.));
        translate(glm::vec3(adv, 0., 0.));
        //Restoring initial orientation
        rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., -1.));
        _roadPosition.y += -adv * sin(glm::radians(_inertiaAngle)); //New Y-position


        //Entering in next segment
        _curSegment++;

        if ((unsigned int)_curSegment >= _map.size())
            throw out_of_range("Out of map range");

        _roadPosition.x = - tan(_map[_curSegment][1]) * _roadPosition.y; //Adding delta-X caused by imbrication of segments
        _abSpeed -= tan(_map[_curSegment][1]) * _roadPosition.y; //Idem
        _orientation -= glm::degrees(_map[_curSegment][1]); //Opposite orientation is added : new road-relative orientation
        _inertiaAngle -= glm::degrees(_map[_curSegment][1]); //Opposite orientation is added : new road-relative orientation

        //Bringing the ship at the frontier, facing the new segment
        translate(glm::vec3(0., 0., -_roadPosition.z));
        rotate((float)glm::radians(_orientation), glm::vec3(0., 0., -1.)); //

        //Applying new Y-rotation, ship is now in the new segment's plane
        rotate((float)_map[_curSegment][2], glm::vec3(0., 1., 0.));

        //Restoring road-relative position and orientation
        rotate((float)glm::radians(_orientation), glm::vec3(0., 0., 1.)); //
        translate(glm::vec3(0., 0., _roadPosition.z));

        //Calculating remaining distance to move
        x -= adv; //Relative
        deltaX = x * cos(glm::radians(_inertiaAngle)); //Absolute
        }

    rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., 1.));
    translate(glm::vec3(x, 0., 0.)); //Movement of remaining distance
    rotate(glm::radians(_inertiaAngle - _orientation), glm::vec3(0., 0., -1.));

    _roadPosition.x += deltaX; //Adding remaining absolute distance
    _abSpeed += deltaX; //Idem
    _roadPosition.y += - x * sin(glm::radians(_inertiaAngle)); //Adding remaining Y-position
    _absPos += _abSpeed; //Updating general X-position
    }

glm::vec2 Ship::getAbsPos() {
    return glm::vec2(_absPos, _roadPosition.y);
    }

void Ship::putOnRoad() {
    if (abs(_roadPosition.y) < ROAD_WIDTH)
        return;

    rotate((float)glm::radians(_orientation), glm::vec3(0., 0., -1.));
    translate(glm::vec3(0., -((_roadPosition.y > 0. ? 1. : -1.) * ROAD_WIDTH - _roadPosition.y), 0.));
    _roadPosition.y = (_roadPosition.y > 0. ? 1 : -1) * ROAD_WIDTH;
    rotate((float)glm::radians(_orientation), glm::vec3(0., 0., 1.));
    _speed.y = 0;
    }
