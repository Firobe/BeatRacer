#include "main.h"
#define PITCH_MAX 1.20
#define PITCH_SUPERMAX 1.3
#define PITCH_MIN 0.8
#define PITCH_STEP 0.05

using namespace std;

int main(int argc, char** argv) {
	try {
		glm::vec2 pos;
		float pitchGoal = 1.;
		bool superSpeed = false;
		//Init Audio/Video/Text
		Video video(1000, 800, "default.vert", "default.frag");
		video.addShader("default.vert", "ship.frag");
		video.addShader("default.vert", "note.frag");
		video.addShader("2d.vert", "2d.frag");
		Audio audio;
		audio.loadBuffer("res/test.ogg");
		Map map;
		map.load("test");
		Ship ship(map.getMap());
		LifeBar bar(glm::vec2(1000., 800.));
		NoteHandler notehandler("test", map, bar, ship);
		audio.playSource();

		//Main loop
		while (!glfwWindowShouldClose(video.win())) {

			//User-interface operations
			glfwPollEvents();
			pos = video.getCursor();
			video.rotateCamera(zAxis, pos[0]);
			video.rotateCamera(yAxis, pos[1]);

			if(bar.getValue() >= 0. && KeyManager::check(GLFW_KEY_SPACE) && KeyManager::check(GLFW_KEY_UP, true))
				superSpeed = true;
			if(bar.getValue() <= 0. || (KeyManager::check(GLFW_KEY_SPACE) && KeyManager::check(GLFW_KEY_DOWN, true)))
				superSpeed = false;

			if (!superSpeed && KeyManager::check(GLFW_KEY_DOWN, true))
				pitchGoal = (pitchGoal - PITCH_STEP <= PITCH_MIN) ?  PITCH_MIN : pitchGoal - PITCH_STEP;

			if (!superSpeed && KeyManager::check(GLFW_KEY_UP, true))
				pitchGoal = (pitchGoal + PITCH_STEP >= PITCH_MAX) ? PITCH_MAX : pitchGoal + PITCH_STEP;

			if(superSpeed){
				ship.setFriction(PITCH_SUPERMAX);
				bar.setValue(bar.getValue() - 0.2);

			}
			else
				ship.setFriction(pitchGoal);

			ship.thrust(ACCELERATION);

			if (KeyManager::check(GLFW_KEY_RIGHT)){
				if(KeyManager::check(GLFW_KEY_SPACE)){
					ship.turn(-5);
					bar.setValue(bar.getValue() - 0.1);
				}
				ship.turn(-2);
			}

			if (KeyManager::check(GLFW_KEY_LEFT)){
				if(KeyManager::check(GLFW_KEY_SPACE)){
					ship.turn(5);
					bar.setValue(bar.getValue() - 0.1);
				}
				ship.turn(2);
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

			if (KeyManager::check(GLFW_KEY_LEFT_CONTROL))
				while (!KeyManager::check(GLFW_KEY_RIGHT_CONTROL)) glfwWaitEvents();

			//General operations
			ship.manage();
			notehandler.placeBar(ship.getAbsPos().x, map);
			notehandler.checkNotes();
			video.shipCamera(ship.getAbsPos(), ship.getVertical(), map);
			audio.changePitch(ship.getSpeed() / SPEED_REFERENCE);
			audio.sync();

			//Render
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			map.draw(video);
			ship.draw(video);
			notehandler.draw(video);
			bar.draw(video);
			video.refresh();
		}
	}
	catch (exception const& ex) {
		cerr << "ERROR : " << ex.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
