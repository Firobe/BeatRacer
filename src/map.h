#ifndef MAP__H
#define MAP__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

#include "model3d.h"
#include <vector>
#include <string>

#define ROAD_WIDTH 2.

typedef struct Segment Segment;
struct Segment {
    glm::vec3 xAxis, yAxis, zAxis, origin;
    float length;
    };

class Map : public Model3D {
    public:
        Map();
        ~Map();
        void loadModel(std::string);
        void forward(float);
        std::vector<glm::vec3>& getMap();
        glm::vec3 getWorldCoordinates(glm::vec3);
        glm::mat3 getWorldOrientation(float); //THETA, PHI
    private:
        void fillModel(int vertex, glm::vec4 v);
        void fillTex(int, float);

        std::vector<glm::vec3> _transMap;
        std::vector<Segment> _segmentMap;
    };

#endif
