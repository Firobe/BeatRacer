#include "main.h"
#include <ctime>

using namespace std;

void drawCube();

int main() {
    srand(time(0));
    bool keys[2] = {false, false};
    float pitch = 1.;
    Video video(800, 600, (void*)keys);
    Audio audio;
    audio.loadBuffer("test.wav");
    audio.playSource();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio;
    int width, height;
    glfwGetFramebufferSize(video.win(), &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glOrtho(-ratio, ratio, -1., 1., 1., -1.);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    while (!glfwWindowShouldClose(video.win())) {
        if (keys[0])
            pitch += 0.002;

        if (keys[1])
            pitch -= 0.002;

        audio.changePitch(pitch);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glRotatef(3 * (pitch - 1), 0, 1, 1);
        drawCube();
        video.refresh();
        }

    return 0;
    }

void drawCube() {
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0,  1.0, 1.0);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5,  0.5, 0.5);
    glVertex3f(-0.5,  0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0,  0.0,  1.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(0.5,  0.5,  0.5);
    glVertex3f(0.5, -0.5,  0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0,  1.0,  0.0);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0,  0.0,  1.0);
    glVertex3f(0.5,  0.5,  0.5);
    glVertex3f(0.5,  0.5, -0.5);
    glVertex3f(-0.5,  0.5, -0.5);
    glVertex3f(-0.5,  0.5,  0.5);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0,  0.0,  0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5,  0.5);
    glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
    }
