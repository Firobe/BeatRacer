#ifndef GAME__H
#define GAME__H

#include "video.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#define PI 3.14159265

typedef struct Coord3D Coord3D;
struct Coord3D {
    float x, y, z;
    };

class Game {
    public:
        Game();
        ~Game();
        void loadMap(std::string);
        void drawMap();
        void placeCamera();
        void drawCamera();
        void forward(float);
    private:
        std::vector<CoordSPH> _map;
        int _currentSegment;
        float _segmentCursor;
        Coord3D _cameraEye;
        Coord3D _cameraVector;
    };

#endif
