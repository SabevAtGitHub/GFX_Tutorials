#pragma once
#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>

namespace ge {

	/// <summary>
	/// To combine and compile the shaders files
	/// </summary>
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		// Compile from extern shader file
		void compileShadersFromFile(const std::string& vertShaderFPath, const std::string& fragShaderFPath);

		// Compile from source code
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource,
			const std::string & vertShaderFPath = "Vertex Shader", 
			const std::string & fragShaderFPath = "Fragment Shader");

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

		void dispose();

	private:
		int m_numAttributes;

		void compileShader(const char* source, const std::string& name, GLuint id);

		GLuint m_programID;
		GLuint m_vertShaderID;
		GLuint m_fragShaderID;
	};



}