#ifndef AUDIO__H
#define AUDIO__H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include "video.h"
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>
#include <ctime>
#include <chrono>
#include <stdexcept>

#define SAMPLES_PER_SECOND 44100

void list_audio_devices(const ALCchar* devices);
typedef std::chrono::high_resolution_clock Clock;

class Audio {
    public:
        Audio();
        void loadBuffer(std::string);
        void playSource();
        void changePitch(float);
        void printPosition();
        void sync();
        ~Audio();
    private:
        ALCdevice* _device;
        ALCcontext* _context;
        ALuint _source;
        ALuint _buffer;

        float _pitch;
        float _correction;
    };

#endif
