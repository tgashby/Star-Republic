#include "CShader.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>


CShader::CShader()
{}

bool const CShader::loadAttribute(std::string const & label)
{
	GLuint AttributeHandle = glGetAttribLocation(Handle, label.c_str());

	if (AttributeHandle == GL_INVALID_OPERATION)
	{
		return false;
	}

	AttributeHandles[label] = AttributeHandle;
	return true;
}

CShaderLoader::CShaderLoader()
	: Valid(true)
{
	double const VersionNumber = std::atof((char const *)glGetString(GL_VERSION));
	if (VersionNumber < 2.0)
	{
		Valid = false;
	}

}

bool const CShaderLoader::isValid()
{
	return Valid;
}

static inline bool printOpenGLErrors()
{
	bool Succeeded = true;

	GLenum glErr = glGetError();
	while (glErr != GL_NO_ERROR)
    {
		Succeeded = false;
		glErr = glGetError();
    }

	return Succeeded;
}

static inline void printShaderInfoLog(GLuint shaderHandle)
{
	printOpenGLErrors();

	GLint infoLogLength = 0;
	glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, & infoLogLength);
	
	if (infoLogLength > 0)
    {
		GLchar * infoLog = new GLchar[infoLogLength];
		GLint charsWritten = 0;
		glGetShaderInfoLog(shaderHandle, infoLogLength, & charsWritten, infoLog);
		delete infoLog;
    }
}

static inline void printProgramInfoLog (GLuint programHandle)
{
	printOpenGLErrors();

	GLint infoLogLength = 0;
	glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, & infoLogLength);
	
	if (infoLogLength > 0)
    {
		GLchar * infoLog = new GLchar[infoLogLength];
		GLint charsWritten = 0;
		glGetProgramInfoLog(programHandle, infoLogLength, & charsWritten, infoLog);
		delete infoLog;
    }
}

CShader * const CShaderLoader::loadShader(std::string const & vertFileName)
{
	return 0;
}

CShader * const CShaderLoader::loadShader(std::string const & vertFileName, std::string const & fragFileName)
{
	// Create OpenGL Shader objects
	GLuint VS = glCreateShader(GL_VERTEX_SHADER);
	GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

	// Read Vertex Shader file
	{
		std::ifstream vertFile(vertFileName.c_str());
		if (! vertFile.is_open())
		{
			glDeleteShader(VS);
			glDeleteShader(FS);
			return 0;
		}
		std::string vertFileSource;

		vertFile.seekg(0, std::ios::end);   
		vertFileSource.reserve((size_t) vertFile.tellg());
		vertFile.seekg(0, std::ios::beg);

		vertFileSource.assign((std::istreambuf_iterator<char>(vertFile)), std::istreambuf_iterator<char>());
		GLchar const * SourceString = vertFileSource.c_str();
		glShaderSource(VS, 1, & SourceString, NULL);
	}
	

	// Read Fragment Shader file
	{
		std::ifstream fragFile(fragFileName.c_str());
		if (! fragFile.is_open())
		{
			glDeleteShader(VS);
			glDeleteShader(FS);
			return 0;
		}
		std::string fragFileSource;

		fragFile.seekg(0, std::ios::end);   
		fragFileSource.reserve((size_t) fragFile.tellg());
		fragFile.seekg(0, std::ios::beg);

		fragFileSource.assign((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
		GLchar const * SourceString = fragFileSource.c_str();
		glShaderSource(FS, 1, & SourceString, NULL);
	}
	

	// Compile Shader
	{
		glCompileShader(VS);
		GLint vCompiled;
		glGetShaderiv(VS, GL_COMPILE_STATUS, & vCompiled);
		printShaderInfoLog(VS);
		if (! vCompiled)
		{
			glDeleteShader(VS);
			glDeleteShader(FS);
			return 0;
		}
	}

	{
		glCompileShader(FS);
		GLint fCompiled;
		glGetShaderiv(FS, GL_COMPILE_STATUS, & fCompiled);
		printShaderInfoLog(FS);
		if (! fCompiled)
		{
			glDeleteShader(VS);
			glDeleteShader(FS);
			return 0;
		}
	}

	CShader * Shader = new CShader();

	// Create Shader program
	Shader->Handle = glCreateProgram();
	glAttachShader(Shader->Handle, VS);
	glAttachShader(Shader->Handle, FS);

	// Link program
	{
		GLint linked;
		glLinkProgram(Shader->Handle);
		glGetProgramiv(Shader->Handle, GL_LINK_STATUS, & linked);
		printProgramInfoLog(Shader->Handle);
	}


	return Shader;
}


CShaderContext::CShaderContext(CShader const & shader)
	: Shader(shader), Valid(true)
{
	glUseProgram(Shader.Handle);
}

CShaderContext::~CShaderContext()
{
	for (std::vector<GLuint>::const_iterator it = EnabledVertexAttribArrays.begin(); it != EnabledVertexAttribArrays.end(); ++ it)
		glDisableVertexAttribArray(* it);

	glUseProgram(0);
}

void CShaderContext::bindBuffer(std::string const & label, GLuint const BufferHandle, GLuint const ElementSize)
{
	std::map<std::string, GLuint>::const_iterator it = Shader.AttributeHandles.find(label);

	if (it == Shader.AttributeHandles.end())
	{
		Valid = false;
		return;
	}

	glEnableVertexAttribArray(it->second);
	glBindBuffer(GL_ARRAY_BUFFER, BufferHandle);
	glVertexAttribPointer(it->second, ElementSize, GL_FLOAT, GL_FALSE, 0, 0);

	EnabledVertexAttribArrays.push_back(it->second);
}
