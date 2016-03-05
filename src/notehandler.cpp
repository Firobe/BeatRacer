#include "notehandler.h"
#include "keymanager.h"

using namespace std;

NoteHandler::NoteHandler(string path, Map& map) : _currentNote(4) , _currentScore(4) , _notes(4) {
    string buffer;
    ifstream input;
    float pos;
    int col;

    _barPosition = 0.;
    for (int i = 0; i < 4; i++){
        _currentNote[i] = 0;
        _currentScore[i] = 0;
        }

    _model.load("note");
    _timingBar.load("timingbar");
    path = "res/map/" + path + ".nt";
    input.open(path);
	if(!input)
		throw runtime_error("Unable to open " + path);

    while (getline(input, buffer)) {
        sscanf(buffer.c_str(), "%f,%d", &pos, &col);
        for (int i = 0; i < 4; i++) {
            if (col % 2) {
                _matrices.push_back(glm::mat4(1.));
                _notes[i].push_back(pos);
                _matrices.back() = glm::translate(_matrices.back(), map.getWorldCoordinates(glm::vec3(pos, (ROAD_WIDTH * (2.*i - 3.)) / 4., 0)));
                glm::mat3 axes = map.getWorldOrientation(pos);
                _matrices.back()[0] = glm::vec4(axes[0], 0.);
                _matrices.back()[1] = glm::vec4(axes[1], 0.);
                _matrices.back()[2] = glm::vec4(axes[2], 0.);
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

void NoteHandler::placeBar(float pos, Map& map) {
    _timingBar.resetMatrix();
    _timingBar.translate(map.getWorldCoordinates(glm::vec3(pos, 0, 0)));
    _timingBar.setOrientation(map.getWorldOrientation(pos));
    _barPosition = pos;
    }

void NoteHandler::checkNotes(){
    int touches[4] = {GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_J};
    for (int col = 0; col < 4; col++){
        if (_barPosition - _notes[col][_currentNote[col]] < -NOTE_ERROR_MAX)
            _currentScore[col] = 0.;
        else{
            if(_barPosition - _notes[col][_currentNote[col]] > NOTE_ERROR_MAX){
                _currentScore[col] = -1.;
                _currentNote[col]++;
            }
            else{
                if(KeyManager::check(touches[col], true)){
                    _currentScore[col] = 1;
                    _currentNote[col]++;
                    cout << "YOUPI" << endl;
                    }
                else{
                    _currentScore[col] = 0;
                }
            }
        }

    }
}
