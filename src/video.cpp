#include "video.h"
#include "map.h"
#include "model3d.h"
#include "model2d.h"

#define GLFW_CDECL
#include <AntTweakBar.h>
#include <stdexcept>
#include <iostream>
#include <sstream>

int screen_width = 1280;
int screen_height = 800;

using namespace std;
enum Axes { xAxis, yAxis, zAxis };

static void error_callback(int error, const char* description) {
    cerr << description << endl;
    }

inline void TwEventMouseButtonGLFW3(GLFWwindow* window, int button, int action, int mods) {
    TwEventMouseButtonGLFW(button, action);
    }
inline void TwEventMousePosGLFW3(GLFWwindow* window, double xpos, double ypos) {
    TwMouseMotion(int(xpos), int(ypos));
    }
inline void TwEventMouseWheelGLFW3(GLFWwindow* window, double xoffset, double yoffset) {
    TwEventMouseWheelGLFW(yoffset);
    }
inline void TwEventKeyGLFW3(GLFWwindow* window, int key, int scancode, int action, int mods) {
    TwEventKeyGLFW(key, action);
    }
inline void TwEventCharGLFW3(GLFWwindow* window, unsigned int codepoint) {
    TwEventCharGLFW(codepoint, GLFW_PRESS);
    }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    TwEventKeyGLFW(key, action);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (key == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS)
        KeyManager::get().press(key);

    if (action == GLFW_RELEASE)
        KeyManager::get().release(key);
    }

void Video::twRedirect() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(_window, TwEventMouseButtonGLFW3);
    glfwSetCursorPosCallback(_window, TwEventMousePosGLFW3);
    glfwSetScrollCallback(_window, TwEventMouseWheelGLFW3);
    glfwSetCharCallback(_window, TwEventCharGLFW3);
    }

Video::Video(string a, string b) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        throw runtime_error("Unable to initialize GLFW");

    glfwWindowHint(GLFW_SAMPLES, 4); //Anti-aliasing

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (FULLSCREEN) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		screen_width = mode->width;
		screen_height = mode->height;
        _window = glfwCreateWindow(screen_width, screen_height, "BeatRacer", glfwGetPrimaryMonitor(), NULL);
        }
    else
        _window = glfwCreateWindow(screen_width, screen_height, "BeatRacer", NULL, NULL);

    if (!_window)
        throw runtime_error("Unable to create window");

    glfwMakeContextCurrent(_window);
#ifdef _WIN32
    glewExperimental = GL_TRUE;
    GLenum init(glewInit());

    if (init != GLEW_OK)
        throw ("Unable to initialize GLEW");

#endif
    glfwSwapInterval(VERTICAL_SYNC);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    _shaderArray.push_back(Shader(a, b));
    _shaderArray[0].load();
    _projection = glm::perspective(FOV, (double) screen_width / screen_height, NEAR, FAR);
    _position = glm::vec3(-5, 0, 0.1);
    _orientationX = glm::vec3(1, 0, 0);
    _orientationY = glm::vec3(1, glm::half_pi<float>(), 0);
    _orientationZ = glm::vec3(1, 0, -glm::half_pi<float>());
    _freeFly = false;
    setCamera();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

Video::~Video() {
    glfwDestroyWindow(_window);
    glfwTerminate();
    }

void Video::refresh() {
    GLenum err;

    if ((err = glGetError()) != GL_NO_ERROR) {
        stringstream ss;
        ss << "0x" << std::hex << err;
        throw runtime_error("OpenGL issue " + ss.str());
        }

    glfwSwapBuffers(_window);
    glfwPollEvents();
    }

GLFWwindow* Video::win() {
    return _window;
    }

void Video::switchFreeFly() {
    _freeFly = !_freeFly;
    }

void Video::setCamera() {
    _view = glm::lookAt(_position, _position + toCartesian(_orientationX), glm::vec3(0, 0, 1));
    }

void Video::setCamera(glm::vec3 toLook, glm::vec3 vertical) {
    _view = glm::lookAt(_position, toLook, vertical);
    }

void Video::dirCamera(glm::vec3 dir, glm::vec3 vertical) {
    _view = glm::lookAt(_position, _position + dir, vertical);
    }

