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

void Shader::load() {
    if (glIsShader(_vertexID) == GL_TRUE)
        glDeleteShader(_vertexID);

    if (glIsShader(_fragmentID) == GL_TRUE)
        glDeleteShader(_fragmentID);

    if (glIsProgram(_programID) == GL_TRUE)
        glDeleteProgram(_programID);

    buildShader(_vertexID, GL_VERTEX_SHADER, _srcVert);
    buildShader(_fragmentID, GL_FRAGMENT_SHADER, _srcFrag);
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
		string err = "Shader linking failure\n--> " + string(error);
        delete[] error;
		throw runtime_error(err);
        }
    }


void Shader::buildShader(GLuint &shader, GLenum type, string const &src) {
    shader = glCreateShader(type);

    if (shader == 0)
		throw invalid_argument("Wrong shader type : " + to_string(type));

    ifstream file(src);

    if (!file)
		throw runtime_error("Unable to open " + src);

    string line;
    string srcC;

    while (getline(file, line))
        srcC += line + '\n';

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
		string err = to_string(type) + " type shader build failure\n--> " + string(error);
        delete[] error;
		throw runtime_error(err);
        }
    }

GLuint Shader::getProgramID() const {
    return _programID;
    }
