#include "main.h"

using namespace std;

int main(int argc, char** argv) {
	try {
		glm::vec2 pos;
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

			if (KeyManager::check(GLFW_KEY_UP)){
                if(KeyManager::check(GLFW_KEY_SPACE)){
                    ship.thrust(2 * ACCELERATION);
                    bar.setValue(bar.getValue() - 0.2);
                }
            else
				ship.thrust(ACCELERATION);
			}

			if (KeyManager::check(GLFW_KEY_DOWN))
				ship.thrust(-ACCELERATION);

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
			audio.changePitch(17 * ship.getSpeed());
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
