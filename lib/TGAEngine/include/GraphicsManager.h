#pragma once
#include "ProjIncludes.h"

namespace TGA
{
	class GraphicsManager
	{
	public:
		GraphicsManager();
		~GraphicsManager();

		void init(int screenWidth = 1024, int screenHeight = 512, 
		 std::string winTitle = "Sandstorms");

		void shutDown();

		void swapBuffers();

		void resizeWindow(int screenWidth, int screenHeight);
		void toggleFullScreen();

	private:
		void initGL();

		int screenWidth;
		int screenHeight;

		bool isLoaded;
		SDL_bool isFullScreen;

		SDL_Window* window;
		SDL_GLContext glCtx;
	};
}


