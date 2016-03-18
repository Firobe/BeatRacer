#include "notehandler.h"
#include "keymanager.h"
#include "map.h"
#include "lifebar.h"
#include "ship.h"
#include <stdexcept>
#include <fstream>

using namespace std;

NoteHandler::NoteHandler(string path, Map& map, LifeBar& lifebar, Ship& ship) : _currentNote(4, 0), _notes(4), _lifebar(lifebar), _ship(ship){
	string buffer;
	ifstream input;
	float pos;
	int col;
	_barPosition = 0.;
	_combo = 0;
	_score = 0;
	_precision = 100.;

	_model.load("note");
	_model.setShaderNb(2);
	_model.addUniform("state", 1);
	_timingBar.load("timingbar");
	path = "res/map/" + path + ".nt";
	input.open(path);
	if(!input)
		throw runtime_error("Unable to open " + path);

	Note def = {.matrix = glm::mat4(1.), .pos = 0., .state = 0};
	while (getline(input, buffer)) {
		sscanf(buffer.c_str(), "%f,%d", &pos, &col);
		for (int i = 0; i < 4; i++) {
			if (col % 2) {
				_notes[i].push_back(def);
				_notes[i].back().pos = pos;
				_notes[i].back().matrix = glm::translate(_notes[i].back().matrix, map.getWorldCoordinates(glm::vec3(pos, (ROAD_WIDTH * (2.*i - 3.)) / 4., 0)));
				glm::mat3 axes = map.getWorldOrientation(pos);
				_notes[i].back().matrix[0] = glm::vec4(axes[0], 0.);
				_notes[i].back().matrix[1] = glm::vec4(axes[1], 0.);
				_notes[i].back().matrix[2] = glm::vec4(axes[2], 0.);
			}
			col /= 2;
		}
	}
}

NoteHandler::~NoteHandler() {
}

void NoteHandler::draw(Video& vid) {
	for (unsigned int i = 0; i < 4 ; i++)
		for(unsigned int j = 0 ; j < _notes[i].size() ; j++){
			_model.setUniform("state", _notes[i][j].state);
			_model.draw(vid, _notes[i][j].matrix);
		}

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
	bool touches2[4];
	int diff = 0;
    int quality = 0;
    int noteIndex = 1;
	for(int i = 0 ; i < 4 ; i++) touches2[i] = KeyManager::check(touches[i], true);
	for (int col = 0; col < 4; col++){
		if(_currentNote[col] >= _notes[col].size())
			continue;
		if (_barPosition - _notes[col][_currentNote[col]].pos < -NOTE_ERROR_MAX)
			continue;
		else{
			if(_barPosition - _notes[col][_currentNote[col]].pos > NOTE_ERROR_MAX){
				_notes[col][_currentNote[col]].state = NOTE_FAILURE;
				_currentNote[col]++;
				_combo = 0;
				diff -= 5;
                if(_lifebar.getValue() < 30){
                    std::uniform_real_distribution<> rot (-(30 - _lifebar.getValue()), 30 - _lifebar.getValue());
                    _ship.turn(30);
                }
			}
			else{
				if(touches2[col]){
                    quality = 0;
                    while (abs(_barPosition - _notes[col][_currentNote[col]].pos) < NOTE_DELTA * (1-quality/4))
                        quality++;
                    noteIndex = 1;
                    for(int i = 0; i < 4; i++)
                            noteIndex += _currentNote[i];
                    _precision =+ ((quality * 0.25) - _precision) / noteIndex;
                    if (quality > 0){
                        _notes[col][_currentNote[col]].state = NOTE_SUCESS;
                        _combo++;
                        diff += 1;
                        _score += (_combo + 1) * quality * SCORE_REFERENCE;
                    }
                    else{
                        _notes[col][_currentNote[col]].state = NOTE_FAILURE;
                        diff -= 5;
                        _combo = 0;
                    }
                if(_notes[col][_currentNote[col]].state == NOTE_FAILURE && _lifebar.getValue() < 30){
                    std::uniform_real_distribution<> rot (-(30 - _lifebar.getValue()), 30 - _lifebar.getValue());
                    _ship.turn(30);
                }
					_currentNote[col]++;
				}
			}
		}
	}
	_lifebar.setValue( _lifebar.getValue() + diff );
}
