#ifndef VIDEO__H
#define VIDEO__H

#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <cstdlib>
#include <iostream>

#define NEAR 0.1
#define FAR 100.
#define FOV 70.

typedef struct CoordSPH CoordSPH;
struct CoordSPH {
    float rho, theta, phi;
    };

class Video {
    public:
        Video(int, int, void*, std::string, std::string);
        ~Video();
        void refresh();
        GLFWwindow* win();
        void render(float*, float*, int, glm::mat4 = glm::mat4(1.));

    private:
        GLFWwindow* _window;
        Shader _shader;
        glm::mat4 _projection;
    };

#endif
