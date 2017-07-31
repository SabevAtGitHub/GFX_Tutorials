#pragma once
#include "GLTexture.h"
#include "PicoPNG.h"

#include <string>

namespace ge {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);

	};
}

