#ifndef MODEL__H
#define MODEL__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/ext.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "video.h"
#include "texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

class Model {
    public:
        Model();
        ~Model();
        void load(std::string);
        void draw(Video&);
    protected:
        void loadTexture(std::string);
        virtual void loadModel(std::string);
        void loadV();

        Texture _texture;
        float *_mapModel, *_mapTex;
        GLuint _vboID, _vaoID;
        int _vertexNb;
    };


#endif
