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
#include <new>

class Model {
    public:
        Model();
        ~Model();
        void load(std::string);
        void draw(Video&);
        void translate(glm::vec3);
        void rotate(float, glm::vec3);
        void scale(glm::vec3);
    protected:
        void loadTexture(std::string);
        virtual void loadModel(std::string);
        void loadV();

        Texture _texture;
        glm::mat4 _modelMatrix;
        float* _mapModel;
        float* _mapTex;
        GLuint _vboID, _vaoID;
        int _vertexNb;
        bool _textured;
    };


#endif
