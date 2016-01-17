#ifndef TEXTURE__H
#define TEXTURE__H

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
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
