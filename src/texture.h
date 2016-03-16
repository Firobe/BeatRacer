#ifndef TEXTURE__H
#define TEXTURE__H

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

class Texture {
    public:
        Texture();
        Texture(std::string);
        ~Texture();
        GLuint getID();
        bool empty();
        virtual void load();
        void setPath(std::string);
    protected:
        std::string _path;
        GLuint _id;
        int _width, _height;
    };

#endif
