#pragma once
#include <vector>

namespace ge {
	/// <summary>
	/// Calling static function to read to buffer char vector
	/// Currently used only by loadPNG (picoPNG)
	/// </summary>
	class IOManager
	{
	public:
		// loading PNG file to char vector
		static bool readFileToBuffer(std::string filePath,
			std::vector<unsigned char>& buffer);
	};
}

