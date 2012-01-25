#ifndef _CMESHLOADER_H_INCLUDED_
#define _CMESHLOADER_H_INCLUDED_

#include <string>

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

#include "CMesh.h"


class CMeshLoader
{

public:

	/*!
	 * Attempts to load a mesh from an ascii .m file
	 * \return A pointer to a mesh object or NULL if the mesh failed to load
	 */
	static CMesh * const loadASCIIMesh(std::string const & fileName);

	/*!
	 * Generates a VBO from a given mesh file.
	 * \param Mesh The mesh to generate the VBO from.
	 * \param TriangleCount Returns the number of triangles to render from the VBO
	 * \param PositionBufferHandle Returns a handle to the position buffer object
	 * \param ColorBufferHandle Returns a handle to the color buffer object
	 */
	static void createVertexBufferObject(CMesh const & Mesh, int & TriangleCount, GLuint & PositionBufferHandle, GLuint & ColorBufferHandle, GLuint & NormalBufferHandle);

};

#endif
