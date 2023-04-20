#pragma once
#include <vector>
#include <string>

namespace ge {
	/// <summary>
	/// Calling static function to read to buffer char vector
	/// Currently used only by loadPNG (picoPNG)
	/// </summary>
	class IOManager
	{
	public:
		// loading PNG file to char vector
		static bool readFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);

		// reading file to string buffer
		static bool readFileToBuffer(const std::string& filePath, std::string& buffer);
	};
}

