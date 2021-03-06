#include "main.h"
#define PITCH_MAX 1.20
#define PITCH_SUPERMAX 1.3
#define PITCH_MIN 0.8
#define PITCH_STEP 0.05

using namespace std;

int main(int argc, char** argv) {
    try {
        unsigned int choice;

        ///////////////MENU////////////
        cout << "BeatRacer (InDev)\n========================\nAvailable tracks : \n";

        for (string s : Track::List()) {
            static int i = 1;
            cout << "[" << i++ << "] " << s << endl;
            }

        do {
            cout << "Your choice : ";
            cin >> choice;
            }
        while (choice < 1 || choice > Track::List().size());

        Track track(choice - 1);
		cout << "\nAvailable difficulties : \n";
        for (string s : track.getDiffList()) {
            static int i = 1;
            cout << "[" << i++ << "] " << s << endl;
            }

        do {
            cout << "Your choice : ";
            cin >> choice;
            }
        while (choice < 1 || choice > track.getDiffList().size());

        track.setDiffID(choice - 1);
        cout << "\nWhat to do with it ?\n[1] Play on it\n[2] Edit it (creates it if nonexistent (or not))" << endl;

        do {
            cout << "Your Choice : ";
            cin >> choice;
            }
        while (choice != 1 && choice != 2);

        ///////////////INIT////////////
        //Init Audio/Video/Text
        cout << "Loading audio...\n";
        Audio audio;
        audio.loadBuffer(track.getOGG());
		cout << "Computing map sectors...\n";
		Map map;
		map.loadModel(track.getMAP());
		cout << "Creating window...\n";

        Video video("default.vert", "default.frag");
        video.addShader("default.vert", "ship.frag");
        video.addShader("default.vert", "note.frag");
        video.addShader("2d.vert", "2d.frag");
        video.addShader("2d.vert", "text.frag");

		map.loadTexture("");
		map.loadV();
        TwInit(TW_OPENGL, nullptr);

        if (choice == 1)
            gameLoop(video, audio, track, map);
        else
            editorLoop(video, audio, track, map);

        TwTerminate();
        }
    catch (exception const& ex) {
        cout << "ERROR : " << ex.what() << endl;
        return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
    }

void gameLoop(Video& video, Audio& audio, Track& track, Map& map) {
    stringstream ss;
    glm::vec2 pos;
    double pitchGoal = 1.;
    bool superSpeed = false;
    Ship ship(map.getMap());
    LifeBar bar(glm::vec2(screen_width, screen_height));
    NoteHandler notehandler(track.getNT(), map, bar, ship);
    Text font(glm::vec2(screen_width, screen_height), 60.);
    font.load("klill");
    //font.setSize(glm::vec2(300, 300));

    ////////////MAIN LOOP/////////
    cout << "Start !" << endl;
    audio.playSource();

    while (!glfwWindowShouldClose(video.win())) {

        //User-interface operations
        glfwPollEvents();
        //pos = video.getCursor();

        if (KeyManager::check(GLFW_KEY_ENTER, true) || audio.hasEnded()) {
            video.reset();
            ship.reset();
            bar.setValue(100.);
            notehandler.reset();
            pitchGoal = 1.;
            superSpeed = false;
            audio.reset();
            }

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

        //General operations
        ship.manage();
        notehandler.placeBar(ship.getAbsPos().x, map);
        notehandler.checkNotes();
        video.shipCamera(ship.getAbsPos(), ship.getVertical(), map);
        audio.changePitch(ship.getSpeed() / SPEED_REFERENCE);
        ss.str("");
        ss << "FPS (" << FPS_GOAL << ") : " << audio.sync();

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        map.draw(video);
        ship.draw(video);
        notehandler.draw(video);
        bar.draw(video);
        font.drawString(glm::vec2(10., 90.), ss.str(), video);
        ss.str("");
        ss << "Speed x" << pitchGoal;
        font.drawString(glm::vec2(10., 42.), ss.str(), video);
        ss.str("");
        ss << "Combo x" << notehandler.getCombo() << "   Score " << fixed << setprecision(0) << notehandler.getScore() << resetiosflags(ios::fixed) << setprecision(6);
        font.drawString(glm::vec2(screen_width / 2 - 100., 42.), ss.str(), video);
        video.refresh();
        }
    }

//TW CALLBACKS
struct Disco {
    Video& video;
    Audio& audio;
    Map& map;
    Track& track;
    };
void TW_CALL resetCamCall(void* v) {
    *((glm::quat*)v) = glm::quat(1., 0., 0., 0.);
    }
void TW_CALL resetSetCall(void* v) {
    *((glm::dvec4*)v) = glm::dvec4(1., 0., 0., 1.);
    }
void TW_CALL saveCall(void* v) {
    struct Disco* s = (Disco*) v;
    s->map.write(s->track.getMAP());
    }
void TW_CALL saveTryCall(void* v) {
    struct Disco* s = (Disco*) v;
    s->map.write(s->track.getMAP());
    gameLoop(s->video, s->audio, s->track, s->map);
    }

void editorLoop(Video& video, Audio& audio, Track& track, Map& map) {
    stringstream ss;
    Disco send = {video, audio, map, track};
    unsigned int curSector = 0, oldSector = 1;
    glm::dvec4 sector(1., 0, 0, 1);
    glm::dvec4 oldSec = sector;
    glm::dvec2 cursor = KeyManager::mousePosition();
    glm::dvec2 oldCursor = cursor;
    double tranSpeed = 0.2;
    auto segMap = map.getMap();
    Text font(glm::vec2(screen_width, screen_height), 60.);
    font.load("atari");
    TwWindowSize(screen_width, screen_height);
    glm::dquat dir;
    TwBar* tbar = TwNewBar("TweakBar");
    TwAddVarRW(tbar, "Camera rotation", TW_TYPE_QUAT4D, &dir, " axisx=-z axisy=-x axisz=y ");
    TwAddButton(tbar, "Reset camera", resetCamCall, &dir, "");
    TwAddVarRW(tbar, "Camera speed", TW_TYPE_DOUBLE, &tranSpeed, " min=0. precision=3 step=0.1 ");
    TwAddVarRW(tbar, "Current sector", TW_TYPE_UINT32, &curSector, "");
    TwAddVarRW(tbar, "Sector repetition", TW_TYPE_DOUBLE, &sector[3], " min=1 ");
    TwAddVarRW(tbar, "Sector length", TW_TYPE_DOUBLE, &sector[0], " min=0. precision=5 step=0.0001 ");
    TwAddVarRW(tbar, "Sector theta", TW_TYPE_DOUBLE, &sector[1], " precision=5 step=0.0001 ");
    TwAddVarRW(tbar, "Sector phi", TW_TYPE_DOUBLE, &sector[2], " precision=5 step=0.0001 ");
    TwAddButton(tbar, "Reset settings", resetSetCall, &sector, "");
    TwAddButton(tbar, "Save map", saveCall, &send, "");
    TwAddButton(tbar, "Save and try", saveTryCall, &send, "");
#ifndef _WIN32
    if(freopen("/dev/null", "w", stderr) == nullptr)
		cout << "WARNING : Couldn't redirect cerr stream" << endl;
#endif
    video.twRedirect();

    ////////////MAIN LOOP/////////
    while (!glfwWindowShouldClose(video.win())) {
        if (sector != oldSec) {
            map.loadModel("editor");
            map.loadV();
            oldSec = sector;
            }

        video.quatCamera(dir);

        if (KeyManager::check(GLFW_KEY_W))
            video.translateCamera(dir, glm::dvec3(tranSpeed, 0, 0));

        if (KeyManager::check(GLFW_KEY_S))
            video.translateCamera(dir, glm::dvec3(-tranSpeed, 0, 0));

        if (KeyManager::check(GLFW_KEY_D))
            video.translateCamera(dir, glm::dvec3(0, -tranSpeed, 0));

        if (KeyManager::check(GLFW_KEY_A))
            video.translateCamera(dir, glm::dvec3(0, +tranSpeed, 0));

        if (KeyManager::check(GLFW_KEY_LEFT_SHIFT))
            video.translateCamera(dir, glm::dvec3(0, 0, tranSpeed));

        if (KeyManager::check(GLFW_KEY_LEFT_CONTROL))
            video.translateCamera(dir, glm::dvec3(0, 0, -tranSpeed));

        cursor = KeyManager::mousePosition();

        if (KeyManager::mouseCheck(GLFW_MOUSE_BUTTON_LEFT) && oldCursor != cursor) {
            dir = glm::rotate(dir, glm::radians((oldCursor.y - cursor.y) * FOV / screen_height), glm::dvec3(0, 1, 0));
            dir = glm::rotate(dir, glm::radians((oldCursor.x - cursor.x) * FOV / screen_width), glm::dvec3(0, 0, -1));
            }

        oldCursor = cursor;

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
