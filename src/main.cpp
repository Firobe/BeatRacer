#include "main.h"

using namespace std;

int main(int argc, char** argv) {
    bool keys[400];

    for (int i = 0 ; i < 400 ; i++) keys[i] = false;

    glm::vec2 pos;

    //Init Audio/Video/Text
    Video video(1000, 800, (void*)keys,
                "res/shaders/shaderVERT.vert", "res/shaders/shaderFRAG.frag");
    Audio audio;
    audio.loadBuffer("res/test.wav");
    audio.playSource();
    Map map;
    map.load("road");

    Ship ship(map.getMap());

    //Main loop
    while (!glfwWindowShouldClose(video.win())) {
        glfwWaitEvents();
        pos = video.getCursor();
        video.rotateCamera(zAxis, pos[0]);
        video.rotateCamera(yAxis, pos[1]);

        //Music speed +-10% when UP or DOWN is pressed
        if (keys[GLFW_KEY_UP])
            ship.thrust(ACCELERATION);

        if (keys[GLFW_KEY_DOWN])
            ship.thrust(-ACCELERATION);

        if (keys[GLFW_KEY_RIGHT])
            ship.turn(-2);

        if (keys[GLFW_KEY_LEFT])
            ship.turn(2);

        if (keys[GLFW_KEY_W])
            video.cameraTranslate(xAxis, 0.2);

        if (keys[GLFW_KEY_S])
            video.cameraTranslate(xAxis, -0.2);

        if (keys[GLFW_KEY_A])
            video.cameraTranslate(yAxis, 0.2);

        if (keys[GLFW_KEY_D])
            video.cameraTranslate(yAxis, -0.2);

        if (keys[GLFW_KEY_LEFT_CONTROL])
            while (!keys[GLFW_KEY_RIGHT_CONTROL]) glfwWaitEvents();

        //Misc operations

        audio.changePitch(5 * ship.getSpeed());
        ship.manage();

        //Clearing screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //RENDERING
        map.draw(video);
        ship.draw(video);

        //Updating screen
        video.refresh();
        }

    return 0;
    }
