#pragma once
#include <string>
#include <GL/glew.h>
namespace ge {

/// <summary>
/// To combine and compile the shaders files
/// </summary>
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertShaderFPath,
			const std::string& fragShaderFPath);
		void linkShaders();

		void addAttribute(const std::string& attrName);

		GLint getUniformLocation(const std::string& uniformName);

		/// <summary>
		/// To bind the color program
		/// </summary>
		void use();
		/// <summary>
		/// to unbind the program
		/// </summary>
		void unuse();

	private:
		int numAttributes;

		void compileShader(const std::string filePath, GLuint id);

		GLuint programID;
		GLuint vertShaderID;
		GLuint fragShaderID;
	};



}