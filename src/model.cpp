#include "model.h"

using namespace std;
using namespace glm;

Model::Model() {
    _vboID = 0;
    _vaoID = 0;
    }

Model::~Model() {
    glDeleteBuffers(1, &_vboID);
    glDeleteVertexArrays(1, &_vaoID);
    delete[] _mapModel;
    delete[] _mapTex;
    }

void Model::load(string name) {
    loadTexture(name);
    loadModel(name);
    loadV();
    }

void Model::loadTexture(string path) {
    _texture.setPath(path + ".png");
    _texture.load();
    }

void Model::loadV() {
    //VBO
    if (glIsBuffer(_vboID) == GL_TRUE)
        glDeleteBuffers(1, &_vboID);

    int size1 = _vertexNb * 3 * sizeof(float);
    int size2 = _vertexNb * 2 * sizeof(float);
    glGenBuffers(1, &_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glBufferData(GL_ARRAY_BUFFER, size1 + size2, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size1, _mapModel);
    glBufferSubData(GL_ARRAY_BUFFER, size1, size2, _mapTex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //VAO
    if (glIsVertexArray(_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &_vaoID);

    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size1));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    }

void Model::loadModel(string path) {
    //FILLS _mapModel & _mapTex
    cout << "Loading Model..." << endl;
    }

void Model::draw(Video& video) {
    video.render(_vaoID, _vertexNb, _texture);
    }
