#include "game.h"

using namespace std;
using namespace glm;

Game::Game() {
    _currentSegment = 0;
    _segmentCursor = 0;
    }

Game::~Game() {
    }

void Game::loadMap(string path) {
    cout << "Processing map...";
    vec3 temp;
    string buffer;
    ifstream map;
    int todo;
    map.open(path.c_str());

    while (getline(map, buffer)) {
        sscanf(buffer.c_str(), "%f,%f,%f:%d", &temp[0], &temp[1], &temp[2], &todo);
        temp[1] *= PI / 180.;
        temp[2] *= -PI / 180.;

        for (int i = 0 ; i < todo ; i++)
            _transMap.push_back(temp);
        }

    map.close();
    _modelSize = 3 * 6 * _transMap.size();
    _mapModel = new float[_modelSize];
    _mapColors = new float[_modelSize];

    for (int i = 0 ; i < _modelSize ; i++)
        _mapColors[i] = (float)((((i / (3 * 6) + 5) * 20) % 206) + 50) / 256.;

    mat4 tmpmod = mat4(1.);
    vec4 act(0.); //Current origin

    for (int s = 0 ; s < (int)_transMap.size() ; s++) { //SEGMENT
        vec4 t = tmpmod[1];
        t *= ROAD_WIDTH;
        fillModel(6 * s, act - t); //V1 (tmpmod[1] -> Y vector)
        fillModel(6 * s + 5, act - t); //V6
        fillModel(6 * s + 1, act + t);

        act += tmpmod * toCartesian(_transMap[s]); //Matricial product of modelview & orientation vector
        tmpmod = rotate(tmpmod, _transMap[s][1], vec3(0., 0., 1.));
        tmpmod = rotate(tmpmod, _transMap[s][2], vec3(0., 1., 0.));
        tmpmod = translate(tmpmod, vec3(_transMap[s][0], 0., 0.));

        t = tmpmod[1];
        t *= 0.1;
        fillModel(6 * s + 2, act + t);
        fillModel(6 * s + 3, act + t);
        fillModel(6 * s + 4, act - t);
        }

    cout << " OK" << endl;
    }

void Game::fillModel(int vertex, vec4 v) {
    _mapModel[vertex * 3] = v[0];
    _mapModel[vertex * 3 + 1] = v[1];
    _mapModel[vertex * 3 + 2] = v[2];
    }

vec4 Game::toCartesian(vec3 v) {
    vec4 res(0.);
    res[0] = v[0] * cos(v[1]) * sin(v[2] + PI / 2);
    res[1] = v[0] * sin(v[1]) * sin(v[2] + PI / 2);
    res[2] = v[0] * cos(v[2] + PI / 2);
    return res;
    }

void Game::drawMap(Video &video) {
    video.render(_mapModel, _mapColors, _modelSize / 3, lookAt(vec3(-0.5, 0, 0.3), vec3(1, 0, 0), vec3(0, 0, 1)));
    }

void Game::forward(float deltaX) {
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
