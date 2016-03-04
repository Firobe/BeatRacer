#ifndef KEYMANAGER__H
#define KEYMANAGER__H

#include <string>
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <vector>

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
        static bool check(int key, bool changed = false, float interval = 0.);
		/*Checks if 'key' is pressed (by default). If 'changed' is specified, checks if 'key' has been released then pressed again by the
		user since the last successful check. If 'interval' is specidied, it additionally checks if 'interval' seconds or more ellapsed
		since the last successful check.*/
        ~KeyManager();
        void press(int);
        void release(int);
    private:
        KeyManager();
        bool instanceCheck(int, bool, float);
        std::vector<Key> _keys;
    };

#endif
