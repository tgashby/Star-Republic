#include "../include/TextureManager.h"

namespace TGA
{
	TextureManager::TextureManager()
	{
		textures.clear();
	}

	TextureManager::~TextureManager()
	{
		removeAllTextures();
	}

	void TextureManager::resetTextures()
	{
		for(unsigned int ndx = 0; ndx < textures.size(); ndx++)
		{
			textures.at(ndx)->reset();
		}
	}

	void TextureManager::removeAllTextures()
	{
		for(unsigned int ndx = 0; ndx < textures.size(); ndx++)
		{
			textures.at(ndx)->deleteMe();
		}
	}

	void TextureManager::addTexture(Texture* texture)
	{
		std::vector<Texture*>::iterator itr = find(textures.begin(), textures.end(), texture);

		// IF the texture was not found
		if(itr == textures.end())
		{
			// Tack on the texture
			textures.push_back(texture);
		}
	}

	void TextureManager::removeTexture(Texture* texture)
	{
		std::vector<Texture*>::iterator itr = find(textures.begin(), textures.end(), texture);

		// IF the texture was found
		if(itr != textures.end())
		{
			if(itr == textures.end() - 1)
			{
				textures.erase(textures.end() - 1);
			}
			else
			{
				// Replace the texture with the last texture in the vector
				*itr = textures.at(textures.size() - 1);
				
				textures.erase(textures.end() - 1);
			}
		}

		delete texture;
	}
}