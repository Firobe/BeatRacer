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

#define NOTE_ERROR_MAX 42.

class NoteHandler {
    public:
        NoteHandler(std::string, Map&);
        ~NoteHandler();
        void draw(Video&);
        void placeBar(float, Map&);
        void checkNotes();
    private:
        float _barPosition;
        std::vector<int> _currentNote;
        std::vector<float> _currentScore;
        Model _model;
        std::vector<glm::mat4> _matrices;
        std::vector<std::vector<float>> _notes; //Vector of (position, column)
        Model _timingBar;
    };


#endif //NOTE_H
