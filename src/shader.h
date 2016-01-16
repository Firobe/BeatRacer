#ifndef DEF_SHADER
#define DEF_SHADER

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

class Shader {
    public:

        Shader();
        Shader(Shader const &shaderACopier);
        Shader(std::string vertexSource, std::string fragmentSource);
        ~Shader();

        Shader& operator=(Shader const &shaderACopier);

        bool charger();
        bool compilerShader(GLuint &shader, GLenum type, std::string const &fichierSource);
        GLuint getProgramID() const;


    private:

        GLuint m_vertexID;
        GLuint m_fragmentID;
        GLuint m_programID;

        std::string m_vertexSource;
        std::string m_fragmentSource;
    };

#endif
