#ifndef NOTE_H
#define NOTE_H

#include <stdexcept>
#include "model.h"
#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"
#include "../libs/glm/gtx/euler_angles.hpp"

#include "map.h"

#define NOTE_ERROR_MAX 0.2
#define NOTE_FAILURE -1
#define NOTE_SUCESS 1

typedef struct Note Note;
struct Note{
	glm::mat4 matrix;
	float pos;
	int state;
};

class NoteHandler {
    public:
        NoteHandler(std::string, Map&);
        ~NoteHandler();
        void draw(Video&);
        void placeBar(float, Map&);
        void checkNotes();
    private:
        float _barPosition;
        std::vector<unsigned int> _currentNote;
        std::vector<float> _currentScore;
        Model _model;
        std::vector<std::vector<Note>> _notes;
        Model _timingBar;
    };


#endif //NOTE_H
