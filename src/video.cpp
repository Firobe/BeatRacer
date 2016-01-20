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

    if (key == GLFW_KEY_UNKNOWN)
        return;

    keys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

Video::Video(int width, int height, void* pointer, string a, string b) : _shader(a, b) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 4); //Anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    _window = glfwCreateWindow(width, height, "BeatRacer", NULL, NULL);

    if (!_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
        }

    glfwSetWindowUserPointer(_window, pointer);
    glfwMakeContextCurrent(_window);
#ifdef WIN32
    glewExperimental = GL_TRUE;
    GLenum init(glewInit());

    if (init != GLEW_OK)
        exit(EXIT_FAILURE);

#endif
    glfwSwapInterval(1);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    _shader.load();
    _projection = glm::perspective(FOV, (double) width / height, NEAR, FAR);
    _position = glm::vec3(-5, 0, 0.1);
    _orientation = glm::vec3(1, 0, 0);
    setCamera();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
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

void Video::setCamera() {
    _modelView = glm::lookAt(_position, _position + toCartesian(_orientation), glm::vec3(0, 0, 1));
    }

void Video::rotateCamera(int axis, float value) {
    value = glm::radians(value);

    if (axis == yAxis)
        _orientation += glm::vec3(0, 0, value);

    if (axis == zAxis)
        _orientation += glm::vec3(0, value, 0);

    if (_orientation[2] >= PI / 2)
        _orientation[2] = PI / 2 - 0.01;
    else if (_orientation[2] <= -PI / 2)
        _orientation[2] = -PI / 2 + 0.01;

    setCamera();
    }

void Video::cameraForward(float value) {
    _position += value * toCartesian(_orientation);
    setCamera();
    }

void Video::render(GLuint id, int size, Texture & tex) {
    glUseProgram(_shader.getProgramID());
    glm::mat4 modViewProj = _projection * _modelView;

    glUniformMatrix4fv(glGetUniformLocation(_shader.getProgramID(), "modViewProj"), 1, GL_FALSE, glm::value_ptr(modViewProj));

    glBindVertexArray(id);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, tex.getID());

    glDrawArrays(GL_TRIANGLES, 0, size);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    }

glm::vec3 toCartesian(glm::vec3 v) {
    glm::vec3 res(0.);
    res[0] = v[0] * cos(v[1]) * sin(v[2] + PI / 2);
    res[1] = v[0] * sin(v[1]) * sin(v[2] + PI / 2);
    res[2] = v[0] * cos(v[2] + PI / 2);
    return res;
    }

glm::vec2 Video::getCursor() {
    double x, y;
    glfwGetCursorPos(_window, &x, &y);
    static double xO = x, yO = y;
    glm::vec2 res = SENSITIVITY * glm::vec2(xO - x, y - yO);
    xO = x;
    yO = y;
    return res;
    }
