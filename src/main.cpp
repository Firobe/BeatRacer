#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

using namespace std;

void drawTriangle();

static void error_callback(int error, const char* description) {
    cerr << description;
    }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Hellow world", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
        }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1., 1., 1., -1.);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50., 0., 0., 1.);
        drawTriangle();
        glfwSwapBuffers(window);
        glfwPollEvents();
        }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    }

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(1., 0., 0.);
    glVertex3f(-0.6, -0.4, 0.);
    glColor3f(0., 1., 0.);
    glVertex3f(0.6, -0.4, 0.);
    glColor3f(0., 0., 1.);
    glVertex3f(0., 0.6, 0.);
    glEnd();
    }
