#include "keymanager.h"
#include <stdexcept>
#include <sstream>
#include <string>

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
	if (key >= KEY_NUMBER){
		stringstream ss;
		ss << key;
		throw out_of_range("Unknown key -> " + ss.str());
	}

	_keys[key].pressed = true;
	_keys[key].pressedSinceLastCheck = true;
}

void KeyManager::release(int key) {
	if (key >= KEY_NUMBER){
		stringstream ss;
		ss << key;
		throw out_of_range("Unknown key -> " + ss.str());
	}

	_keys[key].pressed = false;
}

bool KeyManager::instanceCheck(int key, bool changed, float interval) {
	if (key >= KEY_NUMBER){
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
	else{
		_keys[key].pressedSinceLastCheck = false;
		return false;
	}
}

bool KeyManager::check(int key, bool changed, float interval) {
	return get().instanceCheck(key, changed, interval);
}
