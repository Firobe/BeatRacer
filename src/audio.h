#ifndef AUDIO__H
#define AUDIO__H

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

#define SAMPLES_PER_SECOND 44100

void list_audio_devices(const ALCchar* devices);

class Audio {
    public:
        Audio();
        void loadBuffer(std::string);
        void playSource();
        void changePitch(double);
        float sync();
        ~Audio();
    private:
        ALCdevice* _device;
        ALCcontext* _context;
        ALuint _source;
        ALuint _buffer;

        double _pitch;
        double _correction;
    };

#endif
