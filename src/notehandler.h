#ifndef NOTE_H
#define NOTE_H

#include "model3d.h"

#define NOTE_ERROR_MAX 0.2
#define NOTE_FAILURE -1
#define NOTE_SUCESS 1

class Map;
class Video;
class LifeBar;
class Ship;

typedef struct Note Note;
struct Note{
	glm::mat4 matrix;
	float pos;
	int state;
};

class NoteHandler {
    public:
        NoteHandler(std::string, Map&, LifeBar&, Ship&);
        ~NoteHandler();
        void draw(Video&);
        void placeBar(float, Map&);
        void checkNotes();
    private:
        float _barPosition;
        std::vector<unsigned int> _currentNote;
        std::vector<float> _currentScore;
        Model3D _model;
        std::vector<std::vector<Note>> _notes;
        Model3D _timingBar;

		float _score, _precision;
		int _combo;
		LifeBar& _lifebar;
		Ship& _ship;
    };


#endif //NOTE_H
