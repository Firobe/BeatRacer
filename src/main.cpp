#include "main.h"
#include <ctime>
#include <cmath>

using namespace std;

void drawCube();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    char str[100];
    srand(time(0));
    bool keys[2] = {false, false};
    float pitch = 1, goal = 1;

    //Init Audio/Video/Text
    Video video(1000, 800, (void*)keys);
    Audio audio;
    audio.loadBuffer("res/test.wav");
    audio.playSource();
    FTFont* sans = Text::Instance().getFont("res/comic.ttf", 40);
    Game game;
    game.loadMap("res/test.map");
    Video::Project3D(video);
    game.placeCamera();
    glPushMatrix();

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
        game.forward(pitch / 100);

        //Screen clearing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //3D RENDERING
        Video::Project3D(video);
        game.drawMap();

        //2D RENDERING
        Video::Project2D(video);
        glTranslatef(200, 50, 0);
        sprintf(str, "Vitesse %d %%", (int)round(pitch * 100));
        Text::Render(str, sans);

        //Updating screen
        video.refresh();
        }

    return 0;
    }
