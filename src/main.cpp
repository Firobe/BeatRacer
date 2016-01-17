#include "main.h"

using namespace std;

void drawCube();

int main(int argc, char** argv) {
    bool keys[2] = {false, false};
    float pitch = 1, goal = 1;

    //Init Audio/Video/Text
    Video video(1000, 800, (void*)keys,
                "res/shaders/shaderVERT.vert", "res/shaders/shaderFRAG.frag");
    Audio audio;
    audio.loadBuffer("res/test.wav");
    audio.playSource();
    Map map;
    map.load("road");

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
        //map.forward(pitch / 100);

        //Clearing screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //RENDERING
        map.draw(video);

        //Updating screen
        video.refresh();
        }

    return 0;
    }
