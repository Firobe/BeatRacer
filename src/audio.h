#ifndef AUDIO__H
#define AUDIO__H

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <cstdlib>
#include <string.h>
#include <string>
#include <iostream>

void list_audio_devices(const ALCchar* devices);

class Audio {
    public:
        Audio();
        void loadBuffer(std::string);
        void playSource();
        void changePitch(float);
        ~Audio();
    private:
        ALCdevice* _device;
        ALCcontext* _context;
        ALuint _source;
        ALuint _buffer;
    };

#endif
