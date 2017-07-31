#pragma once
#include <string>


namespace ge {
	/// <summary>
/// Trowing error and ending the program
/// <param name="errorString">Error message</param>
/// </summary>
	extern void fatalError(std::string errString);
}
