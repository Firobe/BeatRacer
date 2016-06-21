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
    glm::dvec3 xAxis, yAxis, zAxis, origin;
    double length;
    };

class Map : public Model3D {
    public:
        Map();
        ~Map();
        void loadModel(std::string);
        void forward(double);
        std::vector<glm::dvec3>& getMap();
        glm::dvec3 getWorldCoordinates(glm::dvec3);
        glm::dmat3 getWorldOrientation(double); //THETA, PHI
		void setMapSeg(unsigned int, glm::dvec4);
		void write(std::string);//contains a string
		glm::dvec4 getSegment(unsigned int);
    private:
		void loadTexture(std::string path) { Model::loadTexture("map"); }
        void fillModel(int vertex, glm::dvec4 v);
        void fillTex(int, double);

		std::vector<glm::dvec4> _fileMap;
        std::vector<glm::dvec3> _transMap;
        std::vector<Segment> _segmentMap;
    };

#endif
