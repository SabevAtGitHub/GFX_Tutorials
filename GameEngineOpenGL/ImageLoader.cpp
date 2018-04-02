#include "ImageLoader.h"
#include "IOManager.h"
#include "ErrManager.h"

namespace ge {

	GLTexture ImageLoader::loadPNG( std::string filePath )
	{
		GLTexture m_texture = {};

		// file buffers
		std::vector<unsigned char> out;
		std::vector <unsigned char> in;

		unsigned long w, h;

		if (false == IOManager::readFileToBuffer(filePath, in)) {
			fatalError("ImageLoader: Failed to load PNG file to buffer!");
		}

		int errCode = decodePNG(out, w, h, &(in[0]), in.size());
		if (errCode) {
			fatalError("ImageLoader: Decode PNG failed with error: " + std::to_string(errCode));
		}

		// generating OpenGL texture object
		GLCall(glGenTextures(1, &(m_texture.id)));

		// binding the texture and uploading data to it
		GLCall(glBindTexture(GL_TEXTURE_2D, m_texture.id));

		// uploading image data to the texture
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0])));

		// setting some parameters about the texture
		// how it should be treated
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

		// To use mitmapping
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		// we must awlays unbind the textures
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		m_texture.w = w;
		m_texture.h = h;

		return m_texture;
	}
}