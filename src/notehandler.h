#ifndef NOTE_H
#define NOTE_H

#include "model3d.h"
#include <random>

#define FAIL_PENALTY true
#define NOTE_ERROR_MAX 1
#define NOTE_DELTA 0.8 //0.4
#define NOTE_FAILURE -1
#define NOTE_SUCESS 1
#define SCORE_REFERENCE 25

class Map;
class Video;
class LifeBar;
class Ship;

typedef struct Note Note;
struct Note {
    glm::dmat4 matrix;
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
		void reset();
        int getCombo() {
            return _combo;
            }
        double getScore() {
            return _score;
            }
		double getPrecision() {
			return _precision;
		}
    private:
        float _barPosition;
        std::vector<unsigned int> _currentNote;
        Model3D _model;
        std::vector<std::vector<Note>> _notes;
        Model3D _timingBar;

        double _score, _precision;
        int _combo;
        LifeBar& _lifebar;
        Ship& _ship;
    };


#endif //NOTE_H
