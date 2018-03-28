#include "IOManager.h"
#include <fstream>

namespace ge {
	bool IOManager::readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail()) {
			perror(filePath.c_str());
			return false;
		}

		// seek to the end of the file
		file.seekg(0, std::ios::end);

		// getting hte file size
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// on the top of every file there is a file header
		// we dont want it, so we need to trow it away
		fileSize -= file.tellg();

		buffer.resize((unsigned int)fileSize);
		file.read((char *)&(buffer[0]), fileSize);
		file.close();
		return true;
	}


	bool IOManager::readFileToBuffer(const std::string& filePath, std::string & buffer)
	{
		std::vector<unsigned char> v;
		if (readFileToBuffer(filePath, v)) {
			buffer = std::string(v.begin(), v.end());
			return true;
		}
		return false;
	}
}
