#ifndef GAME__H
#define GAME__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/ext.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "video.h"
#include "texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#define PI 3.14159265
#define ROAD_WIDTH 0.1

class Game {
    public:
        Game();
        ~Game();
        void loadMap(std::string);
        void loadV();
        void drawMap(Video&);
        void placeCamera();
        void forward(float);
    private:
        std::vector<glm::vec3> _transMap;
        glm::vec4 toCartesian(glm::vec3 v);
        Texture _texture;
        void fillModel(int vertex, glm::vec4 v);
        void fillTex(int, float);
        float *_mapModel, *_mapTex;
        int _modelSize;
        int _currentSegment;
        float _segmentCursor;
        GLuint _vboID, _vaoID;
    };

#endif
