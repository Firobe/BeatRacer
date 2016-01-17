#include "game.h"

using namespace std;
using namespace glm;

Game::Game() : _texture("res/road-tex.bmp") {
    _currentSegment = 0;
    _segmentCursor = 0;
    _vboID = 0;
    _vaoID = 0;
    _texture.load();
    }

Game::~Game() {
    glDeleteBuffers(1, &_vboID);
    glDeleteVertexArrays(1, &_vaoID);
    delete[] _mapModel;
    delete[] _mapTex;
    }

void Game::loadV() {
    //VBO
    if (glIsBuffer(_vboID) == GL_TRUE)
        glDeleteBuffers(1, &_vboID);

    int size1 = _modelSize * sizeof(float);
    int size2 = _transMap.size() * 2 * 6 * sizeof(float);
    glGenBuffers(1, &_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glBufferData(GL_ARRAY_BUFFER, size1 + size2, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size1, _mapModel);
    glBufferSubData(GL_ARRAY_BUFFER, size1, size2, _mapTex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //VAO
    if (glIsVertexArray(_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &_vaoID);

    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size1));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    }

void Game::loadMap(string path) {
    cout << "Processing map... ";
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
    _mapTex = new float[2 * 6 * _transMap.size()];

    mat4 tmpmod = mat4(1.);
    float cursor = 0;
    vec4 act(0.); //Current origin

    for (int s = 0 ; s < (int)_transMap.size() ; s++) { //SEGMENT
        fillTex(s, cursor);
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
        cursor += _transMap[s][0];
        }

    cout << "OK" << endl;
    loadV();
    }

void Game::fillModel(int vertex, vec4 v) {
    _mapModel[vertex * 3] = v[0];
    _mapModel[vertex * 3 + 1] = v[1];
    _mapModel[vertex * 3 + 2] = v[2];
    }

void Game::fillTex(int segment, float cursor) {
    int cur = 12 * segment;
    float rho = _transMap[segment][0];
    _mapTex[cur] = 0.;
    _mapTex[cur + 1] = cursor;
    _mapTex[cur + 2] = 1.;
    _mapTex[cur + 3] = cursor;
    _mapTex[cur + 4] = 1.;
    _mapTex[cur + 5] = cursor + rho;
    _mapTex[cur + 6] = 1.;
    _mapTex[cur + 7] = cursor + rho;
    _mapTex[cur + 8] = 0.;
    _mapTex[cur + 9] = cursor + rho;
    _mapTex[cur + 10] = 0.;
    _mapTex[cur + 11] = cursor;
    }

vec4 Game::toCartesian(vec3 v) {
    vec4 res(0.);
    res[0] = v[0] * cos(v[1]) * sin(v[2] + PI / 2);
    res[1] = v[0] * sin(v[1]) * sin(v[2] + PI / 2);
    res[2] = v[0] * cos(v[2] + PI / 2);
    return res;
    }

void Game::drawMap(Video & video) {
    video.render(_vaoID, _modelSize / 3, _texture,
                 lookAt(vec3(-0.5, 0, 0.3), vec3(1, 0, 0), vec3(0, 0, 1)));
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
