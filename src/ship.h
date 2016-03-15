#ifndef SHIP__H
#define SHIP__H

#include "model3d.h"
#include "../libs/glm/glm.hpp"
#include <vector>

#define SHIP_HEIGHT (0.4)
#define SHIP_ROUTINE_AMPLITUDE (0.05)
#define SHIP_ROUTINE_STEP (3.)
#define DECCELERATION_FACTOR (2.5) //p
#define ACCELERATION (0.01) //a
#define SPEED_REFERENCE (0.05) // v

class Ship : public Model3D {
    public:
        Ship(std::vector<glm::vec3>&);
        ~Ship();
        void turn(float); //Angle in degrees
        void manage(); //Perform physic actions, should be called at each frame
        void thrust(float);
		void setFriction(float);
        float getSpeed();
        glm::vec3 getAbsPos();
        glm::vec3 getVertical();
    protected:
        void move(float);
        void putOnRoad();

        std::vector<glm::vec3>& _map; //Reference to the map vector
        Model3D _model;
        int _curSegment;
        glm::vec3 _roadPosition; //Road-relative position of the ship, X being the local position in the current segment
        float _absPos; //Absolute road-relative x-position
        float _orientation; //Road-relative Z angle
        float _counter; //Used for the Z-position routine

        //INERTIA
        glm::vec2 _speed;
        float _inertiaAngle;
        float _abSpeed;
		float _frictionCoef;

		bool _gameOver;
    };

#endif
