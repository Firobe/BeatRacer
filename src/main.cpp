#include "main.h"

using namespace std;

int main(int argc, char** argv) {
	try {
		glm::vec2 pos;
		//Init Audio/Video/Text
		Video video(1000, 800, "default.vert", "default.frag");
		video.addShader("default.vert", "ship.frag");
		video.addShader("default.vert", "note.frag");
		video.addShader("2d.vert", "default.frag");
		Audio audio;
		audio.loadBuffer("res/test.wav");
		Map map;
		map.load("test");
		NoteHandler notehandler("test", map);
		//audio.playSource();
		Ship ship(map.getMap());
		LifeBar bar(glm::vec2(1000., 800.));

		//Main loop
		while (!glfwWindowShouldClose(video.win())) {

			//User-interface operations
			glfwPollEvents();
			pos = video.getCursor();
			video.rotateCamera(zAxis, pos[0]);
			video.rotateCamera(yAxis, pos[1]);

			if (KeyManager::check(GLFW_KEY_UP))
				ship.thrust(ACCELERATION);

			if (KeyManager::check(GLFW_KEY_DOWN))
				ship.thrust(-ACCELERATION);

			if (KeyManager::check(GLFW_KEY_RIGHT))
				ship.turn(-2);

			if (KeyManager::check(GLFW_KEY_LEFT))
				ship.turn(2);

			if (KeyManager::check(GLFW_KEY_SPACE, true))
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
			audio.changePitch(5 * ship.getSpeed());
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
