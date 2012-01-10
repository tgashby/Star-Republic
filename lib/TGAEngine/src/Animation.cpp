#include "../include/Animation.h"
#include "../include/AnimationManager.h"

namespace TGA
{
	Animation::Animation()
	{
		texture = NULL;

		currFrame = 0;
		lastUpdate = SDL_GetPerformanceCounter();
		paused = false;
		done = false;
		repetitions = 0;

		Singleton<AnimationManager>::GetSingletonPtr()->addAnimation(this);
	}

	Animation::Animation(Texture* texture)
	{
		this->texture = texture;

		currFrame = 0;
		lastUpdate = SDL_GetPerformanceCounter();
		paused = false;
		done = false;
		repetitions = 0;

		Singleton<AnimationManager>::GetSingletonPtr()->addAnimation(this);
	}

	Animation::~Animation()	{}

	void Animation::update()
	{
		if(texture != NULL && !paused && !done && frames.size() > 0)	
		{
			// IF enough time has passed
			if(frames.at(currFrame).second < (SDL_GetPerformanceCounter() - lastUpdate))
			{
				currFrame++;

				lastUpdate = SDL_GetPerformanceCounter();

				if(currFrame == frames.size())
				{
					// IF it is not running indefinitely
					if(repetitions != -1)
					{
						if(repetitions == 0)
						{
							done = true;
							paused = true;
						}
						else 
						{
							repetitions--;
						}
					}

					currFrame = 0;
				}
			}
		}
	}

	void Animation::pause()
	{
		paused = true;
	}
	
	void Animation::resume()
	{
		paused = false;

		lastUpdate = SDL_GetPerformanceCounter();
	}

	void Animation::reset()
	{
		currFrame = 0;

		done = false;
		paused = false;

		lastUpdate = SDL_GetPerformanceCounter();
	}

	bool Animation::isDone()
	{
		return done;
	}

	bool Animation::isPaused()
	{
		return paused;
	}

	void Animation::deleteMe()
	{
		Singleton<AnimationManager>::GetSingletonPtr()->removeAnimation(this);
	}

	void Animation::addFrame(SDL_Rect frameRect, Uint32 delay)
	{
		frames.push_back(std::make_pair(frameRect, delay));
	}

	void Animation::deleteFrame(GLuint frameNum)
	{
		if(frameNum < frames.size())
		{
			frames.erase(frames.begin() + frameNum);
		}
	}

	void Animation::clearFrames()
	{
		frames.clear();
	}

	void Animation::goToFrame(GLuint frame)
	{
		if(frame < frames.size())
		{
			currFrame = frame;
			
			lastUpdate = SDL_GetPerformanceCounter();
		}
	}

	void Animation::setRepetitions(int repetitions)
	{
		this->repetitions = repetitions;
	}

	void Animation::setTexture(Texture* texture)
	{
		delete this->texture;

		this->texture = texture;
	}

	void Animation::setDelay(GLuint frame, Uint32 delay)
	{
		if(frame < frames.size())
		{
			frames.at(frame).second = delay;
		}
	}
	
	void Animation::setFrameBounds(GLuint frame, SDL_Rect newBounds)
	{
		if(frame < frames.size())
		{
			frames.at(frame).first = newBounds;
		}
	}

	void Animation::draw(GLfloat xPos, GLfloat yPos)
	{
		// IF the texture exists
		if(texture != NULL && frames.size() > 0)
		{
			SDL_Rect tempRect = frames.at(currFrame).first;

			texture->drawSection(xPos, yPos, tempRect.x, tempRect.y, tempRect.w, tempRect.h);
		}
	}

	GLuint Animation::getFrameCount()
	{
		return frames.size();
	}

	std::string Animation::printFrames()
	{
		std::stringstream frameStr;

		for(unsigned int ndx = 0; ndx < frames.size(); ndx++)
		{
			frameStr << "Frame: " << ndx << "\n"
				 << "X: " << frames.at(ndx).first.x << "  Y: " << frames.at(ndx).first.y << "\n" 
				 << "Width: " << frames.at(ndx).first.w << "  Height: " << frames.at(ndx).first.h << "\n"
				 << "Delay: " << frames.at(ndx).second << "\n";
		}

		return frameStr.str();
	}

}
