#pragma once

#include "../include/ProjIncludes.h"
#include "../include/TextureManager.h"
 
namespace TGA
{
	class Animation
	{
		friend class AnimationManager;

	public:
		Animation();
		Animation(Texture* texture);
		~Animation();

		// Update the animation
		void update();

		// Pause the animation
		void pause();

		// Resume a paused animation
		void resume();

		// Move the animation back to its first frame
		void reset();

		// Determine if the animation is done playing
		bool isDone();

		// Determine if the animation is paused
		bool isPaused();

		// Delete the animation
		void deleteMe();

		// Add a frame to the animation
		void addFrame(SDL_Rect frameRect, Uint32 delay);

		// Delete a frame
		void deleteFrame(GLuint frameNum);

		// Clear all frames
		void clearFrames();

		// Go to a specified frame
		void goToFrame(GLuint frame);

		// Set how often this animation repeats (-1 for indefinitely)
		void setRepetitions(int repetitions);

		// Set the texture of a particular frame
		void setTexture(Texture* texture);

		// Set the delay of the specified frame
		void setDelay(GLuint frame, Uint32 delay);

		// Change the frame's bounds
		void setFrameBounds(GLuint frame, SDL_Rect newBounds);

		// Draw the animation at a location
		void draw(GLfloat xPos, GLfloat yPos);

		GLuint getFrameCount();

		std::string printFrames();

	private:
		// The texture panel that has all the frames
		Texture* texture;

		// The individual frames, with delays
		std::vector<std::pair<SDL_Rect, Uint32> > frames;

		GLuint currFrame;

		// The last time this animation was updated
		Uint64 lastUpdate;

		bool paused;
		bool done;
		int repetitions;
	};
}