#include "video.h"
#include "map.h"
#include "model3d.h"
#include "model2d.h"
#include "keymanager.h"

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
    cout << description << endl;
    }

void Video::twRedirect() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetScrollCallback(_window, [](GLFWwindow* w, double x, double y){
			TwEventMouseWheelGLFW(y);
			});
    glfwSetCharCallback(_window, [](GLFWwindow* w, unsigned int codepoint){
			TwEventCharGLFW(codepoint, GLFW_PRESS);
			});
    }

Video::Video(string a, string b) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        throw runtime_error("Unable to initialize GLFW");

	int min, maj, rev;
	glfwGetVersion(&maj, &min, &rev);
	cout << "GLFW version is " << maj << "." << min << "." << rev << endl;
    glfwWindowHint(GLFW_SAMPLES, 4); //Anti-aliasing

    if (FULLSCREEN) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		screen_width = mode->width;
		screen_height = mode->height;
        _window = glfwCreateWindow(screen_width, screen_height, "BeatRacer", glfwGetPrimaryMonitor(), nullptr);
        }
    else
        _window = glfwCreateWindow(screen_width, screen_height, "BeatRacer", nullptr, nullptr);

    if (!_window)
        throw runtime_error("Unable to create window");

    glfwMakeContextCurrent(_window);
#ifdef _WIN32
    cout << "Windows patch enabled !" << endl;
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
        throw ("Unable to initialize GLEW");

#endif
    glfwSwapInterval(VERTICAL_SYNC);
    glfwSetKeyCallback(_window, KeyManager::keyCallback);
	glfwSetCursorPosCallback(_window, KeyManager::cursorPosCallback);
	glfwSetMouseButtonCallback(_window, KeyManager::mouseButtonCallback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    _shaderArray.emplace_back(a, b);
    _shaderArray[0].load();
    _projection = glm::perspective(FOV, (double) screen_width / screen_height, NEAR, FAR);
    _position = glm::dvec3(-5, 0, 0.1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	cout << "OpenGL context version is " << (unsigned char*)glGetString(GL_VERSION) << endl;
    }

Video::~Video() {
    glfwDestroyWindow(_window);
    glfwTerminate();
    }

void Video::reset(){
    _position = glm::dvec3(-5, 0, 0.1);
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

void Video::quatCamera(glm::dquat quat) {
	_view = glm::lookAt(_position, _position + glm::rotate(quat, glm::dvec3(1., 0., 0.)), glm::rotate(quat, glm::dvec3(0., 0., 1.)));
    }

void Video::translateCamera(glm::dquat orientation, glm::dvec3 axis){
	_position += glm::rotate(orientation, axis);
}

void Video::shipCamera(glm::dvec3 shipPos, glm::dvec3 vertical, Map& map) {
    _position = map.getWorldCoordinates(glm::dvec3(shipPos.x - SHIP_CAMERA_BEHIND, shipPos.y, SHIP_CAMERA_HEIGHT));
    glm::dvec3 toLook = map.getWorldCoordinates(glm::dvec3(shipPos.x + SHIP_CAMERA_GROUNDPOINT, 0, shipPos.z));
    _view = glm::lookAt(_position, toLook, vertical);
    }

void Video::addShader(string a, string b) {
    _shaderArray.emplace_back(a, b);
    }

void Video::render(GLuint id, int size, Texture& tex, Model3D* mod, glm::dmat4 model, int shaderNb) {
    if ((unsigned int)shaderNb >= _shaderArray.size())
        throw runtime_error("Unknown shader");

    glUseProgram(_shaderArray[shaderNb].getProgramID());
    mod->uniformize(_shaderArray[shaderNb].getProgramID());
    glm::mat4 modViewProj(_projection * _view * model);
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

void Video::render2D(GLuint id, int size, Texture& tex, Model2D* mod, glm::dmat4 model, int shaderNb) {
    if ((unsigned int)shaderNb >= _shaderArray.size())
        throw runtime_error("Unknown shader");

    glDisable(GL_DEPTH_TEST);
    glUseProgram(_shaderArray[shaderNb].getProgramID());
    mod->uniformize(_shaderArray[shaderNb].getProgramID());
    glm::mat4 modViewProj(model);
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

glm::dvec3 toCartesian(glm::dvec3 v) {
    glm::dvec3 res(0.);
    res[0] = v[0] * cos(v[1]) * sin(v[2] + glm::half_pi<double>());
    res[1] = v[0] * sin(v[1]) * sin(v[2] + glm::half_pi<double>());
    res[2] = v[0] * cos(v[2] + glm::half_pi<double>());
    return res;
    }

glm::dvec3 toSpherical(glm::dvec3 v) {
    glm::dvec3 res(0.);
    res[0] = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    res[1] = atan(v.y / v.x); //THETA
    res[2] = (acos(v.z / res[0]) - glm::half_pi<double>()); //PHI
    return res;
    }
