#include "CMeshLoader.h"

#include <iostream>
#include <fstream>
#include <sstream>

//#include "MeshParser.h"

CMesh * const CMeshLoader::loadASCIIMesh(std::string const & fileName)
{
   
   MeshData *meshData = loadMesh(fileName, LOAD_NORMAL_FACE, 1.0);
   CMesh *Mesh = new CMesh();
   
   for (int i = 0; i < meshData->vertexCount; ++i) {
      SVector3 Position;
      Position.X = meshData->vertices[i*11];
      Position.Y = meshData->vertices[i*11+1];
      Position.Z = meshData->vertices[i*11+2];
      
      SVertex Vertex;
      Vertex.Position = Position;
      
      Mesh->Vertices.push_back(Vertex);
   }
   
   for (int i = 0; i < meshData->indexCount;) {
      CMesh::STriangle Triangle;
      Triangle.VertexIndex1 = meshData->indices[i++];
      Triangle.VertexIndex2 = meshData->indices[i++];
      Triangle.VertexIndex3 = meshData->indices[i++];
      
      Mesh->Triangles.push_back(Triangle);
   }
   
   return Mesh;
   //return 0;
}
   /*
	std::ifstream File;
   std::cout << fileName << std::endl;
	File.open(fileName.c_str());

	if (! File.is_open())
	{
		std::cerr << "Unable to open mesh file: " << fileName << std::endl;
		return 0;
	}

	CMesh * Mesh = new CMesh();

	while (File)
	{
		std::string ReadString;
		std::getline(File, ReadString);

		std::stringstream Stream(ReadString);

		std::string Label;
		Stream >> Label;

		if (Label.find("#") != std::string::npos)
		{
			// Comment, skip
			continue;
		}

		if ("v " == Label)
		{
			int Index;
			//Stream >> Index; // We don't care, throw it away

			SVector3 Position;
			Stream >> Position.X;
			Stream >> Position.Y;
			Stream >> Position.Z;

			SVertex Vertex;
			Vertex.Position = Position;

			Mesh->Vertices.push_back(Vertex);
		}
		else if ("Face" == Label)
		{
			int Index;
			Stream >> Index; // We don't care, throw it away

			int Vertex1, Vertex2, Vertex3;
			Stream >> Vertex1;
			Stream >> Vertex2;
			Stream >> Vertex3;

			CMesh::STriangle Triangle;
			Triangle.VertexIndex1 = Vertex1 - 1;
			Triangle.VertexIndex2 = Vertex2 - 1;
			Triangle.VertexIndex3 = Vertex3 - 1;

			size_t Location;
			if ((Location = ReadString.find("{")) != std::string::npos) // there is a color
			{
				Location = ReadString.find("rgb=(");
				Location += 5; // rgb=( is 5 characters

				ReadString = ReadString.substr(Location);
				std::stringstream Stream(ReadString);
				float Color;
				Stream >> Color;
				Triangle.Color.Red = Color;
				Stream >> Color;
				Triangle.Color.Green = Color;
				Stream >> Color;
				Triangle.Color.Blue = Color;
			}

			Mesh->Triangles.push_back(Triangle);
		}
		else if ("" == Label)
		{
			// Just a new line, carry on...
		}
		else if ("Corner" == Label)
		{
			// We're not doing any normal calculations... (oops!)
		}
		else
		{
			std::cerr << "While parsing ASCII mesh: Expected 'Vertex' or 'Face' label, found '" << Label << "'." << std::endl;
		}
	}

	if (! Mesh->Triangles.size() || ! Mesh->Vertices.size())
	{
		delete Mesh;
		return 0;
	}

	return Mesh;
}*/

void CMeshLoader::createVertexBufferObject(CMesh const & Mesh, int & TriangleCount, GLuint & PositionBufferHandle, GLuint & ColorBufferHandle, GLuint & NormalBufferHandle)
{
    GLfloat out[3], d;
    glGenBuffers(1, & PositionBufferHandle);
	glGenBuffers(1, & ColorBufferHandle);
           glGenBuffers(1, & NormalBufferHandle);

	std::vector<GLfloat> VertexPositions;
	std::vector<GLfloat> Colors;
        std::vector<GLfloat> Normals;

	for(unsigned int j = 0; j < Mesh.Triangles.size(); j++) 
	{
        SVertex Vertex1, Vertex2, Vertex3;
        SVector3 newNormal;
        GLfloat v1[3], v2[3];


        Vertex1 = Mesh.Vertices[Mesh.Triangles[j].VertexIndex1];

        VertexPositions.push_back(Vertex1.Position.X);
        VertexPositions.push_back(Vertex1.Position.Y);
        VertexPositions.push_back(Vertex1.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);


		Vertex2 = Mesh.Vertices[Mesh.Triangles[j].VertexIndex2];

        VertexPositions.push_back(Vertex2.Position.X);
        VertexPositions.push_back(Vertex2.Position.Y);
        VertexPositions.push_back(Vertex2.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);

       v1[0] = Vertex2.Position.X - Vertex1.Position.X;
       v1[1] = Vertex2.Position.Y - Vertex1.Position.Y;
       v1[2] = Vertex2.Position.Z - Vertex1.Position.Z;


		Vertex3 = Mesh.Vertices[Mesh.Triangles[j].VertexIndex3];

        VertexPositions.push_back(Vertex3.Position.X);
        VertexPositions.push_back(Vertex3.Position.Y);
        VertexPositions.push_back(Vertex3.Position.Z);
		VertexPositions.push_back(1.f);
		Colors.push_back(Mesh.Triangles[j].Color.Red);
		Colors.push_back(Mesh.Triangles[j].Color.Green);
		Colors.push_back(Mesh.Triangles[j].Color.Blue);

       v2[0] = Vertex3.Position.X - Vertex1.Position.X;
       v2[1] = Vertex3.Position.Y - Vertex1.Position.Y;
       v2[2] = Vertex3.Position.Z - Vertex1.Position.Z;        
        
        //LOOK! It's normal calculation time! (Should probably make a function in the future)
     out[0] = v1[1]*v2[2] - v1[2]*v2[1];
     out[1] = v1[2]*v2[0] - v1[0]*v2[2];
     out[2] = v1[0]*v2[1] - v1[1]*v2[0];
     d = sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
     
     out[0] = out[0]/d; out[1] = out[1]/d; out[2] = out[2]/d;
     newNormal.X = out[0];
     newNormal.Y = out[1];
     newNormal.Z = out[2];
     for (int i = 0; i < 3; i++) {
        Normals.push_back(newNormal.X);
        Normals.push_back(newNormal.Y);
        Normals.push_back(newNormal.Z);
     }
	}

    TriangleCount = Mesh.Triangles.size();

	glBindBuffer(GL_ARRAY_BUFFER, PositionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, VertexPositions.size()*sizeof(GLfloat), & VertexPositions.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, Colors.size()*sizeof(GLfloat), & Colors.front(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, NormalBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, Normals.size()*sizeof(GLfloat), & Normals.front(), GL_STATIC_DRAW);
}
