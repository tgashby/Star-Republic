#include <iostream>

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


#include <stdio.h>

#include "Bullet.h"

void waitForUser4() 
{
	std::cout << "Press [Enter] to continue . . .";
	std::cin.get();
}

Bullet::Bullet(SVector3* pos, SVector3* vel, CMesh* mod, float size, int damage) : GameObject(pos, vel, mod) {

   this->damage = damage;
   this->size = size;
   toDie = false;
   ignore = false;
   Translation.X = pos->X;
	Translation.Y = pos->Y;
	Translation.Z = pos->Z;

        float scale = 0.3;
	Scale.X = scale; 
	Scale.Y = scale;
	Scale.Z = scale;

	Rotation.X = 0;
	Rotation.Y = 90;
	Rotation.Z = 0;

  // First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		waitForUser4();
	}

	// Now attempt to load the shaders
	shade = ShaderLoader.loadShader("Shaders/GameVert2.glsl", "Shaders/Lab3_frag.glsl");
	if (! shade)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		waitForUser4();
	}
	shade->loadAttribute("aPosition");
	shade->loadAttribute("aColor");
  shade->loadAttribute("aNormal");
	
	// Attempt to load mesh
	mod = CMeshLoader::loadASCIIMesh("Models/bullet.obj");
	if (! mod)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		waitForUser4();
	}
	// Make out mesh fit within camera view
	mod->resizeMesh(SVector3(1));
	// And center it at the origin
	mod->centerMeshByExtents(SVector3(0));

	// Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* mod, TriangleCount, 
		PositionBufferHandle, ColorBufferHandle, NormalBufferHandle);
        printf("\nLook ma! It's a bullet!\n");
}

Bullet::~Bullet() {
}

SVector3* Bullet::getPosition()
{
   return position;
}

bool Bullet::getIgnore()
{
    return ignore;
}

void Bullet::draw()
{
	{
                if (ignore == false) {
		// Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(*shade);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle, 3);
    		ShaderContext.bindBuffer("aNormal", NormalBufferHandle, 3);

		glPushMatrix();

		glTranslatef(-Translation.X + 6, Translation.Y + 4, Translation.Z);
		glRotatef(Rotation.Z, 0, 0, 1);
		glRotatef(Rotation.Y, 0, 1, 0);
		glRotatef(Rotation.X, 1, 0, 0);
		glScalef(Scale.X, Scale.Y, Scale.Z);

		glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);

		glPopMatrix();
                }
	}
}

void Bullet::collideWith(GameObject * collided)
{
   toDie = true;
}

void Bullet::nullify()
{
   ignore = true;
}

void Bullet::collisionCheck(Player* object)
{
   if (ignore == false) {
   fprintf(stderr, "\nSTAGE1\n");
   float temp = object->size/2 + this->size/2;
   SVector3* a = new SVector3();
   a = object->getPosition();
   SVector3* b = new SVector3();
   b = this->getPosition();
   float aX = a->X;
fprintf(stderr, "\nSTAGEA\n");
   float aY = a->Y;
fprintf(stderr, "\nSTAGEB\n");
   float aZ = a->Z;
fprintf(stderr, "\nSTAGEC\n");
   float bX = b->X;
fprintf(stderr, "\nSTAGED\n");
   float bY = b->Y;
fprintf(stderr, "\nSTAGEE\n");
   float bZ = b->Z;
fprintf(stderr, "\nSTAGEF\n");
   //float x2 = x1->X;
   //float x2 = second->getPosition()->X;
   fprintf(stderr, "\nSTAGE2\n");
   float distance = sqrt((aX - bX) * (aX - bX) + (aY - bY) * (aY - bY) + (aZ - bZ) * (aZ - bZ));
   //float distance = 1;
   fprintf(stderr, "\nSTAGE3\n");
   if (distance < temp) {
   fprintf(stderr, "\nSTAGE4\n");
      //first->collideWith((GameObject*)second);
   fprintf(stderr, "\nSTAGE5\n");
      //this->collideWith((GameObject*)object);
   fprintf(stderr, "\nSTAGE6\n");
   //return true;
   toDie = true;
   }
   }
   else {
        toDie = false;
   }
   //return false;
}


bool Bullet::gettoDie()
{
   return toDie;
}

void Bullet::update(float dt)
{
        if (ignore == false) {
        //printf("\nUpdate on a bullet!");
	position->X -= velocity->X * dt;
	position->Y -= velocity->Y * dt;
	position->Z += velocity->Z * dt;
	
	Rotation.X = velocity->X * 5.0;
	Rotation.Y = 90 + velocity->Y * 5.0;

	Translation.X = -position->X;
	Translation.Y = position->Y;
	Translation.Z = position->Z;
        }
}
