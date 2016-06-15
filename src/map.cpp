#include "map.h"
#include "video.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

enum Axes { xAxis, yAxis, zAxis };

using namespace std;
using namespace glm;

Map::Map() : Model3D() {
    }

Map::~Map() {
    }

vector<glm::dvec3>& Map::getMap() {
    return _transMap;
    }

glm::dvec4 Map::getSegment(unsigned int n) {
    if (n < _fileMap.size())
        return _fileMap[n];
    else return glm::dvec4(1., 0., 0., 1);
    }

void Map::setMapSeg(unsigned int n, glm::dvec4 seg) {
    if (n >= _fileMap.size())
        _fileMap.resize(n + 1);

    _fileMap[n] = seg;
    }

void Map::write(string path) {
    path = "res/map/" + path + ".map";
    ofstream map(path);

    for (glm::dvec4 v : _fileMap) {
        map << v[0] << "," << v[1] << "," << v[2] << ":" << v[3] << endl;
        }
    }

void Map::loadModel(string in) {
    string buffer;
    ifstream map;
    string path = "res/map/" + in + ".map";

    if (in != "editor") {
        dvec4 temp;
        int todo;
        map.open(path);

        if (!map)
            throw runtime_error("Unable to open " + path);

        while (getline(map, buffer)) {
            if (sscanf(buffer.c_str(), "%lf,%lf,%lf:%d", &temp[0], &temp[1], &temp[2], &todo) != 4)
                throw runtime_error("Bad map format : \"" + buffer + "\"");

            temp[3] = todo;
            _fileMap.push_back(temp);
            }
        }

    _transMap.clear();

    for (glm::dvec4 v : _fileMap) {
        if (in != "editor")
            while (v[1] >= 0.001 || v[2] >= 0.001) {
                v[0] /= 2.;    //On s'assure que les angles sont suffisamment petits
                v[1] /= 2. ;
                v[2] /= 2. ;
                v[3] *= 2.;
                }

        for (int i = 0 ; i < v[3] ; i++)
            _transMap.push_back(glm::dvec3(v[0], radians(v[1]), -radians(v[2])));
        }
	if(in != "editor")
		cout << "Map contains " << _transMap.size() << " sectors." << endl;
    _segmentMap.resize(_transMap.size());
    _vertexNb = 6 * _transMap.size();

    _mapModel.resize(3 * _vertexNb);
    _mapTex.resize(2 * _vertexNb);

    dmat4 tmpmod = dmat4(1.);
    double cursor = 0;
    dvec4 act(0.); //Current origin

    for (int s = 0 ; s < (int)_transMap.size() ; s++) { //SEGMENT
        _segmentMap[s].origin = dvec3(act);
        _segmentMap[s].length = _transMap[s][0];
        _segmentMap[s].xAxis = dvec3(tmpmod[xAxis]);
        _segmentMap[s].yAxis = dvec3(tmpmod[yAxis]);
        _segmentMap[s].zAxis = dvec3(tmpmod[zAxis]);

        fillTex(s, cursor);
        dvec4 t = tmpmod[yAxis];
        t *= ROAD_WIDTH;
        //Vertices for first half of the two triangles
        fillModel(6 * s, act - t); //V1 (tmpmod[1] -> Y vector)
        fillModel(6 * s + 5, act - t); //V6
        fillModel(6 * s + 1, act + t);

        act += tmpmod * dvec4(toCartesian(_transMap[s]), 0.); //Matricial product of model matrix & orientation vector
        tmpmod = glm::rotate(tmpmod, _transMap[s][1], dvec3(0, 0, 1));
        tmpmod = glm::rotate(tmpmod, _transMap[s][2], dvec3(0, 1, 0));
        tmpmod = glm::translate(tmpmod, _transMap[s][0] * dvec3(1, 0, 0));
        t = tmpmod[yAxis];
        t *= ROAD_WIDTH;

        //vertices for second half
        fillModel(6 * s + 2, act + t);
        fillModel(6 * s + 3, act + t);
        fillModel(6 * s + 4, act - t);
        cursor += 1. / ROAD_WIDTH * _transMap[s][0];
        }
    }

void Map::fillModel(int vertex, dvec4 v) {
    //writing a vertex into the model array
    _mapModel[vertex * 3] = v[0];
    _mapModel[vertex * 3 + 1] = v[1];
    _mapModel[vertex * 3 + 2] = v[2];
    }

void Map::fillTex(int segment, double cursor) {
    //Texture coordinates
    int cur = 12 * segment;
    double rho = 1. / ROAD_WIDTH * _transMap[segment][0];
    _mapTex[cur] = 1.;
    _mapTex[cur + 1] = cursor;
    _mapTex[cur + 2] = 0.;
    _mapTex[cur + 3] = cursor;
    _mapTex[cur + 4] = 0.;
    _mapTex[cur + 5] = cursor + rho;
    _mapTex[cur + 6] = 0.;
    _mapTex[cur + 7] = cursor + rho;
    _mapTex[cur + 8] = 1.;
    _mapTex[cur + 9] = cursor + rho;
    _mapTex[cur + 10] = 1.;
    _mapTex[cur + 11] = cursor;
    }

glm::dvec3 Map::getWorldCoordinates(glm::dvec3 roadCoord) {
    static double curLen = 0.;
    static int seg = 0;

    while (seg - 1 >= 0 && curLen - roadCoord.x >= _segmentMap[seg - 1].length) {
        curLen -= _segmentMap[seg - 1].length;
        seg--;
        }

    while (curLen < roadCoord.x && (unsigned int)seg < _segmentMap.size()) {
        curLen += _segmentMap[seg].length;
        seg++;
        }

    if ((unsigned int)seg >= _segmentMap.size() || seg < 0)
        throw out_of_range("Out of map range");

    return _segmentMap[seg].origin + (roadCoord.x - curLen) * _segmentMap[seg].xAxis
           - roadCoord.y * _segmentMap[seg].yAxis + roadCoord.z * _segmentMap[seg].zAxis;
    }

glm::dmat3 Map::getWorldOrientation(double xPos) { //RADIANS
    static double curLen = 0.;
    static int seg = 0;

    while (seg - 1 >= 0 && curLen - xPos >= _segmentMap[seg - 1].length) {
        curLen -= _segmentMap[seg - 1].length;
        seg--;
        }

    while (curLen < xPos && (unsigned int)seg < _segmentMap.size()) {
        curLen += _segmentMap[seg].length;
        seg++;
        }

    if ((unsigned int)seg >= _segmentMap.size() || seg < 0)
        throw out_of_range("Out of map range");

    return glm::dmat3(_segmentMap[seg].xAxis, _segmentMap[seg].yAxis, _segmentMap[seg].zAxis);
    }
