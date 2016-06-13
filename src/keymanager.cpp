#include "keymanager.h"
#include <AntTweakBar.h>
#include <stdexcept>
#include <sstream>
#include <string>

using namespace std;

void KeyManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    TwEventKeyGLFW(key, action);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (key == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS)
        KeyManager::get().press(key);

    if (action == GLFW_RELEASE)
        KeyManager::get().release(key);
    }

void KeyManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
    TwMouseMotion(int(xpos), int(ypos));
	KeyManager::get()._cursorPos = glm::dvec2(xpos, ypos);
}

void KeyManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
    TwEventMouseButtonGLFW(button, action);
	KeyManager::get()._mouseButtons[button] = (action == GLFW_PRESS);
}

KeyManager::KeyManager() : _keys(KEY_NUMBER, (Key) {false, false, Clock::now()}), _mouseButtons(4, false) {
    }

KeyManager::~KeyManager() {
    }

KeyManager& KeyManager::get() {
    static KeyManager instance;
    return instance;
    }

void KeyManager::press(int key) {
    if (key >= KEY_NUMBER) {
        stringstream ss;
        ss << key;
        throw out_of_range("Unknown key -> " + ss.str());
        }

    _keys[key].pressed = true;
    _keys[key].pressedSinceLastCheck = true;
    }

void KeyManager::release(int key) {
    if (key >= KEY_NUMBER) {
        stringstream ss;
        ss << key;
        throw out_of_range("Unknown key -> " + ss.str());
        }

    _keys[key].pressed = false;
    }

bool KeyManager::instanceCheck(int key, bool changed, float interval) {
    if (key >= KEY_NUMBER) {
        stringstream ss;
        ss << key;
        throw out_of_range("Unknown key -> " + ss.str());
        }

    Clock::time_point current = Clock::now();
    float duration = chrono::duration_cast<chrono::duration<float>>(current - _keys[key].lastGetTime).count();

    if (duration < interval)
        return false;

    if ((changed && _keys[key].pressedSinceLastCheck) || (!changed && _keys[key].pressed)) {
        _keys[key].lastGetTime = current;
        _keys[key].pressedSinceLastCheck = false;
        return true;
        }
    else {
        _keys[key].pressedSinceLastCheck = false;
        return false;
        }
    }

bool KeyManager::check(int key, bool changed, float interval) {
    return get().instanceCheck(key, changed, interval);
    }

bool KeyManager::mouseCheck(int button){
	return get()._mouseButtons[button];
}

glm::dvec2 KeyManager::mousePosition(){
	return get()._cursorPos;
}
