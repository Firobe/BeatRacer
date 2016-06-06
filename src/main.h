#ifndef MAIN__H
#define MAIN__H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>
#include <exception>
#include <stdio.h>
#include <AntTweakBar.h>

#include "map.h"
#include "audio.h"
#include "ship.h"
#include "video.h"
#include "notehandler.h"
#include "keymanager.h"
#include "lifebar.h"
#include "text.h"

enum Axes { xAxis, yAxis, zAxis };
void gameLoop(Video&, Audio&, std::string);
void editorLoop(Video&, Audio&, std::string);

#endif
