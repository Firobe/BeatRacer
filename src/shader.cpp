#include "shader.h"

Shader::Shader() : _vertexID(0), _fragmentID(0), _programID(0), _vertexSource(), _fragmentSource() {
    }

Shader::Shader(Shader const &shaderACopier) {
    _vertexSource = shaderACopier._vertexSource;
    _fragmentSource = shaderACopier._fragmentSource;
    load();
    }


Shader::Shader(std::string vertexSource, std::string fragmentSource) : _vertexID(0), _fragmentID(0), _programID(0),
    _vertexSource(vertexSource), _fragmentSource(fragmentSource) {
    }


Shader::~Shader() {
    glDeleteShader(_vertexID);
    glDeleteShader(_fragmentID);
    glDeleteProgram(_programID);
    }

Shader& Shader::operator=(Shader const &shaderACopier) {
    _vertexSource = shaderACopier._vertexSource;
    _fragmentSource = shaderACopier._fragmentSource;
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

    if (!compilerShader(_vertexID, GL_VERTEX_SHADER, _vertexSource))
        return false;

    if (!compilerShader(_fragmentID, GL_FRAGMENT_SHADER, _fragmentSource))
        return false;

    _programID = glCreateProgram();
    glAttachShader(_programID, _vertexID);
    glAttachShader(_programID, _fragmentID);
    glBindAttribLocation(_programID, 0, "in_Vertex");
    glBindAttribLocation(_programID, 1, "in_Color");
    glBindAttribLocation(_programID, 2, "in_TexCoord0");
    glLinkProgram(_programID);
    GLint erreurLink(0);
    glGetProgramiv(_programID, GL_LINK_STATUS, &erreurLink);

    if (erreurLink != GL_TRUE) {
        GLint tailleErreur(0);
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &tailleErreur);
        char *erreur = new char[tailleErreur + 1];
        glGetShaderInfoLog(_programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';
        std::cout << erreur << std::endl;
        delete[] erreur;
        glDeleteProgram(_programID);
        return false;
        }
    else
        return true;
    }


bool Shader::compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource) {
    shader = glCreateShader(type);

    if (shader == 0) {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
        }

    std::ifstream fichier(fichierSource.c_str());

    if (!fichier) {
        std::cout << "Erreur le fichier " << fichierSource << " est introuvable" << std::endl;
        glDeleteShader(shader);
        return false;
        }

    std::string ligne;
    std::string codeSource;

    while (getline(fichier, ligne))
        codeSource += ligne + '\n';

    fichier.close();
    const GLchar* chaineCodeSource = codeSource.c_str();
    glShaderSource(shader, 1, &chaineCodeSource, 0);
    glCompileShader(shader);
    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);

    if (erreurCompilation != GL_TRUE) {
        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);
        char *erreur = new char[tailleErreur + 1];
        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';
        std::cout << erreur << std::endl;
        delete[] erreur;
        glDeleteShader(shader);
        return false;
        }
    else
        return true;
    }

GLuint Shader::getProgramID() const {
    return _programID;
    }
