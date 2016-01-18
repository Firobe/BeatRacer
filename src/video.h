#ifndef VIDEO__H
#define VIDEO__H

#ifdef WIN32
#define GLEW_BUILD GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE

#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtx/transform.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "texture.h"
#include <cstdlib>
#include <iostream>

#define NEAR 0.1
#define FAR 100.
#define FOV 70.

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif

class Video {
    public:
        Video(int, int, void*, std::string, std::string);
        ~Video();
        void refresh();
        GLFWwindow* win();
        void render(GLuint id, int, Texture&, glm::mat4 =
                        glm::lookAt(glm::vec3(-0.5, 0, 0.3), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1)));

    private:
        GLFWwindow* _window;
        Shader _shader;
        glm::mat4 _projection;
    };

#endif
