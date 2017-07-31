#include "ResourceManager.h"

namespace ge {
	TextureCache ResourceManager::textureCache;

	GLTexture ResourceManager::getTexture(std::string texturePath)
	{
		return textureCache.getTexture(texturePath);
	}
}
