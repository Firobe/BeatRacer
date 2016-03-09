#ifndef SHADER__H
#define SHADER__H

#ifdef _WIN32
#define GLEW_BUILD GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE

#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>
#include <string>

class Shader {
    public:
        Shader();
        Shader(Shader const& toCp);
        Shader(std::string srcVert, std::string srcFrag);
        ~Shader();
        Shader& operator=(Shader const& toCp);
        void load();
        void buildShader(GLuint& shader, GLenum type, std::string const& src);
        GLuint getProgramID() const;
    private:
        GLuint _vertexID;
        GLuint _fragmentID;
        GLuint _programID;
        std::string _srcVert;
        std::string _srcFrag;
    };

#endif
