#pragma once
#include <string>

namespace ge {

#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
					x;\
					ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else 
#define GLCall(x) x
#endif // _DEBUG

	/// <summary>
	/// Trowing error and ending the program
	/// <param name="errorString">Error message</param>
	/// </summary>
	extern void fatalError(std::string errorString);

	extern void GLClearError();

	extern bool GLLogCall(const char* function, const char* file, int line);
}
