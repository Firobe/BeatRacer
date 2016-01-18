#ifndef TEXTURE__H
#define TEXTURE__H

#ifdef WIN32
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE

#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Texture {
    public:
        Texture();
        Texture(std::string);
        ~Texture();
        GLuint getID();
        bool empty();
        bool load();
        void setPath(std::string);
    private:
        std::string _path;
        GLuint _id;
        int _width, _height;
    };

#endif
