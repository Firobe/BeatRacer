#include "model.h"

using namespace std;
using namespace glm;

Model::Model() : _modelMatrix(1.) {
    _vboID = 0;
    _vaoID = 0;
    _textured = true;
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
    _texture.setPath("res/tex/" + path + ".png");

    if (!_texture.load()) {
        cout << "Can't open " << path << " texture file, falling back to default texture" << endl;
        _textured = false;
        _texture.setPath("res/tex/default.png");

        if (!_texture.load()) {
            cout << "!! Cannot open default texture : halting !!" << endl;
            exit(EXIT_FAILURE);
            }
        }
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
    vector<vec3> vert;
    vector<vec2> tex;
    vector<unsigned int> vInd, tInd;
    path = "res/obj/" + path + ".obj";
    FILE* file = fopen(path.c_str(), "r");

    if (file == NULL) {
        cout << "!! Can't open " << path << " : halting !!" << endl;
        exit(EXIT_FAILURE);
        }

    if (!_textured) {
        tex.push_back(vec2(0., 0.));
        tex.push_back(vec2(1., 0.));
        tex.push_back(vec2(1., 1.));
        }

    //Reading file
    for (;;) {
        char lineHeader[128];

        if (fscanf(file, "%s", lineHeader) == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            vec3 temp;
            fscanf(file, "%f %f %f\n", &temp.x, &temp.y, &temp.z);
            vert.push_back(temp);
            }
        else if (strcmp(lineHeader, "vt") == 0) {
            vec2 temp;
            fscanf(file, "%f %f\n", &temp.x, &temp.y);
            tex.push_back(temp);
            }
        else if (strcmp(lineHeader, "f") == 0) {
            unsigned int v[3], t[3];

            if (_textured) {
                if (fscanf(file, "%d/%d %d/%d %d/%d\n", &v[0], &t[0], &v[1], &t[1],
                           &v[2], &t[2]) != 6) {
                    cout << "!! Obj format not supported for " << path << " : halting !!" << endl;
                    exit(EXIT_FAILURE);
                    }

                vInd.push_back(v[0]);
                vInd.push_back(v[1]);
                vInd.push_back(v[2]);
                tInd.push_back(t[0]);
                tInd.push_back(t[1]);
                tInd.push_back(t[2]);
                }
            else {
                unsigned int v[3];

                if (fscanf(file, "%d %d %d\n", &v[0], &v[1],
                           &v[2]) != 3) {
                    cout << "!! Map format not supported : halting !!" << endl;
                    exit(EXIT_FAILURE);
                    }

                vInd.push_back(v[0]);
                vInd.push_back(v[1]);
                vInd.push_back(v[2]);
                tInd.push_back(1);
                tInd.push_back(2);
                tInd.push_back(3);

                }
            }
        }

    fclose(file);
    _vertexNb = vInd.size();
    _mapModel = new(std::nothrow) float[_vertexNb * 3];
    _mapTex = new(std::nothrow) float[_vertexNb * 2];

    if (_mapModel == NULL || _mapTex == NULL) {
        cout << "!! Could not allocate model memory : halting !!" << endl;
        exit(EXIT_FAILURE);
        }

    //Processing data
    for (unsigned int i = 0 ; i < vInd.size() ; i++) {
        _mapModel[i * 3] = vert[vInd[i] - 1].x;
        _mapModel[i * 3 + 1] = vert[vInd[i] - 1].y;
        _mapModel[i * 3 + 2] = vert[vInd[i] - 1].z;
        _mapTex[i * 2] = tex[tInd[i] - 1].x;
        _mapTex[i * 2 + 1] = tex[tInd[i] - 1].y;
        }

    }

void Model::draw(Video& video) {
    video.render(_vaoID, _vertexNb, _texture, _modelMatrix);
    }

void Model::draw(Video& video, glm::mat4& modelMatrix) {
    video.render(_vaoID, _vertexNb, _texture, modelMatrix);
    }

void Model::translate(vec3 tr) {
    _modelMatrix = glm::translate(_modelMatrix, tr);
    }

void Model::rotate(float angle, vec3 axis) {
    _modelMatrix = glm::rotate(_modelMatrix, angle, axis);
    }

void Model::scale(vec3 sc) {
    _modelMatrix = glm::scale(_modelMatrix, sc);
    }
