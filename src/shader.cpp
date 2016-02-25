#include "shader.h"

using namespace std;
Shader::Shader() : _vertexID(0), _fragmentID(0), _programID(0), _srcVert(), _srcFrag() {
    }

Shader::Shader(Shader const &toCp) {
    _srcVert = toCp._srcVert;
    _srcFrag = toCp._srcFrag;
    load();
    }


Shader::Shader(string srcVert, string srcFrag) : _vertexID(0), _fragmentID(0), _programID(0),
    _srcVert(srcVert), _srcFrag(srcFrag) {
    }


Shader::~Shader() {
    glDeleteShader(_vertexID);
    glDeleteShader(_fragmentID);
    glDeleteProgram(_programID);
    }

Shader& Shader::operator=(Shader const &toCp) {
    _srcVert = toCp._srcVert;
    _srcFrag = toCp._srcFrag;
    load();
    return *this;
    }

bool Shader::load() {
    if (glIsShader(_vertexID) == GL_TRUE)
        glDeleteShader(_vertexID);

    if (glIsShader(_fragmentID) == GL_TRUE)
        glDeleteShader(_fragmentID);

    if (glIsProgram(_programID) == GL_TRUE)
        glDeleteProgram(_programID);

    if (!buildShader(_vertexID, GL_VERTEX_SHADER, _srcVert))
        return false;

    if (!buildShader(_fragmentID, GL_FRAGMENT_SHADER, _srcFrag))
        return false;

    _programID = glCreateProgram();
    glAttachShader(_programID, _vertexID);
    glAttachShader(_programID, _fragmentID);
    glBindAttribLocation(_programID, 0, "in_Vertex");
    glBindAttribLocation(_programID, 1, "in_Color");
    glBindAttribLocation(_programID, 2, "in_TexCoord0");
    glLinkProgram(_programID);
    GLint linkError(0);
    glGetProgramiv(_programID, GL_LINK_STATUS, &linkError);

    if (linkError != GL_TRUE) {
        GLint errSize(0);
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &errSize);
        char *error = new char[errSize + 1];
        glGetShaderInfoLog(_programID, errSize, &errSize, error);
        error[errSize] = '\0';
        cout << error << endl;
        delete[] error;
        glDeleteProgram(_programID);
        return false;
        }
    else
        return true;
    }


bool Shader::buildShader(GLuint &shader, GLenum type, string const &src) {
    shader = glCreateShader(type);

    if (shader == 0) {
        cerr << "No (" << type << ") type shader." << endl;
        return false;
        }

    ifstream file(src.c_str());

    if (!file) {
        cerr << "Can't open " << src << endl;
        glDeleteShader(shader);
        return false;
        }

    string line;
    string srcC;

    while (getline(file, line))
        srcC += line + '\n';

    file.close();
    const GLchar* srcStr = srcC.c_str();
    glShaderSource(shader, 1, &srcStr, 0);
    glCompileShader(shader);
    GLint compError(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compError);

    if (compError != GL_TRUE) {
        GLint errSize(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errSize);
        char *error = new char[errSize + 1];
        glGetShaderInfoLog(shader, errSize, &errSize, error);
        error[errSize] = '\0';
        cout << error << endl;
        delete[] error;
        glDeleteShader(shader);
        return false;
        }
    else
        return true;
    }

GLuint Shader::getProgramID() const {
    return _programID;
    }
