#include "game.h"

using namespace std;
extern int c;

Game::Game() {
    _currentSegment = 0;
    _segmentCursor = 0;
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
    c = (10 * _currentSegment) % 256;
    glPushMatrix();
    Video::DrawSegmentRev(_map[_currentSegment], _map[_currentSegment], _segmentCursor);

    for (int i = _currentSegment - 1 ; i >= 0 ; i--) {
        Video::DrawSegmentRev(_map[i + 1], _map[i]);
        }

    glPopMatrix();
    glPushMatrix();
    Video::DrawSegment(_map[_currentSegment], _segmentCursor);

    for (unsigned int i = _currentSegment + 1 ; i < _map.size() ; i++)
        Video::DrawSegment(_map[i]);

    glPopMatrix();
    }

void Game::placeCamera() {
    gluLookAt(0., 0., 0.1,
              1., 0., 0.1,
              0., 0., 1.);
    }

void Game::forward(float deltaX) {
    while (_segmentCursor + deltaX >= _map[_currentSegment].rho) {
        float diff =  _map[_currentSegment].rho - _segmentCursor;
        deltaX -= diff;
        _segmentCursor = 0;
        _currentSegment++;

        //LOOP
        if (_currentSegment >= _map.size()) {
            glLoadIdentity();
            placeCamera();
            _currentSegment = 0;
            break;
            }
        }

    _segmentCursor += deltaX;
    }
