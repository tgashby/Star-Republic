#pragma once

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#ifdef __unix__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#include <GL\glew.h>
#endif