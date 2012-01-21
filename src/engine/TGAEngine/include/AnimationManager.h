#pragma once

#include "../include/ProjIncludes.h"
#include "../include/Animation.h"

namespace TGA
{
	class AnimationManager
	{
		friend class Animation;

	public:
		AnimationManager();
		~AnimationManager();

		// Remove all animations
		void removeAll();
				
		// Pause all animations
		void pauseAll();

		// Resume all animations
		void resumeAll();
		
		// Update all animations
		void updateAll();

		// Reset all animations
		void resetAll();

	protected:
		// Add animation to be managed
		void addAnimation(Animation* animation);

		// Remove an animation from being managed
		void removeAnimation(Animation* animation);

		// The animations and their IDs
		std::vector<Animation*> animations;
	};
}