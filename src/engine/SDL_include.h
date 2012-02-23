#pragma once

#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#endif

#ifdef __unix__
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <GL\glew.h>
#endif
