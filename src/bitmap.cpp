#define STB_TRUETYPE_IMPLEMENTATION
#include "bitmap.h"
#include <stdexcept>
#include <fstream>

std::ifstream::pos_type filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg(); 
}

using namespace std;

Bitmap::Bitmap() : Texture(), _fontHeight(-1){
}

Bitmap::Bitmap(string path, float height) : Texture(path){
	_fontHeight = height;
}

Bitmap::~Bitmap() {
}

void Bitmap::setHeight(float h){
	_fontHeight = h;
}

void Bitmap::load(){
	unsigned int size = filesize(_path.c_str());
	unsigned char* buffer = new unsigned char[size];
	unsigned char bitm[BITMAP_SIZE * BITMAP_SIZE];
	FILE* file = NULL;
	file = fopen(_path.c_str(), "rb");

	if( file == NULL )
		throw runtime_error("Unable to open " + _path);

	fread(buffer, 1, size, file);
	fclose(file);

	if( stbtt_BakeFontBitmap(buffer, 0, _fontHeight, bitm, BITMAP_SIZE, BITMAP_SIZE, 32, 96, _cdata) == 0)
		throw runtime_error("No character fit in the bitmap");

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, BITMAP_SIZE, BITMAP_SIZE, 0, GL_ALPHA, GL_UNSIGNED_BYTE, _cdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	free(buffer);
}
