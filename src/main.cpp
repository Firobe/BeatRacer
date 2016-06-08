#include "main.h"
#define PITCH_MAX 1.20
#define PITCH_SUPERMAX 1.3
#define PITCH_MIN 0.8
#define PITCH_STEP 0.05

using namespace std;
string mapName;

int main(int argc, char** argv) {
    try {
        int choice;
        ///////////////MENU////////////
        cout << "BeatRacer (InDev)\n========================\nMap name : ";
        cin >> mapName;

        do {
            cout << "What to do with it ?\n\t1 - Play on it\n\t2 - Edit it (creates it if nonexistent (or not))\nChoice : ";
            cin >> choice;
            }
        while (choice != 1 && choice != 2);

        ///////////////INIT////////////
        //Init Audio/Video/Text
        cout << "Loading audio... (might take some time)" << endl;
        Audio audio;
        audio.loadBuffer("res/" + mapName + ".ogg");

        Video video("default.vert", "default.frag");
        video.addShader("default.vert", "ship.frag");
        video.addShader("default.vert", "note.frag");
        video.addShader("2d.vert", "2d.frag");
        video.addShader("2d.vert", "text.frag");

        TwInit(TW_OPENGL, NULL);

        if (choice == 1)
            gameLoop(video, audio);
        else
            editorLoop(video, audio);

        TwTerminate();
        }
    catch (exception const& ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
    }

void gameLoop(Video& video, Audio& audio) {
    stringstream ss;
    glm::vec2 pos;
    double pitchGoal = 1.;
    bool superSpeed = false;
    Map map;
    map.load(mapName);
    Ship ship(map.getMap());
    LifeBar bar(glm::vec2(screen_width, screen_height));
    NoteHandler notehandler(mapName, map, bar, ship);
    Text font(glm::vec2(screen_width, screen_height), 60.);
    font.load("atari");
    //font.setSize(glm::vec2(300, 300));

    ////////////MAIN LOOP/////////
    cout << "Start !" << endl;
    audio.playSource();

    while (!glfwWindowShouldClose(video.win())) {

        //User-interface operations
        glfwPollEvents();
        pos = video.getCursor();
        video.rotateCamera(zAxis, pos[0]);
        video.rotateCamera(yAxis, pos[1]);

        if (bar.getValue() > 0. && KeyManager::check(GLFW_KEY_SPACE) && KeyManager::check(GLFW_KEY_UP))
            superSpeed = true;

        if (bar.getValue() <= 0. || (KeyManager::check(GLFW_KEY_SPACE) && KeyManager::check(GLFW_KEY_DOWN)))
            superSpeed = false;

        if (!superSpeed && KeyManager::check(GLFW_KEY_DOWN, true))
            pitchGoal = (pitchGoal - PITCH_STEP <= PITCH_MIN) ?  PITCH_MIN : pitchGoal - PITCH_STEP;

        if (!superSpeed && KeyManager::check(GLFW_KEY_UP, true))
            pitchGoal = (pitchGoal + PITCH_STEP >= PITCH_MAX) ? PITCH_MAX : pitchGoal + PITCH_STEP;

        if (superSpeed) {
            ship.setFriction(PITCH_SUPERMAX);
            bar.setValue(bar.getValue() - 0.2);

            }
        else
            ship.setFriction(pitchGoal);

        ship.thrust(ACCELERATION);

        if (KeyManager::check(GLFW_KEY_RIGHT)) {
            if (bar.getValue() > 0 && KeyManager::check(GLFW_KEY_SPACE)) {
                ship.turn(-5);
                bar.setValue(bar.getValue() - 0.1);
                }
            else ship.turn(-2);
            }

        if (KeyManager::check(GLFW_KEY_LEFT)) {
            if (bar.getValue() > 0 && KeyManager::check(GLFW_KEY_SPACE)) {
                ship.turn(5);
                bar.setValue(bar.getValue() - 0.1);
                }
            else ship.turn(2);
            }

        if (KeyManager::check(GLFW_KEY_LEFT_ALT, true))
            video.switchFreeFly();

        if (KeyManager::check(GLFW_KEY_W))
            video.cameraTranslate(xAxis, 0.2);

        if (KeyManager::check(GLFW_KEY_S))
            video.cameraTranslate(xAxis, -0.2);

        if (KeyManager::check(GLFW_KEY_A))
            video.cameraTranslate(yAxis, 0.2);

        if (KeyManager::check(GLFW_KEY_D))
            video.cameraTranslate(yAxis, -0.2);

        //General operations
        ship.manage();
        notehandler.placeBar(ship.getAbsPos().x, map);
        notehandler.checkNotes();
        video.shipCamera(ship.getAbsPos(), ship.getVertical(), map);
        audio.changePitch(ship.getSpeed() / SPEED_REFERENCE);
        ss.str("");
		ss << audio.sync();

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map.draw(video);
        ship.draw(video);
        notehandler.draw(video);
        bar.draw(video);
        font.drawString(glm::vec2(screen_width - 300, 40), ss.str(), video);
        ss.str("");
        ss << "x" << pitchGoal;
        font.drawString(glm::vec2(10., 42.), ss.str(), video);
        video.refresh();
        }
    }

void editorLoop(Video& video, Audio& audio) {
    Map map;
    stringstream ss;
    struct Disco {
        Video& video;
        Audio& audio;
        Map& map;
        } send = {video, audio, map};
    unsigned int curSector = 0, oldSector = 1;
    glm::dvec4 sector(1., 0, 0, 1);
    glm::dvec4 oldSec = sector;
    map.load(mapName);
    auto segMap = map.getMap();
    Text font(glm::vec2(screen_width, screen_height), 60.);
    font.load("atari");
    TwWindowSize(screen_width, screen_height);
    glm::quat dir;
    TwBar* tbar = TwNewBar("TweakBar");
    TwAddVarRW(tbar, "Camera direction", TW_TYPE_QUAT4F, &dir, " axisx=-z axisy=x axisz=-y ");
    TwAddButton(tbar, "Reset camera", [](void* v) {
        *((glm::quat*)v) = glm::quat(1., 0., 0., 0.);
        }, &dir, "");
    TwAddVarRW(tbar, "Current sector", TW_TYPE_UINT32, &curSector, "");
    TwAddVarRW(tbar, "Sector repetition", TW_TYPE_DOUBLE, &sector[3], " min=1 ");
    TwAddVarRW(tbar, "Sector length", TW_TYPE_DOUBLE, &sector[0], " min=0. precision=5 step=0.0001 ");
    TwAddVarRW(tbar, "Sector theta", TW_TYPE_DOUBLE, &sector[1], " precision=5 step=0.0001 ");
    TwAddVarRW(tbar, "Sector phi", TW_TYPE_DOUBLE, &sector[2], " precision=5 step=0.0001 ");
    TwAddButton(tbar, "Reset settings", [](void* v) {
        *((glm::dvec4*)v) = glm::dvec4(1., 0., 0., 1.);
        }, &sector, "");
    TwAddButton(tbar, "Save map", [](void* m) {
        ((Map*)m)->write(mapName);
        }, &map, "");
    TwAddButton(tbar, "Save and try", [](void* v) {
        struct Disco* s = (Disco*) v;
        s->map.write(mapName);
        gameLoop(s->video, s->audio);
        }, &send, "");
    freopen("/dev/null", "w", stderr);
    video.twRedirect();
    //font.setSize(glm::vec2(300, 300));

    ////////////MAIN LOOP/////////
    while (!glfwWindowShouldClose(video.win())) {
        if (sector != oldSec) {
            map.loadModel("editor");
            map.loadV();
            oldSec = sector;
            }

        video.quatCamera(dir);

        //User-interface operations
        glfwPollEvents();

        //General operations
        ss.str("");
        ss << audio.sync(); //FPS CONTROL

        if (oldSector != curSector) {
            sector = map.getSegment(curSector);
            oldSector = curSector;
            }

        map.setMapSeg(curSector, sector);

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map.draw(video);
        font.drawString(glm::vec2(screen_width - 300, 40), ss.str(), video);
        TwDraw();
        video.refresh();
        }

    }
