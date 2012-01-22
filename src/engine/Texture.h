#pragma once

#include "ProjIncludes.h"

namespace TGA
{
	// A texture with ID, filename, width, and height
	class Texture
	{
		friend class TextureManager;

	public:
		Texture();
		Texture(std::string fileName);
		~Texture();

		// Load texture from an image
		bool loadTexture(std::string imgFileName);

		// Delete texture
		void deleteMe();

		// Reset the texture
		void reset();

		// Draw an entire texture
		void draw(GLfloat xPos, GLfloat yPos);

		// Draw a section of a texture
		void drawSection(GLfloat xPos, GLfloat yPos, SDL_Rect section);

		// Draw a section of a texture
		void drawSection(GLfloat xPos, GLfloat yPos, int sectX, int sectY,
			int sectWidth, int sectHeight);

		GLsizei getWidth();

		GLsizei getHeight();

	private:
		GLuint texture;
		std::string fileName;
		GLsizei width;
		GLsizei height;
	};
}