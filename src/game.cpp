#include "game.h"

using namespace std;

Game::Game() {
    _currentSegment = 0;
    _segmentCursor = 0;
    _cameraEye = {0, 0, 0.05};
    _cameraVector = {1, 0, 0};
    }

Game::~Game() {
    }

void Game::loadMap(string path) {
    CoordSPH temp;
    string buffer;
    ifstream map;
    int todo;
    map.open(path.c_str());

    while (getline(map, buffer)) {
        sscanf(buffer.c_str(), "%f,%f,%f:%d", &temp.rho, &temp.theta, &temp.phi, &todo);

        for (int i = 0 ; i < todo ; i++)
            _map.push_back(temp);
        }

    map.close();
    }

void Game::drawMap() {
    glPushMatrix();

    for (unsigned int i = 0 ; i < _map.size() ; i++)
        Video::DrawSegment(_map[i]);

    glPopMatrix();
    }

void Game::placeCamera() {
    gluLookAt(_cameraEye.x, _cameraEye.y, _cameraEye.z,
              _cameraEye.x + _cameraVector.x, _cameraEye.y + _cameraVector.y,
              _cameraEye.z + _cameraVector.z, 0., 0., 1.);
    }

void Game::forward(float deltaX) {
    while (_segmentCursor + deltaX >= _map[_currentSegment].rho) {
        float diff =  _map[_currentSegment].rho - _segmentCursor;
        _cameraEye.x += diff * _cameraVector.x;
        _cameraEye.y += diff * _cameraVector.y;
        _cameraEye.z += diff * _cameraVector.z;
        deltaX -= diff;
        _segmentCursor = 0;
        _currentSegment++;

        //LOOP
        if (_currentSegment >= _map.size()) {
            _cameraEye = {0, 0, 0.05};
            _cameraVector = {1, 0, 0};
            _currentSegment = 0;
            break;
            }


        float angle = PI * (-_map[_currentSegment].phi) / 180;
        //ROTATION Y
        float temp = _cameraVector.x;
        _cameraVector.x = _cameraVector.x * cos(angle) + _cameraVector.z * sin(angle);
        _cameraVector.z = -temp * sin(angle) + _cameraVector.z * cos(angle);

        angle = PI * (_map[_currentSegment].theta) / 180;
        //ROTATION Z
        temp = _cameraVector.x;
        _cameraVector.x = _cameraVector.x * cos(angle) - _cameraVector.y * sin(angle);
        _cameraVector.y = temp * sin(angle) + _cameraVector.y * cos(angle);

        }

    _cameraEye.x += deltaX * _cameraVector.x;
    _cameraEye.y += deltaX * _cameraVector.y;
    _cameraEye.z += deltaX * _cameraVector.z;
    _segmentCursor += deltaX;
    //cout << "p1 " << _cameraEye.x << " ;p2 " << _cameraEye.y << " ;p3 " << _cameraEye.z << endl;
    //cout << "v1 " << _cameraVector.x << " ;v2 " << _cameraVector.y << " ;v3 " << _cameraVector.z << endl;
    }

void Game::drawCamera() {
    glColor3ub(255, 0, 0);
    glTranslatef(_cameraEye.x, _cameraEye.y, _cameraEye.z);
    glutSolidCube(0.05);
    glLineWidth(4);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(_cameraVector.x, _cameraVector.y, _cameraVector.z);
    glEnd();
    glTranslatef(_cameraVector.x, _cameraVector.y, _cameraVector.z);
    glutSolidCube(0.05);
    }
