#ifndef VIDEO__H
#define VIDEO__H

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <iostream>

#define NEAR 0.01
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
        static void DrawSegment(CoordSPH next, float delta = 0.);
        static void DrawSegmentRev(CoordSPH prev, CoordSPH next, float delta = 0.);
    private:
        GLFWwindow* _window;
    };

#endif
