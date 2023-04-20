#include "GLSLProgram.h"
#include <SDL2/SDL.h>
#include "ErrManager.h"
#include <fstream>
#include <vector>
#include "IOManager.h"

namespace ge {

	GLSLProgram::GLSLProgram() :
		m_numAttributes(0),
		m_programID(0),
		m_vertShaderID(0),
		m_fragShaderID(0)
	{ /* empty */
	}

	GLSLProgram::~GLSLProgram() { /* empty */ }

	void GLSLProgram::compileShadersFromFile(const std::string & vertShaderFPath, const std::string & fragShaderFPath)
	{
		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertShaderFPath, vertSource);
		IOManager::readFileToBuffer(fragShaderFPath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str(), vertShaderFPath, fragShaderFPath);

	}

	void GLSLProgram::compileShadersFromSource(const char * vertexSource, const char * fragmentSource,
		const std::string & vertShaderFPath/* = "Vertex Shader"*/, const std::string & fragShaderFPath/* = "Fragment Shader"*/)
	{
		//Get a program object.
		GLCall(m_programID = glCreateProgram());

		// create vertex shader
		GLCall(m_vertShaderID = glCreateShader(GL_VERTEX_SHADER));
		if (m_vertShaderID == 0) {
			fatalError("GLSLProgram: Vertex Shader creation failed!");
		}

		// create fragment shader
		GLCall(m_fragShaderID = glCreateShader(GL_FRAGMENT_SHADER));
		if (m_fragShaderID == 0) {
			fatalError("GLSLProgram: Fragment Shader creation failed!");
		}

		// compile them
		compileShader(vertexSource, vertShaderFPath, m_vertShaderID);
		compileShader(fragmentSource, fragShaderFPath, m_fragShaderID);
	}

	void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id)
	{
		// compile
		GLCall(glShaderSource(id, 1, &source, nullptr));

		GLCall(glCompileShader(id));

		// test if it is compiled
		GLint isCompiled = 0;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled));
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			GLCall(glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]));

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			GLCall(glDeleteShader(id)); // Don't leak the shader.

			// display error
			std::printf("%s\n", &(errorLog[0]));
			fatalError("GLSLProgram: Shader " + name + " failed to compile!");
		}
	}


	void GLSLProgram::linkShaders()
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.

		//Attach our shaders to our program
		GLCall(glAttachShader(m_programID, m_vertShaderID));
		GLCall(glAttachShader(m_programID, m_fragShaderID));

		//Link our program
		GLCall(glLinkProgram(m_programID));

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		GLCall(glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked));
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			GLCall(glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength));

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetProgramInfoLog(m_programID, maxLength, &maxLength, &infoLog[0]));

			//We don't need the program anymore.
			GLCall(glDeleteProgram(m_programID));
			//Don't leak shaders either.
			GLCall(glDeleteShader(m_vertShaderID));
			GLCall(glDeleteShader(m_fragShaderID));

			// display error
			std::printf("%s\n", &(infoLog[0]));
			fatalError("GLSLProgram: Shaders failed to link!");
		}

		//Always detach shaders after a successful link.
		GLCall(glDetachShader(m_programID, m_vertShaderID));
		GLCall(glDetachShader(m_programID, m_fragShaderID));
		GLCall(glDeleteShader(m_vertShaderID));
		GLCall(glDeleteShader(m_fragShaderID));
	}


	// adds an attribute to my shader.
	//Shold be called between compiling and linking
	void GLSLProgram::addAttribute(const std::string & attrName)
	{
		GLCall(glBindAttribLocation(m_programID, m_numAttributes++, attrName.c_str()));
	}

	// gets location in file for uniform variable
	GLint GLSLProgram::getUniformLocation(const std::string & uniformName)
	{

		GLCall(GLint loc = glGetUniformLocation(m_programID, uniformName.c_str()));
		if (loc == GL_INVALID_INDEX) {
			fatalError("GLSLProgram: Uniform " + uniformName + " not found in shader!");
		}
		return loc;
	}

	void GLSLProgram::use()
	{
		GLCall(glUseProgram(m_programID));
		for (auto i = 0; i < m_numAttributes; i++) {
			GLCall(glEnableVertexAttribArray(i));
		}
	}

	void GLSLProgram::unuse()
	{
		GLCall(glUseProgram(0));
		for (auto i = 0; i < m_numAttributes; i++) {
			GLCall(glDisableVertexAttribArray(i));
		}
	}

	void GLSLProgram::dispose()
	{
		if (m_programID){
			GLCall(glDeleteProgram(m_programID));
			m_programID = 0;
		}
	}
}