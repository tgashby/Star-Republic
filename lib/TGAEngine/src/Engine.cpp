#include "Engine.h"

namespace TGA
{
	Engine::Engine(void)
	{
	}


	Engine::~Engine(void)
	{
	}

	void Engine::Initalize()
	{
		Sounds = Singleton<SoundManager>::GetSingletonPtr();
		Animations = Singleton<AnimationManager>::GetSingletonPtr();
		Textures = Singleton<TextureManager>::GetSingletonPtr();
		Fonts = Singleton<FontManager>::GetSingletonPtr();
		GameCamera = Singleton<Camera>::GetSingletonPtr();
		Graphics = Singleton<GraphicsManager>::GetSingletonPtr();
		Input = Singleton<InputManager>::GetSingletonPtr();
	}

	void Engine::Shutdown()
	{

	}

}
