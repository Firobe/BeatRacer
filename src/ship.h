#ifndef SHIP__H
#define SHIP__H

#include "map.h"
#include "model.h"
#include "../libs/glm/glm.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

#define SHIP_HEIGHT (0.4)
#define SHIP_ROUTINE_AMPLITUDE (0.05)
#define SHIP_ROUTINE_STEP (3.)

class Ship {
    public:
        Ship(std::vector<glm::vec3>&);
        ~Ship();
        void draw(Video&);
        void turn(float); //Angle in degrees
        void move(float); //Movement of (x, y) on its own axes
    protected:
        std::vector<glm::vec3>& _map; //Reference to the map vector
        Model _model;
        int _curSegment;
        glm::vec3 _roadPosition; //Road-relative position of the ship, X being the local position in the current segment
        float _orientation; //Road-relative Z angle
        float _counter; //Used for the Z-position routine
    };

#endif
