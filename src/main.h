#ifndef MAIN__H
#define MAIN__H

#include <iostream>
#include <exception>
#include <sstream>
#include <AntTweakBar.h>
#include <iomanip>

#include "map.h"
#include "audio.h"
#include "ship.h"
#include "video.h"
#include "notehandler.h"
#include "keymanager.h"
#include "lifebar.h"
#include "text.h"
#include "track.h"

extern int screen_width;
extern int screen_height;
enum Axes { xAxis, yAxis, zAxis };
void gameLoop(Video&, Audio&, Track&, Map&);
void editorLoop(Video&, Audio&, Track&, Map&);

#endif
