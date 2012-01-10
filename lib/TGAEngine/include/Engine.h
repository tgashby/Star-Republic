#pragma once

#include "Singleton.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "AnimationManager.h"
#include "FontManager.h"
#include "Camera.h"
#include "GraphicsManager.h"
#include "InputManager.h"

namespace TGA
{
	class Engine
	{
	public:
		Engine(void);
		~Engine(void);

		void Initalize();
		void Shutdown();

		SoundManager* Sounds;
		AnimationManager* Animations;
		TextureManager* Textures;
		FontManager* Fonts;
		Camera* GameCamera;
		GraphicsManager* Graphics;
		InputManager* Input;
	};
}

