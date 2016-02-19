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
    Map map;
    map.load("road");
    NoteHandler notehandler("pouet", map);
    audio.playSource();
    Ship ship(map.getMap());

    //Main loop
    while (!glfwWindowShouldClose(video.win())) {

        //User-interface operations
        glfwPollEvents();
        pos = video.getCursor();
        video.rotateCamera(zAxis, pos[0]);
        video.rotateCamera(yAxis, pos[1]);

        if (keys[GLFW_KEY_UP])
            ship.thrust(ACCELERATION);

        if (keys[GLFW_KEY_DOWN])
            ship.thrust(-ACCELERATION);

        if (keys[GLFW_KEY_RIGHT])
            ship.turn(-2);

        if (keys[GLFW_KEY_LEFT])
            ship.turn(2);

        if (keys[GLFW_KEY_SPACE])
            video.switchFreeFly();

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

        //General operations
        ship.manage();
        notehandler.placeBar(ship.getAbsPos().x, map);
        video.shipCamera(ship.getAbsPos(), ship.getVertical(), map);
        audio.changePitch(5 * ship.getSpeed());
        audio.sync();

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map.draw(video);
        ship.draw(video);
        notehandler.draw(video);
        video.refresh();
        }

    return 0;
    }
