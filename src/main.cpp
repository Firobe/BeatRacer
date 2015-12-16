#include "main.h"
#include <ctime>
#include <cmath>

using namespace std;

void drawCube();

int main() {
    char str[100];
    srand(time(0));
    bool keys[2] = {false, false};
    float pitch = 1, goal = 1;

    //Init Audio/Video/Text
    Video video(800, 600, (void*)keys);
    Audio audio;
    audio.loadBuffer("test.wav");
    audio.playSource();
    FTFont* sans = Text::Instance().getFont("comic.ttf", 40);

    //Main loop
    while (!glfwWindowShouldClose(video.win())) {

        //Music speed +-10% when UP or DOWN is pressed
        if (keys[0]) {
            goal += 0.1;
            keys[0] = false;
            }

        if (keys[1]) {
            goal -= 0.1;
            keys[1] = false;
            }

        pitch -= 0.1 * (pitch - goal); //Smooth transition from pitch to goal
        audio.changePitch(pitch);

        //Screen clearing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //3D RENDERING
        Video::Project3D(video);
        glRotatef(300 * (pitch - 1), 0, 1, 1);
        drawCube();

        //2D RENDERING
        Video::Project2D(video);
        glTranslatef(200, 50, 0);
        sprintf(str, "musique a %d %% xd lol !", (int)round(pitch * 100));
        Text::Render(str, sans);

        //Updating screen
        video.refresh();
        }

    return 0;
    }

void drawCube() {
    /*
     * Draws a colored centered cube.
     */
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
