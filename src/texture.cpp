#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../libs/stb_image.h"
#pragma GCC diagnostic pop

#include <vector>
#include <stdexcept>

using namespace std;

Texture::Texture(): Texture("Empty path") {
    }

Texture::Texture(string path) : _path(path), _id(-1), _width(-1), _height(-1) {
    }

Texture::~Texture() {
	if(!empty())
    	glDeleteTextures(1, &_id);
    }

GLuint Texture::getID() {
    return _id;
    }

bool Texture::empty() {
    return (_width == -1);
    }

void Texture::setPath(string str) {
    _path = str;
    }

void Texture::load() {
    int compN;
    unsigned char* data = stbi_load(_path.c_str(), &_width, &_height, &compN, 0);

    if (data == nullptr)
        throw runtime_error("Unable to open "+ _path);

    //Flipping texture
    vector<char> flipData(_width * _height * compN);

    for (int i = 0 ; i < _height ; i++)
        for (int j = 0 ; j < compN * _width ; j++)
            flipData[(_height - 1 - i) * compN * _width + j] = data[i * compN * _width + j];

    GLenum format = (compN == 3) ? GL_RGB : GL_RGBA;
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, flipData.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    }
