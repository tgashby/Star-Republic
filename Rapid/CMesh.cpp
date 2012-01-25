#include "CMesh.h"

CMesh::CMesh()
{}

CMesh::~CMesh()
{}

void CMesh::centerMeshByAverage(SVector3 const & CenterLocation)
{
	SVector3 VertexSum;
	for (std::vector<SVertex>::const_iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
		VertexSum += it->Position;

	VertexSum /= (float) Vertices.size();
	SVector3 VertexOffset = CenterLocation - VertexSum;
	for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
		it->Position += VertexOffset;
}

void CMesh::centerMeshByExtents(SVector3 const & CenterLocation)
{
	if (Vertices.size() < 2)
		return;

	SVector3 Min, Max;
	{
		std::vector<SVertex>::const_iterator it = Vertices.begin();
		Min = it->Position;
		Max = it->Position;
		for (; it != Vertices.end(); ++ it)
		{
			if (Min.X > it->Position.X)
				Min.X = it->Position.X;
			if (Min.Y > it->Position.Y)
				Min.Y = it->Position.Y;
			if (Min.Z > it->Position.Z)
				Min.Z = it->Position.Z;

			if (Max.X < it->Position.X)
				Max.X = it->Position.X;
			if (Max.Y < it->Position.Y)
				Max.Y = it->Position.Y;
			if (Max.Z < it->Position.Z)
				Max.Z = it->Position.Z;
		}
	}

	SVector3 Center = (Max + Min) / 2;

	SVector3 VertexOffset = CenterLocation - Center;
	for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
		it->Position += VertexOffset;
}

void CMesh::resizeMesh(SVector3 const & Scale)
{
	if (Vertices.size() < 2)
		return;

	SVector3 Min, Max;
	{
		std::vector<SVertex>::const_iterator it = Vertices.begin();
		Min = it->Position;
		Max = it->Position;
		for (; it != Vertices.end(); ++ it)
		{
			if (Min.X > it->Position.X)
				Min.X = it->Position.X;
			if (Min.Y > it->Position.Y)
				Min.Y = it->Position.Y;
			if (Min.Z > it->Position.Z)
				Min.Z = it->Position.Z;

			if (Max.X < it->Position.X)
				Max.X = it->Position.X;
			if (Max.Y < it->Position.Y)
				Max.Y = it->Position.Y;
			if (Max.Z < it->Position.Z)
				Max.Z = it->Position.Z;
		}
	}

	SVector3 Extent = (Max - Min);
	SVector3 Resize = Scale / std::max(Extent.X, std::max(Extent.Y, Extent.Z));
	for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
		it->Position *= Resize;
}
