#define STB_TRUETYPE_IMPLEMENTATION
#include "bitmap.h"
#include <stdexcept>
#include <fstream>
#include <iostream>

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

	if( fread(buffer, 1, size, file) != size)
		throw runtime_error("Unable to load " + _path + " in memory");
	fclose(file);

	if( stbtt_BakeFontBitmap(buffer, 0, _fontHeight, bitm, BITMAP_SIZE, BITMAP_SIZE, 32, 96, _cdata) == 0)
		throw runtime_error("No character fit in the bitmap");

	//if( stbtt_InitFont(&_font, buffer, stbtt_GetFontOffsetForIndex(buffer, 0)) == 0)
	//	throw runtime_error("Unable to initialize font " + _path);
	//int w, h;
	//bitm = stbtt_GetCodepointBitmap(&_font, 0, stbtt_ScaleForPixelHeight(&_font, 60), 'A', &w, &h, 0, 0);

	if( bitm == NULL )
		throw runtime_error("Unable to make bitmap");

	for(int i = 0 ; i < BITMAP_SIZE ; i++){
		for(int x = 0 ; x < BITMAP_SIZE ; x++){
			printf("%c", bitm[x+i*BITMAP_SIZE]>>5 ? '1' : ' ');
		}
		printf("\n");
	}

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, BITMAP_SIZE, BITMAP_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, bitm);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] buffer;
}

