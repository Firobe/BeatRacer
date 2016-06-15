#include "audio.h"
#include "video.h"
#include "../libs/stb_vorbis.c"
#include <stdexcept>
#include <cstdlib>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

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
    _correction = 1;
    _device = alcOpenDevice(NULL);

    if (!_device)
        throw runtime_error("OpenAL : can't open default audio device");

    _context = alcCreateContext(_device, NULL);

    if (!alcMakeContextCurrent(_context))
        throw runtime_error("OpenAL : failed to make context current");

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
    }

void Audio::loadBuffer(string name) {
    int samples, sample_rate, channels;
    short* data;
    alGenBuffers((ALuint)1, &_buffer);

    samples = stb_vorbis_decode_filename(name.c_str(), &channels, &sample_rate, &data);

    if (samples <= 0)
        throw runtime_error("Unable to open " + name);

    alBufferData(_buffer, AL_FORMAT_STEREO16, data, channels * samples * sizeof(short), sample_rate);
    alSourcei(_source, AL_BUFFER, _buffer);
    free(data);
    }

void Audio::playSource() {
    alSourcePlay(_source);
    }

void Audio::changePitch(double pitch) {
    _pitch = pitch;
    }

float Audio::sync() {
    static auto prev = Clock::now();
    static bool first = true;
    auto now = Clock::now();

    if (!first) {
        double diff = (double)chrono::duration_cast<chrono::duration<double>>(now - prev).count();

        while (diff < FPS_INTERVAL) {
            now = Clock::now();
            diff = (double)chrono::duration_cast<chrono::duration<double>>(now - prev).count();
            }

        _correction = (double)FPS_INTERVAL / diff ;
        }
    else _correction = 1.;

    first = false;
    prev = now;
    alSourcef(_source, AL_PITCH, _pitch * _correction);
    return FPS_GOAL * _correction;
    }
