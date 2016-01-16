#ifndef GAME__H
#define GAME__H

#include "video.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        void drawMap(Video&);
        void placeCamera();
        void forward(float);
    private:
        std::vector<glm::vec3> _transMap;
        glm::vec4 toCartesian(glm::vec3 v);
        void fillModel(int vertex, glm::vec4 v);
        float *_mapModel, *_mapColors;
        int _modelSize;
        int _currentSegment;
        float _segmentCursor;
    };

#endif
