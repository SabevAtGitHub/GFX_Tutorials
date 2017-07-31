#include "GLSLProgram.h"
#include "ErrManager.h"
#include <fstream>
#include <vector>

namespace ge {

	GLSLProgram::GLSLProgram() : 
		numAttributes(0), 
		programID(0), 
		vertShaderID(0), 
		fragShaderID(0) 
	{ /* empty */ }
	
	GLSLProgram::~GLSLProgram() { /* empty */ }

	void GLSLProgram::compileShaders(const std::string & vertShaderFPath, const std::string & fragShaderFPath)
	{
		//Get a program object.
		programID = glCreateProgram();

		// create vertex shader
		vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (vertShaderID == 0) {
			fatalError("GLSLProgram: Vertex Shader creation failed!");
		}

		// create fragment shader
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (fragShaderID == 0) {
			fatalError("GLSLProgram: Fragment Shader creation failed!");
		}

		// compile them
		compileShader(vertShaderFPath, vertShaderID);
		compileShader(fragShaderFPath, fragShaderID);
	}

	void GLSLProgram::compileShader(const std::string filePath, GLuint id)
	{

		// load shader file
		std::ifstream vertFile(filePath);
		if (vertFile.fail()) {
			perror(filePath.c_str());
			fatalError("GLSLProgram: Failed to open " + filePath);
		}

		std::string fileContents = "";
		std::string line;

		// read file contents - line by line
		while (std::getline(vertFile, line)) {
			fileContents += line + "\n";
		}

		// close
		vertFile.close();

		// compile
		const char* contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);

		glCompileShader(id);

		// test if it is compiled
		GLint isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			// display error
			std::printf("%s\n", &(errorLog[0]));
			fatalError("GLSLProgram: Shader " + filePath + " failed to compile!");
		}
	}


	void GLSLProgram::linkShaders()
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.

		//Attach our shaders to our program
		glAttachShader(programID, vertShaderID);
		glAttachShader(programID, fragShaderID);

		//Link our program
		glLinkProgram(programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(programID);
			//Don't leak shaders either.
			glDeleteShader(vertShaderID);
			glDeleteShader(fragShaderID);

			// display error
			std::printf("%s\n", &(infoLog[0]));
			fatalError("GLSLProgram: Shaders failed to link!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(programID, vertShaderID);
		glDetachShader(programID, vertShaderID);
		glDeleteShader(vertShaderID);
		glDeleteShader(fragShaderID);
	}


	// adds an attribute to my shader.
	//Shold be called between compiling and linking
	void GLSLProgram::addAttribute(const std::string & attrName)
	{
		glBindAttribLocation(programID, numAttributes++, attrName.c_str());
	}

	// gets location in file for uniform variable
	GLint GLSLProgram::getUniformLocation(const std::string & uniformName)
	{

		GLint loc = glGetUniformLocation(programID, uniformName.c_str());
		if (loc == GL_INVALID_INDEX) {
			fatalError("GLSLProgram: Uniform " + uniformName + " not found in shader!");
		}
		return loc;
	}

	void GLSLProgram::use()
	{
		glUseProgram(programID);
		for (auto i = 0; i < numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (auto i = 0; i < numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}
}