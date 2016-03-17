#ifndef BITMAP__H
#define BITMAP__H

#include "texture.h"
#include "../libs/stb_truetype.h"

#include <string>

#define BITMAP_SIZE (128)
class Bitmap : public Texture{
    public:
		Bitmap();
        Bitmap(std::string, float);
        ~Bitmap();
		void load();	
		void setHeight(float);
    private:
		stbtt_bakedchar _cdata[96];
		stbtt_fontinfo _font;
		float _fontHeight;
    };

#endif
