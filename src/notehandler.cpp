#include "notehandler.h"
#include "keymanager.h"
#include "map.h"
#include <stdexcept>
#include <fstream>

using namespace std;

NoteHandler::NoteHandler(string path, Map& map) : _currentNote(4, 0) , _currentScore(4, 0) , _notes(4) {
	string buffer;
	ifstream input;
	float pos;
	int col;
	_barPosition = 0.;

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
	for(int i = 0 ; i < 4 ; i++) touches2[i] = KeyManager::check(touches[i], true);
	for (int col = 0; col < 4; col++){
		if(_currentNote[col] >= _notes[col].size())
			continue;
		if (_barPosition - _notes[col][_currentNote[col]].pos < -NOTE_ERROR_MAX)
			_currentScore[col] = 0.;
		else{
			if(_barPosition - _notes[col][_currentNote[col]].pos > NOTE_ERROR_MAX){
				_currentScore[col] = -1.;
				_notes[col][_currentNote[col]].state = NOTE_FAILURE;
				_currentNote[col]++;
			}
			else
				if(touches2[col]){
					_currentScore[col] = 1;
					_notes[col][_currentNote[col]].state = NOTE_SUCESS;
					_currentNote[col]++;
				}
				else
					_currentScore[col] = 0;
		}
	}
}
