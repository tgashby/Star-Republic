#ifndef _CSHADER_H_INCLUDED_
#define _CSHADER_H_INCLUDED_

#include <string>
#include <map>
#include <vector>

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#include <GL\glut.h>
#endif


class CShader
{

	friend class CShaderLoader;
	friend class CShaderContext;

	GLuint Handle;
	std::map<std::string, GLuint> AttributeHandles;

	CShader();

public:

	/*!
	 * Queries the shader program for an attribute variable of the given name.
	 */
	bool const loadAttribute(std::string const & label);

};

class CShaderLoader
{

	bool Valid;

public:

	/*!
	 * Initializes the shader loader by checking that the graphic hardware can load shaders.
	 */
	CShaderLoader();

	/*!
	 * Check to see if our system can load shaders.
	 */
	bool const isValid();

	/*!
	 * Load a vertex shader.
	 */
	CShader * const loadShader(std::string const & vertFileName);

	/*!
	 * Load a vertex shader and fragment shader.
	 */
	CShader * const loadShader(std::string const & vertFileName, std::string const & fragFileName);

};

class CShaderContext
{

	bool Valid;
	CShader const & Shader;

	std::vector<GLuint> EnabledVertexAttribArrays;

public:

	/*!
	 * Creates a shader context, used for drawing an object with the shader
	 */
	CShaderContext(CShader const & shader);

	/*!
	 * Cleans up shader call... make sure that the CShaderContext leaves scope before another context is created!
	 */
	~CShaderContext();

	/*!
	 * Binds a shader attribute variable to a VBO.
	 */
	void bindBuffer(std::string const & label, GLuint const BufferHandle, GLuint const ElementSize);

};

#endif
