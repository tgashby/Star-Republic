#include "../include/AnimationManager.h"

namespace TGA
{
	AnimationManager::AnimationManager()
	{
		animations.clear();
	}

	AnimationManager::~AnimationManager()
	{
		removeAll();
	}

	void AnimationManager::removeAll()
	{
		for(unsigned int ndx = 0; ndx < animations.size(); ndx++)
		{
			animations.at(ndx)->deleteMe();
		}
	}

	void AnimationManager::pauseAll()
	{
		for(unsigned int ndx = 0; ndx < animations.size(); ndx++)
		{
			animations.at(ndx)->pause();
		}
	}

	void AnimationManager::resumeAll()
	{
		for(unsigned int ndx = 0; ndx < animations.size(); ndx++)
		{
			animations.at(ndx)->resume();
		}
	}
	
	void AnimationManager::updateAll()
	{
		for(unsigned int ndx = 0; ndx < animations.size(); ndx++)
		{
			animations.at(ndx)->update();
		}
	}

	void AnimationManager::resetAll()
	{
		for(unsigned int ndx = 0; ndx < animations.size(); ndx++)
		{
			animations.at(ndx)->reset();
		}
	}

	void AnimationManager::addAnimation(Animation* animation)
	{
		std::vector<Animation*>::iterator itr = find(animations.begin(), animations.end(), animation);

		// IF the texture was not found
		if(itr == animations.end())
		{
			// Tack on the texture
			animations.push_back(animation);
		}
		
	}

	void AnimationManager::removeAnimation(Animation* animation)
	{
		std::vector<Animation*>::iterator itr = find(animations.begin(), animations.end(), animation);

		// IF the animation was found
		if(itr != animations.end())
		{
			if(itr == animations.end() - 1)
			{
				animations.erase(animations.end() - 1);
			}
			else
			{
				*itr = animations.at(animations.size() - 1);

				animations.erase(animations.end() - 1);
			}
		}
	}
}