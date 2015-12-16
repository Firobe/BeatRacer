#ifndef VIDEO__H
#define VIDEO__H

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

class Video {
    public:
        Video(int, int, void*);
        ~Video();
        void refresh();
        GLFWwindow* win();
        static void Project2D(Video&);
        static void Project3D(Video&);
    private:
        GLFWwindow* _window;
    };

#endif
