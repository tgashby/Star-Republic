#pragma once

#include "Singleton.h"

#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL_image/SDL_image.h>
#endif

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
