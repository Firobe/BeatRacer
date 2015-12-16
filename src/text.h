#ifndef TEXT__H
#define TEXT__H

#include <map>
#include <string>
#include <iostream>
#include <FTGL/ftgl.h>
#include <GLFW/glfw3.h>

typedef std::map<std::string, FTFont*> FontList;
typedef FontList::const_iterator FontIter;

class Text {
    public:
        static Text& Instance();
        ~Text();
        FTFont* getFont(const char*, int);
        static void Render(const char*, FTFont*, float = 1., float = 1., float = 1.);
    private:
        Text() {};
        Text(const Text&) {};
        Text& operator = (const Text&) {
            return *this;
            };
        FontList fonts;
    };

#endif
