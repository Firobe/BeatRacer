#ifndef VIDEO__H
#define VIDEO__H

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

#define NEAR 0.1
#define FAR 1000
#define FOV 70

typedef struct Coord3D Coord3D;
struct CoordSPH {
    float rho, theta, phi;
    };

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
void drawSegment(CoordSPH next);

#endif
