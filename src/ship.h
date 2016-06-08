#ifndef SHIP__H
#define SHIP__H

#include "model3d.h"
#include "../libs/glm/glm.hpp"
#include <vector>

#define SHIP_HEIGHT (0.4)
#define SHIP_ROUTINE_AMPLITUDE (0.00) //0.05
#define SHIP_ROUTINE_STEP (3.)
#define DECCELERATION_FACTOR (2.5) //p
#define ACCELERATION (0.01) //a
#define SPEED_REFERENCE (0.1) // v

class Ship : public Model3D {
    public:
        Ship(std::vector<glm::dvec3>&);
        ~Ship();
        void turn(double); //Angle in degrees
        void manage(); //Perform physic actions, should be called at each frame
        void thrust(double);
		void setFriction(double);
        double getSpeed();
        glm::dvec3 getAbsPos();
        glm::dvec3 getVertical();
    protected:
        void move(double);
        void putOnRoad();

        std::vector<glm::dvec3>& _map; //Reference to the map vector
        Model3D _model;
        int _curSegment;
        glm::dvec3 _roadPosition; //Road-relative position of the ship, X being the local position in the current segment
        double _absPos; //Absolute road-relative x-position
        double _orientation; //Road-relative Z angle
        double _counter; //Used for the Z-position routine

        //INERTIA
        glm::dvec2 _speed;
        double _inertiaAngle;
        double _abSpeed;
		double _frictionCoef;

		bool _gameOver;
    };

#endif
