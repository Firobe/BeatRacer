#include "video.h"

using namespace std;
int c;


static void error_callback(int error, const char* description) {
    cerr << description;
    }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    void* data = glfwGetWindowUserPointer(window);
    bool* keys = (bool*) data;

    if (key == GLFW_KEY_UP)
        keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);

    if (key == GLFW_KEY_DOWN)
        keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
    }

Video::Video(int width, int height, void* pointer) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    _window = glfwCreateWindow(width, height, "BeatRacer", NULL, NULL);

    if (!_window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
        }

    glfwSetWindowUserPointer(_window, pointer);
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(_window, key_callback);
    }

Video::~Video() {
    glfwDestroyWindow(_window);
    glfwTerminate();
    }

void Video::refresh() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
    }

GLFWwindow* Video::win() {
    return _window;
    }

void Video::Project3D(Video& video) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio;
    int width, height;
    glfwGetFramebufferSize(video.win(), &width, &height);
    ratio = width / (float) height;
    gluPerspective(FOV, ratio, NEAR, FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    }

void Video::Project2D(Video& video) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetFramebufferSize(video.win(), &width, &height);
    glOrtho(0., width, 0., height, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    }

void drawSegment(CoordSPH next) {

    c += 20;
    c %= 256;
    glRotatef(next.theta, 0, 0, 1);
    glRotatef(-next.phi, 0, 1, 0);
    glTranslatef(next.rho / 2, 0, 0);
    glScalef(1, 1 / (10 * next.rho), 1 / (100 * next.rho));

    glColor3ub(c, c, c);
    glutWireCube(next.rho);

    glScalef(1, 10 * next.rho, 100 * next.rho);
    glTranslatef(next.rho / 2, 0, 0);
    }
