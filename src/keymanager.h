#ifndef KEYMANAGER__H
#define KEYMANAGER__H

#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"

#ifdef _WIN32
#define GLEW_BUILD GLEW_STATIC
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE

#else
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif

#include <GLFW/glfw3.h>

#include <vector>
#include <chrono>

#define KEY_NUMBER 400

typedef std::chrono::high_resolution_clock Clock;

typedef struct Key Key;
struct Key {
    bool pressed;
    bool pressedSinceLastCheck;
    Clock::time_point lastGetTime;
    };

class KeyManager {
    public:
        static KeyManager& get();
        static void keyCallback(GLFWwindow*, int, int, int, int);
        static void cursorPosCallback(GLFWwindow*, double, double);
        static void mouseButtonCallback(GLFWwindow*, int, int, int);
        static bool mouseCheck(int button);
        static glm::dvec2 mousePosition();
        static bool check(int key, bool changed = false, float interval = 0.);
        /*Checks if 'key' is pressed (by default). If 'changed' is specified, checks if 'key' has been released then pressed again by the
        user since the last successful check. If 'interval' is specidied, it additionally checks if 'interval' seconds or more ellapsed
        since the last successful check.*/
        ~KeyManager();
    protected:
        KeyManager();
        void press(int);
        void release(int);
        bool instanceCheck(int, bool, float);
        std::vector<Key> _keys;
        std::vector<bool> _mouseButtons;
        glm::dvec2 _cursorPos;
    };

#endif
