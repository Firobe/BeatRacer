#include "notehandler.h"

using namespace std;

NoteHandler::NoteHandler(string path, Map& map)  {
    string buffer;
    ifstream input;
    float pos;
    int col;
    _model.load("note");
    _timingBar.load("timingbar");
    path = "res/map/" + path + ".nt";
    input.open(path.c_str());

    if (!input.is_open()) {
        cout << "!! Can't open " << path << " : halting !!" << endl;
        exit(EXIT_FAILURE);
        }

    while (getline(input, buffer)) {
        sscanf(buffer.c_str(), "%f,%d", &pos, &col);

        for (int i = 0; i < 4; i++) {
            if (col % 2) {
                _matrices.push_back(glm::mat4(1.));
                _notes.push_back(glm::vec2(pos, i));
                _matrices.back() = glm::translate(_matrices.back(), map.getWorldCoordinates(glm::vec3(pos, (ROAD_WIDTH * (2.*i - 3.)) / 4., 0)));
                glm::vec2 angles = map.getWorldAngles(pos);
                _matrices.back() = glm::rotate(_matrices.back(), angles[1], glm::vec3(0., 1., 0.));
                _matrices.back() = glm::rotate(_matrices.back(), angles[0], glm::vec3(0., 0., 1.));
                }

            col /= 2;
            }
        }
    }

NoteHandler::~NoteHandler() {
    }

void NoteHandler::draw(Video& vid) {
    for (unsigned int i = 0; i < _notes.size() ; i++)
        _model.draw(vid, _matrices[i]);
    _timingBar.draw(vid);
    }

void NoteHandler::placeBar(float pos, Map& map){
    _timingBar.resetMatrix();
    _timingBar.translate(map.getWorldCoordinates(glm::vec3(pos,0,0)));
    glm::vec2 angles = map.getWorldAngles(pos);
    _timingBar.rotate(angles[1], glm::vec3(0., 1., 0.));
    _timingBar.rotate(angles[0], glm::vec3(0., 0., 1.));
}
