#ifndef MODEL__H
#define MODEL__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/gtx/transform.hpp"

#include "texture.h"
#include <vector>
#include <string>

class Video;

typedef struct UniformValue UniformValue;

class UniformValue {
    public:
        UniformValue(int s, std::string n) : size(s), name(n) {}
        int size;
        std::string name;
    };

class Model { //ABSTRACT
    public:
        Model();
        ~Model();
        void load(std::string); //Load texture and model
        virtual void draw(Video&) = 0; //Render the model
        virtual void draw(Video&, glm::dmat4&) = 0; //Render the model
        void translate(glm::dvec3); //Translate the model by (x, y, z)
        void rotate(double angle, glm::dvec3 axis); //Rotate the model by angle around axis
        void scale(glm::dvec3); //Scale the model by (x, y, z)
        void setOrientation(glm::dmat3);
        void resetMatrix();
        glm::dmat4 getMatrix();
        void uniformize(int);
        void setShaderNb(int);
        void addUniform(std::string, int);
        void setUniform(std::string, float, int = 0);
        virtual void loadTexture(std::string);
    protected:
        virtual void loadModel(std::string) = 0;
        virtual void loadV() = 0;
        Texture _texture;
        glm::dmat4 _modelMatrix;
        std::vector<float> _mapModel; //Vertices (te be rendered as triangles)
        std::vector<float> _mapTex; //Texture coordinates
        GLuint _vboID, _vaoID;
        int _vertexNb;
        bool _textured;

        int _shaderNb;
        std::vector<float> _uniform;
        std::vector<UniformValue> _uniformStructure;
    };

#endif
