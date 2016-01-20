#include "map.h"

using namespace std;
using namespace glm;

Map::Map() : Model() {
    _currentSegment = 0;
    _segmentCursor = 0;
    }

Map::~Map() {
    }

void Map::loadModel(string path) {
    vec3 temp;
    string buffer;
    ifstream map;
    int todo;
    path = "res/map/" + path + ".map";
    map.open(path.c_str());

    if (!map.is_open()) {
        cout << "Can't open " << path << endl;
        exit(EXIT_FAILURE);
        }

    while (getline(map, buffer)) {
        sscanf(buffer.c_str(), "%f,%f,%f:%d", &temp[0], &temp[1], &temp[2], &todo);
        temp[1] = radians(temp[1]);
        temp[2] = -radians(temp[2]);

        for (int i = 0 ; i < todo ; i++)
            _transMap.push_back(temp);
        }

    map.close();
    _vertexNb = 6 * _transMap.size();
    _mapModel = new float[3 * _vertexNb];
    _mapTex = new float[2 * _vertexNb];

    mat4 tmpmod = mat4(1.);
    float cursor = 0;
    vec4 act(0.); //Current origin

    for (int s = 0 ; s < (int)_transMap.size() ; s++) { //SEGMENT
        fillTex(s, cursor);
        vec4 t = tmpmod[yAxis];
        t *= ROAD_WIDTH;
        fillModel(6 * s, act - t); //V1 (tmpmod[1] -> Y vector)
        fillModel(6 * s + 5, act - t); //V6
        fillModel(6 * s + 1, act + t);

        act += tmpmod * vec4(toCartesian(_transMap[s]), 0.); //Matricial product of modelview & orientation vector
        tmpmod = rotate(tmpmod, _transMap[s][1], normalize(vec3(tmpmod[zAxis])));
        tmpmod = rotate(tmpmod, _transMap[s][2], normalize(vec3(tmpmod[yAxis])));
        tmpmod = translate(tmpmod, _transMap[s][0] * normalize(vec3(tmpmod[xAxis])));

        t = tmpmod[yAxis];
        t *= 0.1;
        fillModel(6 * s + 2, act + t);
        fillModel(6 * s + 3, act + t);
        fillModel(6 * s + 4, act - t);
        cursor += 1. / ROAD_WIDTH * _transMap[s][0];
        }
    }

void Map::fillModel(int vertex, vec4 v) {
    _mapModel[vertex * 3] = v[0];
    _mapModel[vertex * 3 + 1] = v[1];
    _mapModel[vertex * 3 + 2] = v[2];
    }

void Map::fillTex(int segment, float cursor) {
    int cur = 12 * segment;
    float rho = 1. / ROAD_WIDTH * _transMap[segment][0];
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

void Map::forward(float deltaX) {
    while (_segmentCursor + deltaX >= _transMap[_currentSegment][0]) {
        float diff =  _transMap[_currentSegment][0] - _segmentCursor;
        deltaX -= diff;
        _segmentCursor = 0;
        _currentSegment++;

        //LOOP
        if ((unsigned int)_currentSegment >= _transMap.size()) {
            _currentSegment = 0;
            break;
            }
        }

    _segmentCursor += deltaX;
    }
