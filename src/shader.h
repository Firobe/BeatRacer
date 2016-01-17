#ifndef SHADER__H
#define SHADER__H

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

class Shader {
    public:

        Shader();
        Shader(Shader const &toCp);
        Shader(std::string srcVert, std::string srcFrag);
        ~Shader();

        Shader& operator=(Shader const &toCp);

        bool load();
        bool buildShader(GLuint &shader, GLenum type, std::string const &src);
        GLuint getProgramID() const;


    private:

        GLuint _vertexID;
        GLuint _fragmentID;
        GLuint _programID;

        std::string _srcVert;
        std::string _srcFrag;
    };

#endif
