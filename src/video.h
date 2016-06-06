#ifndef VIDEO__H
#define VIDEO__H

#ifdef _WIN32
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
#include "../libs/glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "texture.h"
#include "keymanager.h"

//Projection parameters
#define NEAR 0.1
#define FAR 10000.
#define FOV 70.

#define PI 3.14159265358979

#define VERTICAL_SYNC 0
#define FPS_GOAL 60

#define SENSITIVITY ((float)0.1) //Mouse sensitivity

#define SHIP_CAMERA_BEHIND 3.
#define SHIP_CAMERA_HEIGHT (1.7)
#define SHIP_CAMERA_GROUNDPOINT 5.


#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))
#endif //!BUFFER_OFFSET

#if VERTICAL_SYNC == 1
#define FPS_GOAL 60
#endif //VERTICAL_SYNC == 1
#define FPS_INTERVAL (1./FPS_GOAL) //IN MICROSECONDS
#define FPS_TICKS (CLOCKS_PER_SEC * FPS_INTERVAL)

glm::vec3 toCartesian(glm::vec3 v);
glm::vec3 toSpherical(glm::vec3 v);

class Map;
class Model3D;
class Model2D;

class Video {
    public:
        Video(int, int, std::string, std::string);
        ~Video();
        void refresh();
        GLFWwindow* win();
        void render(GLuint id, int, Texture&, Model3D*, glm::mat4 = glm::mat4(1.), int = 0);
        void render2D(GLuint id, int, Texture&, Model2D*, glm::mat4 = glm::mat4(1.), int = 0);
        void rotateCamera(int, float);
        void cameraTranslate(int, float);
        glm::vec2 getCursor();
        void switchFreeFly();
        void shipCamera(glm::vec3, glm::vec3, Map&);
		void addShader(std::string, std::string);
		void twRedirect();
		void dirCamera(glm::vec3, glm::vec3);
    private:
        void setCamera();
        void setCamera(glm::vec3, glm::vec3);
        bool _freeFly;
        GLFWwindow* _window;
		std::vector<Shader> _shaderArray;
        glm::mat4 _projection;
        glm::mat4 _view;
        glm::vec3 _orientationX, _orientationY, _orientationZ;
        glm::vec3 _position;
    };

#endif
