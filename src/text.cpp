#include "text.h"

using namespace std;

Text& Text::Instance() {
    static Text tm;
    return tm;
    }

Text::~Text() {
    fonts.clear();
    }

FTFont* Text::getFont(const char* filename, int size) {
    char buf[256];
    sprintf(buf, "%s%i", filename, size);
    string fontKey = string(buf);

    FontIter result = fonts.find(fontKey);

    if (result != fonts.end())
        return result->second;

    FTFont* font = NULL;
    font = new FTTextureFont(filename);

    if (font == NULL) {
        cerr << "Impossible d'ouvrir la police" << endl;
        delete font;
        return NULL;
        }

    if (!font->FaceSize(size)) {
        cerr << "Impossible de mettre la bonne taille" << endl;
        delete font;
        return NULL;
        }

    fonts[fontKey] = font;

    return font;
    }

void Text::Render(const char* str, FTFont* font, float R, float G, float B) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(R, G, B);
    font->Render(str);
    glPopAttrib();
    }
