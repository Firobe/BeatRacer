#include "audio.h"

using namespace std;

/*
void listDev(const ALCchar *devices) {
    const ALCchar *device = devices, *next = devices + 1;
    size_t len = 0;

    cout << "Devices list:\n";
    cout << "----------\n";

    while (device && *device != '\0' && next && *next != '\0') {
        cout << device << endl;;
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
        }

    cout << "----------\n";
    }
*/

Audio::Audio() {
    alutInitWithoutContext(NULL, NULL);
    _device = alcOpenDevice(NULL);

    if (!_device) {
        cout << "!! Can't open default audio device : halting !!" << endl;
        exit(EXIT_FAILURE);
        }

    _context = alcCreateContext(_device, NULL);

    if (!alcMakeContextCurrent(_context)) {
        cout << "!! (OpenAL) Failed to make context current : halting !!" << endl;
        exit(EXIT_FAILURE);
        }

    alGenSources((ALuint)1, &_source);
    alSourcef(_source, AL_PITCH, 1);
    alSourcef(_source, AL_GAIN, 1);
    alSource3f(_source, AL_POSITION, 0, 0, 0);
    alSource3f(_source, AL_VELOCITY, 0, 0, 0);
    alSourcei(_source, AL_LOOPING, AL_TRUE);
    }

Audio::~Audio() {
    alDeleteSources(1, &_source);
    alDeleteBuffers(1, &_buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
    alutExit();
    }

void Audio::loadBuffer(string name) {
    ALsizei size;
    ALfloat freq;
    ALenum format;
    alGenBuffers((ALuint)1, &_buffer);
    ALvoid* data = alutLoadMemoryFromFile(name.c_str(), &format, &size, &freq);
    alBufferData(_buffer, format, data, size, freq);
    alSourcei(_source, AL_BUFFER, _buffer);
    }

void Audio::playSource() {
    alSourcePlay(_source);
    }

void Audio::changePitch(float pitch) {
    alSourcef(_source, AL_PITCH, pitch);
    }
