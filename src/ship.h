#ifndef SHIP__H
#define SHIP__H

#include "model.h"
#include "../libs/glm/glm.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

#define SHIP_SCALE (0.2)
#define SHIP_HEIGHT (0.3)

class Ship {
    public:
        Ship(std::vector<glm::vec3>&);
        ~Ship();
        void draw(Video&);
        void turn(float); //Angle in degrees
        void move(float, float = 0); //Movement of (x, y) on its own axes
    protected:
        std::vector<glm::vec3>& _map;
        Model _model;
        glm::vec3 _roadPosition; //(Road-relative position of the ship)
        float _orientation; //Road-relative Z angle
    };

#endif
