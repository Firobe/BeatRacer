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
#define GLM_FORCE_RADIANS
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/type_ptr.hpp"
#include "../libs/glm/gtc/quaternion.hpp"
#include "../libs/glm/gtx/quaternion.hpp"

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
void gameLoop(Video&, Audio&, Track&);
void editorLoop(Video&, Audio&, Track&);

#endif
