#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace ge {
	TextureCache::TextureCache() { /* empty */ }

	TextureCache::~TextureCache() { /* empty */ }

	GLTexture TextureCache::getTexture(std::string filePath)
	{
		auto mit = m_textureMap.find(filePath);
		if (mit == m_textureMap.end())
		{
			// loading the texture and insert to the map
			GLTexture newTexture = ImageLoader::loadPNG(filePath);

			// adding new pair to the texture map
			m_textureMap.insert(std::make_pair(filePath, newTexture));

			//std::cout << "Created texture!\n";
			return newTexture;
		}

		//std::cout << " Existing texture!\n";
		return mit->second;
	}
}
