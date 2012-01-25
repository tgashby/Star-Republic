#ifndef _CMESH_H_INCLUDED_
#define _CMESH_H_INCLUDED_

#include <vector>

#include "Util/SVertex.h"


class CMesh
{

    friend class CVertexBufferObject;
    friend class CMeshLoader;

    struct STriangle
    {
        unsigned int VertexIndex1, VertexIndex2, VertexIndex3;
		SColor Color;
    };

    std::vector<SVertex> Vertices;
    std::vector<STriangle> Triangles;

    CMesh();

public:

    ~CMesh();

	void centerMeshByAverage(SVector3 const & CenterLocation);
	void centerMeshByExtents(SVector3 const & CenterLocation);

	void resizeMesh(SVector3 const & Scale);

};

#endif