void Video::quatCamera(glm::quat quat) {
    setCamera();
    _view *= glm::toMat4(quat);
    }

void Video::shipCamera(glm::vec3 shipPos, glm::vec3 vertical, Map& map) {
    if (_freeFly)
        return;

    _position = map.getWorldCoordinates(glm::vec3(shipPos.x - SHIP_CAMERA_BEHIND, shipPos.y, SHIP_CAMERA_HEIGHT));
    glm::vec3 lookAt = map.getWorldCoordinates(glm::vec3(shipPos.x + SHIP_CAMERA_GROUNDPOINT, 0, shipPos.z));
    setCamera(lookAt, vertical);
    }

void Video::addShader(string a, string b) {
    _shaderArray.push_back(Shader(a, b));
    }

void Video::rotateCamera(int axis, float value, bool degree) {
    if (!_freeFly)
        return;

    if (degree)
        value = glm::radians(value);

    if (axis == yAxis) {
        _orientationX += glm::vec3(0, 0, value);
        _orientationZ += glm::vec3(0, 0, value);
        }

    if (axis == zAxis) {
        _orientationX += glm::vec3(0, value, 0);
        _orientationY += glm::vec3(0, value, 0);
        }

    if (_orientationX[2] >= glm::half_pi<float>())
        _orientationX[2] = glm::half_pi<float>() - 0.01;
    else if (_orientationX[2] <= - glm::half_pi<float>())
        _orientationX[2] = - glm::half_pi<float>() + 0.01;

    setCamera();
    }

void Video::cameraTranslate(int axis, float value) {
    if (!_freeFly)
        return;

    if (axis == xAxis)
        _position += value * toCartesian(_orientationX);

    if (axis == yAxis)
        _position += value * toCartesian(_orientationY);

    if (axis == zAxis)
        _position += value * toCartesian(_orientationZ);

    setCamera();
    }

void Video::render(GLuint id, int size, Texture& tex, Model3D* mod, glm::mat4 model, int shaderNb) {
    if ((unsigned int)shaderNb >= _shaderArray.size())
        throw runtime_error("Unknown shader");

    glUseProgram(_shaderArray[shaderNb].getProgramID());
    mod->uniformize(_shaderArray[shaderNb].getProgramID());
    glm::mat4 modViewProj = _projection * _view * model;
    glUniformMatrix4fv(glGetUniformLocation(_shaderArray[shaderNb].getProgramID(), "modViewProj"), 1, GL_FALSE, glm::value_ptr(modViewProj));
    glBindVertexArray(id);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, tex.getID());

    glDrawArrays(GL_TRIANGLES, 0, size);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    }

void Video::render2D(GLuint id, int size, Texture& tex, Model2D* mod, glm::mat4 model, int shaderNb) {
    if ((unsigned int)shaderNb >= _shaderArray.size())
        throw runtime_error("Unknown shader");

    glDisable(GL_DEPTH_TEST);
    glUseProgram(_shaderArray[shaderNb].getProgramID());
    mod->uniformize(_shaderArray[shaderNb].getProgramID());
    glm::mat4 modViewProj = model;
    glUniformMatrix4fv(glGetUniformLocation(_shaderArray[shaderNb].getProgramID(), "modViewProj"), 1, GL_FALSE, glm::value_ptr(modViewProj));
    glBindVertexArray(id);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, tex.getID());

    glDrawArrays(GL_TRIANGLES, 0, size);

    if (!tex.empty())
        glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    glEnable(GL_DEPTH_TEST);
    }

glm::vec3 toCartesian(glm::vec3 v) {
    glm::vec3 res(0.);
    res[0] = v[0] * cos(v[1]) * sin(v[2] + glm::half_pi<float>());
    res[1] = v[0] * sin(v[1]) * sin(v[2] + glm::half_pi<float>());
    res[2] = v[0] * cos(v[2] + glm::half_pi<float>());
    return res;
    }

glm::vec3 toSpherical(glm::vec3 v) {
    glm::vec3 res(0.);
    res[0] = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    res[1] = atan(v.y / v.x); //THETA
    res[2] = (acos(v.z / res[0]) - glm::half_pi<float>()); //PHI
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
