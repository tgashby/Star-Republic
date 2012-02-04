#pragma once

#include "ProjIncludes.h"

// A texture with ID, filename, width, and height
class Texture
{
	friend class TextureManager;

public:
	Texture();
	Texture(std::string fileName);
	~Texture();
      
   /**
      Load an image from a file.
      @param imgFileName the file name of the texture
      @return True if successful, false if any failures occur
      */
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
