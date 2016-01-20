#ifndef MAP__H
#define MAP__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/ext.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "model.h"
#include "video.h"
#include "texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

#define ROAD_WIDTH 1.

class Map : public Model {
    public:
        Map();
        ~Map();
        void loadModel(std::string);
        void forward(float);
    private:
        std::vector<glm::vec3> _transMap;
        void fillModel(int vertex, glm::vec4 v);
        void fillTex(int, float);
        int _currentSegment;
        float _segmentCursor;
    };

#endif
