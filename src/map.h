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

#define ROAD_WIDTH 2.

typedef struct Segment Segment;
struct Segment {
    glm::vec3 xAxis, yAxis, zAxis, origin;
    float length;
    };

class Model;

class Map : public Model {
    public:
        Map();
        ~Map();
        void loadModel(std::string);
        void forward(float);
        std::vector<glm::vec3>& getMap();
        glm::vec3 getWorldCoordinates(glm::vec3);
        glm::vec2 getWorldAngles(float); //THETA, PHI
    private:
        void fillModel(int vertex, glm::vec4 v);
        void fillTex(int, float);

        std::vector<glm::vec3> _transMap;
        std::vector<Segment> _segmentMap;
    };

#endif
