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

Video::Video(int width, int height, void* pointer, string a, string b) : _shader(a, b) {
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
    _shader.load();
    _projection = glm::perspective(FOV, (double) width / height, NEAR, FAR);
    glEnable(GL_DEPTH_TEST);
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

void Video::render(float* vertices, float* colors, int size, glm::mat4 modelview) {
    glUseProgram(_shader.getProgramID());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "modelview"), 1, GL_FALSE, glm::value_ptr(modelview));
    glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

    glDrawArrays(GL_TRIANGLES, 0, size);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    glUseProgram(0);
    }
