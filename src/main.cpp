#include "main.h"

using namespace std;

void drawTriangle();

int main() {
    bool keys[2] = {false, false};
    float pitch = 1.;
    float rotation = 0;
    Video video(800, 600, (void*)keys);
    Audio audio;
    audio.loadBuffer("test.wav");
    audio.playSource();

    while (!glfwWindowShouldClose(video.win())) {
        if (keys[0])
            pitch += 0.002;

        if (keys[1])
            pitch -= 0.002;

        audio.changePitch(pitch);
        float ratio;
        int width, height;
        glfwGetFramebufferSize(video.win(), &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1., 1., 1., -1.);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(rotation += 5 * pitch, 0, 0, 1);
        drawTriangle();
        video.refresh();
        }

    return 0;
    }

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(1., 0., 0.);
    glVertex3f(-0.6, -0.4, 0.);
    glColor3f(0., 1., 0.);
    glVertex3f(0.6, -0.4, 0.);
    glColor3f(0., 0., 1.);
    glVertex3f(0., 0.6, 0.);
    glEnd();
    }
