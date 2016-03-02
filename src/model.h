#ifndef MODEL__H
#define MODEL__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/ext.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"

#include "texture.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <new>
#include <stdexcept>

class Video;

typedef struct UniformValue UniformValue;

struct UniformValue{
	int size;
	std::string name;
};

class Model {
    public:
        Model();
        ~Model();
        void load(std::string); //Load texture and model
        void draw(Video&); //Render the model
        void draw(Video&, glm::mat4&); //Render the model
        void translate(glm::vec3); //Translate the model by (x, y, z)
        void rotate(float angle, glm::vec3 axis); //Rotate the model by angle around axis
        void scale(glm::vec3); //Scale the model by (x, y, z)
        void setOrientation(glm::mat3);
        void resetMatrix();
        glm::mat4 getMatrix();
		void uniformize(int);
		void setShaderNb(int);
    protected:
        void loadTexture(std::string);
        virtual void loadModel(std::string);
        void loadV();

        Texture _texture;
        glm::mat4 _modelMatrix;
        float* _mapModel; //Vertices (te be rendered as triangles)
        float* _mapTex; //Texture coordinates
        GLuint _vboID, _vaoID;
        int _vertexNb;
        bool _textured;

		int _shaderNb;
		float* _uniform;
		std::vector<UniformValue> _uniformStructure;
    };


#include "video.h"
#endif
