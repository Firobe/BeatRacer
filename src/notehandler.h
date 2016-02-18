#ifndef NOTE_H
#define NOTE_H

#include "model.h"
#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "map.h"

class NoteHandler {
    public:
        NoteHandler(std::string, Map&);
        ~NoteHandler();
        void draw(Video&);
    private:
        Model _model;
        std::vector<glm::mat4> _matrices;
        std::vector<glm::vec2> _notes; //Vector of (position, column)
    };


#endif //NOTE_H
