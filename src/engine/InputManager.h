#pragma once

#include "Key.h"

namespace TGA
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		// Determine if a given key is down
		bool keyDown(Key key);

		// Update the timers and key states
		bool update();

		SDL_Point getMouseCoords();

	private:
		std::map<int, bool> keystates;
		int mouseX;
		int mouseY;
	};
}