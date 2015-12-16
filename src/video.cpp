#include "video.h"

using namespace std;


static void error_callback(int error, const char* description) {
    cerr << description;
    }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    void* data = glfwGetWindowUserPointer(window);
    bool* keys = (bool*) data;

    if (key == GLFW_KEY_UP)
        keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);

    if (key == GLFW_KEY_DOWN)
        keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

Video::Video(int width, int height, void* pointer) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    _window = glfwCreateWindow(width, height, "BeatRacer", NULL, NULL);

    if (!_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
        }

    glfwSetWindowUserPointer(_window, pointer);
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(_window, key_callback);
    }

Video::~Video() {
    glfwDestroyWindow(_window);
    glfwTerminate();
    }

void Video::refresh() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
    }

GLFWwindow* Video::win() {
    return _window;
    }

void Video::Project3D(Video& video) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio;
    int width, height;
    glfwGetFramebufferSize(video.win(), &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glOrtho(-ratio, ratio, -1., 1., 1., -1.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    }

void Video::Project2D(Video& video) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetFramebufferSize(video.win(), &width, &height);
    glOrtho(0., width, 0., height, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    }
