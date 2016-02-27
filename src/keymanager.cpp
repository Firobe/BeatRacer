#include "keymanager.h"

using namespace std;

KeyManager::KeyManager() : _keys(KEY_NUMBER, (Key) {
    false, false, Clock::now()
    }) {
    }

KeyManager::~KeyManager() {
    }

KeyManager& KeyManager::get() {
    static KeyManager instance;
    return instance;
    }

void KeyManager::press(int key) {
    if (key >= KEY_NUMBER)
        throw out_of_range("Unknown key -> " + to_string(key));

    _keys[key].pressed = true;
    _keys[key].pressedSinceLastCheck = true;
    }

void KeyManager::release(int key) {
    if (key >= KEY_NUMBER)
        throw out_of_range("Unknown key -> " + to_string(key));

    _keys[key].pressed = false;
    }

bool KeyManager::instanceCheck(int key, bool changed, float interval) {
    if (key >= KEY_NUMBER)
        throw out_of_range("Unknown key -> " + to_string(key));

    Clock::time_point current = Clock::now();
    float duration = chrono::duration_cast<chrono::duration<float>>(current - _keys[key].lastGetTime).count();

    if (duration < interval)
        return false;

    if ((changed && _keys[key].pressedSinceLastCheck) || (!changed && _keys[key].pressed)) {
        _keys[key].lastGetTime = current;
        _keys[key].pressedSinceLastCheck = false;
        return true;
        }
    else return false;
    }

bool KeyManager::check(int key, bool changed, float interval) {
    return get().instanceCheck(key, changed, interval);
    }
