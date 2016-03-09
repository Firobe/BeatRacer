#ifndef MODEL__H
#define MODEL__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtx/transform.hpp"

#include "texture.h"
#include <cstdlib>
#include <vector>
#include <string>

class Video;

typedef struct UniformValue UniformValue;

struct UniformValue{
	int size;
	std::string name;
};

class Model { //ABSTRACT
    public:
        Model();
        ~Model();
        void load(std::string); //Load texture and model
        virtual void draw(Video&) = 0; //Render the model
        virtual void draw(Video&, glm::mat4&) = 0; //Render the model
        void translate(glm::vec3); //Translate the model by (x, y, z)
        void rotate(float angle, glm::vec3 axis); //Rotate the model by angle around axis
        void scale(glm::vec3); //Scale the model by (x, y, z)
        void setOrientation(glm::mat3);
        void resetMatrix();
        glm::mat4 getMatrix();
		void uniformize(int);
		void setShaderNb(int);
		void addUniform(std::string, int);
		void setUniform(std::string, float, int = 0);
    protected:
        void loadTexture(std::string);
        virtual void loadModel(std::string) = 0;
        virtual void loadV() = 0;

        Texture _texture;
        glm::mat4 _modelMatrix;
        float* _mapModel; //Vertices (te be rendered as triangles)
        float* _mapTex; //Texture coordinates
        GLuint _vboID, _vaoID;
        int _vertexNb;
        bool _textured;

		int _shaderNb;
		std::vector<float> _uniform;
		std::vector<UniformValue> _uniformStructure;
    };

#endif
