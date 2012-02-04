#include "Texture.h"
#include "TextureManager.h"

Texture::Texture()
{
	// Initialize all members with default values
	texture = 0;
	fileName = "";
	width = 0;
	height = 0;
}

Texture::Texture(std::string imgFileName)
{
	fileName = imgFileName;
	loadTexture(imgFileName);
}

Texture::~Texture()
{
}

bool Texture::loadTexture(std::string imgFileName)
{
	SDL_Surface* image;
	try {
		image = IMG_Load(imgFileName.c_str());
	} catch(...) {
		std::cout << "Error:" << SDL_GetError() << std::endl;
		return false;
	}

	if(image == NULL)
	{
		std::cout << "Error:" << SDL_GetError() << std::endl;
		return false;
	}

	// Format the image to be more usable with OGL
	SDL_DisplayFormatAlpha(image);

	fileName = imgFileName;

	width = image->w;
	height = image->h;

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

	SDL_FreeSurface(image);

	Singleton<TextureManager>::GetSingletonPtr()->addTexture(this);
	return true;
}

void Texture::deleteMe()
{
	Singleton<TextureManager>::GetSingletonPtr()->removeTexture(this);
}

void Texture::reset()
{
	loadTexture(fileName);
}

void Texture::draw(GLfloat xPos, GLfloat yPos)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(xPos, yPos);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(xPos + width, yPos);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(xPos + width, yPos + height);
			
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(xPos, yPos + height);

	glEnd();

	glPopMatrix();
}

void Texture::drawSection(GLfloat xPos, GLfloat yPos, SDL_Rect section)
{
	drawSection(xPos, yPos, section.x, section.y, section.w, section.h);
}

void Texture::drawSection(GLfloat xPos, GLfloat yPos, int sectX, int sectY, int sectWidth, int sectHeight)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
		
	glLoadIdentity();
		
	glScalef(1/(GLfloat)width, 1/(GLfloat)height, 1);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);

		glTexCoord2f((GLfloat)sectX, (GLfloat)sectY);
		glVertex2f(xPos, yPos);

		glTexCoord2f((GLfloat)(sectX + sectWidth), (GLfloat)sectY);
		glVertex2f(xPos + sectWidth, yPos);

		glTexCoord2f((GLfloat)(sectX + sectWidth), (GLfloat)(sectY + sectHeight));
		glVertex2f(xPos + sectWidth, yPos + sectHeight);

		glTexCoord2f((GLfloat)sectX, (GLfloat)(sectY + sectHeight));
		glVertex2f(xPos, yPos + sectHeight);

	glEnd();

	glPopMatrix();
}

GLsizei Texture::getWidth()
{
	return width;
}

GLsizei Texture::getHeight()
{
	return height;
}
