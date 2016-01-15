#include "video.h"

int c;
using namespace std;


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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
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
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    }

void Video::Project2D(Video& video) {
    glPushMatrix();
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

void Video::DrawSegment(CoordSPH next, GLuint texture, float cursor, float delta) {
    if (delta != 0.) {
        next.rho -= delta;
        next.phi = 0;
        next.theta = 0;
        }

    glRotatef(next.theta, 0, 0, 1);
    glRotatef(next.phi, 0, -1, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, cursor);
    glVertex3f(0, -0.1, 0);
    glTexCoord2f(1, cursor);
    glVertex3f(0, 0.1, 0);
    glTexCoord2f(1, cursor + next.rho);
    glVertex3f(next.rho, 0.1, 0);
    glTexCoord2f(0, cursor + next.rho);
    glVertex3f(next.rho, -0.1, 0);
    glEnd();

    glTranslatef(next.rho, 0, 0);
    }

void Video::DrawSegmentRev(CoordSPH prev, CoordSPH next, GLuint texture, float delta) {

    if (delta != 0.) {
        next.rho = delta;
        prev.theta = 0;
        prev.phi = 0;
        }

    glRotatef(prev.theta, 0, 0, -1);
    glRotatef(prev.phi, 0, 1, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glVertex3f(0, -0.1, 0);
    glVertex3f(0, 0.1, 0);
    glVertex3f(-next.rho, 0.1, 0);
    glVertex3f(-next.rho, -0.1, 0);
    glEnd();
    /*glTranslatef(-next.rho / 2, 0, 0);
    glScalef(1, 1 / (10 * next.rho), 1 / (100 * next.rho));
    glutSolidCube(next.rho);
    glScalef(1, 10 * next.rho, 100 * next.rho);
    glTranslatef(-next.rho / 2, 0, 0);*/
    glTranslatef(-next.rho, 0, 0);
    }


GLuint Video::LoadTexture(const char * filename) {

    GLuint texture;
    int width, height;
    unsigned char * data;
    FILE * file;
    file = fopen(filename, "rb");

    if (file == NULL) return 0;

    width = 512;
    height = 512;
    data = (unsigned char *)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height ; ++i) {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];

        data[index] = R;
        data[index + 2] = B;

        }


    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
    }
