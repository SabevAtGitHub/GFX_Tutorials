#pragma once

#include <string>

#include "GLTexture.h"
#include "TextureCache.h"


namespace ge {
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache textureCache;
	};
}

