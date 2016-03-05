#ifndef SHIP__H
#define SHIP__H

#include "map.h"
#include "model.h"
#include "../libs/glm/glm.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdexcept>

#define SHIP_HEIGHT (0.4)
#define SHIP_ROUTINE_AMPLITUDE (0.05)
#define SHIP_ROUTINE_STEP (3.)
#define DECCELERATION (0.0155)
#define ACCELERATION (0.001)
#define SPEED_CAP (0.1)

class Ship : public Model {
    public:
        Ship(std::vector<glm::vec3>&);
        ~Ship();
        void turn(float); //Angle in degrees
        void manage(); //Perform physic actions, should be called at each frame
        void thrust(float);
        float getSpeed();
        glm::vec2 getAbsPos();
        glm::vec3 getVertical();
    protected:
        void move(float);
        void putOnRoad();

        std::vector<glm::vec3>& _map; //Reference to the map vector
        Model _model;
        int _curSegment;
        glm::vec3 _roadPosition; //Road-relative position of the ship, X being the local position in the current segment
        float _absPos; //Absolute road-relative x-position
        float _orientation; //Road-relative Z angle
        float _counter; //Used for the Z-position routine

        //INERTIA
        glm::vec2 _speed;
        float _inertiaAngle;
        float _abSpeed;
    };

#endif
