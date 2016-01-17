#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "../stb_image.h"


using namespace std;

Texture::Texture(): _path("NULL"), _width(-1) {
    }

Texture::Texture(string path) : _path(path), _id(0), _width(-1), _height(-1) {
    }

Texture::~Texture() {
    glDeleteTextures(1, &_id);
    }

GLuint Texture::getID() {
    return _id;
    }

bool Texture::empty() {
    return (_width == -1);
    }

bool Texture::load() {
    int compN;
    unsigned char* data = stbi_load(_path.c_str(), &_width, &_height, &compN, 0);

    if (data == NULL) {
        cout << stbi_failure_reason() << endl;
        return false;
        }

    GLenum format = (compN == 3) ? GL_RGB : GL_RGBA;

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return true;
    }
