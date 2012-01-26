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

#include "Turret.h"
#include <stdio.h>


Turret::Turret(int xloc, int zloc, Player* toAimAt)
{
   health = 100;
   Translation.X = xloc;
   Translation.Y = 0;
   Translation.Z = zloc;

   aim = toAimAt;
   
   Rotation.X = 0;
   Rotation.Y = 0;
   Rotation.Z = 0;
   
   mod = NULL;
   
   // First create a shader loader and check if our hardware supports shaders
	CShaderLoader ShaderLoader;
	if (! ShaderLoader.isValid())
	{
		std::cerr << "Shaders are not supported by your graphics hardware, or the shader loader was otherwise unable to load." << std::endl;
		//waitForUser3();
	}
   
   // Now attempt to load the shaders
	shade = ShaderLoader.loadShader("Shaders/GameVert2.glsl", "Shaders/Lab3_frag.glsl");
	if (! shade)
	{
		std::cerr << "Unable to open or compile necessary shader." << std::endl;
		//waitForUser3();
	}
	shade->loadAttribute("aPosition");
	shade->loadAttribute("aColor");
   shade->loadAttribute("aNormal");
   
   // Attempt to load mesh
   mod = CMeshLoader::loadASCIIMesh("Models/turretbase.obj");
	if (! mod)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		//waitForUser3();
	}
	// Make out mesh fit within camera view
	mod->resizeMesh(SVector3(1));
	// And center it at the origin
	mod->centerMeshByExtents(SVector3(0));
   
   // Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* mod, TriangleCount, 
         PositionBufferHandle1, ColorBufferHandle1, NormalBufferHandle1);
   
   // Attempt to load mesh
   mod = CMeshLoader::loadASCIIMesh("Models/turretmiddle.obj");
	if (! mod)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		//waitForUser3();
	}
	// Make out mesh fit within camera view
	mod->resizeMesh(SVector3(1));
	// And center it at the origin
	mod->centerMeshByExtents(SVector3(0));
   
   // Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* mod, TriangleCount, 
         PositionBufferHandle2, ColorBufferHandle2, NormalBufferHandle2);
   
   // Attempt to load mesh
   mod = CMeshLoader::loadASCIIMesh("Models/turrethead.obj");
	if (! mod)
	{
		std::cerr << "Unable to load necessary mesh." << std::endl;
		//waitForUser3();
	}
	// Make out mesh fit within camera view
	mod->resizeMesh(SVector3(1));
	// And center it at the origin
	mod->centerMeshByExtents(SVector3(0));
   
   // Now load our mesh into a VBO, retrieving the number of triangles and the handles to each VBO
	CMeshLoader::createVertexBufferObject(* mod, TriangleCount, 
        PositionBufferHandle3, ColorBufferHandle3, NormalBufferHandle3);
}
Turret::~Turret()
{

}
void Turret::update(float dt)
{
   SVector3* d = new SVector3(0,0,0);
   d->X = Translation.X - aim->getPosition()->X;
   d->Y = Translation.Y - aim->getPosition()->Y;
   d->Z = Translation.Z - aim->getPosition()->Z;
   

   //Inclienation?
   if(aim->getPosition()->Y > 0){    
      Rotation.X = (atan(sqrt(d->X * d->X + d->Z * d->Z) / d->Y) * 180 / 3.1415926); 
   }
   else {
      Rotation.X = (-atan(sqrt(d->X * d->X + d->Z * d->Z) / d->Y) * 180 / 3.1415926); 
   }
      
   //Rotation?
   Rotation.Y = (atan(d->X / d->Z) * 180 / 3.1415926); 
  
   //Rotation.Z = (cos(d->Z) * 180 / 3.1415926); 
}
void Turret::draw()
{
   if (health > 0)
   {
      // Shader context works by cleaning up the shader settings once it
		// goes out of scope
		CShaderContext ShaderContext(*shade);
		ShaderContext.bindBuffer("aPosition", PositionBufferHandle1, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle1, 3);
		ShaderContext.bindBuffer("aNormal", NormalBufferHandle1, 3);

      glPushMatrix();

      glTranslatef(Translation.X, Translation.Y, Translation.Z);
      glScalef(2, 2, 2);
      glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
      glTranslatef(0, 0.35, 0);
      glRotatef(Rotation.Y, 0, 1, 0);
      
      ShaderContext.bindBuffer("aPosition", PositionBufferHandle2, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle2, 3);
		ShaderContext.bindBuffer("aNormal", NormalBufferHandle2, 3);
      glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3);
      
      glTranslatef(0, 0.3, 0);
      glRotatef(90, 1, 0, 0);
      glRotatef(Rotation.X, 1, 0, 0);
      
      ShaderContext.bindBuffer("aPosition", PositionBufferHandle3, 4);
		ShaderContext.bindBuffer("aColor", ColorBufferHandle3, 3);
		ShaderContext.bindBuffer("aNormal", NormalBufferHandle3, 3);
      glDrawArrays(GL_TRIANGLES, 0, TriangleCount*3); 

      glPopMatrix();
   }
}
void Turret::tryToShoot()
{

}
float Turret::getSize()
{
   return 1.5;
}
void Turret::collideWith(Player* p)
{
   if (p->getTranslation()->Y < -0.8)
   { 
      float x = p->getTranslation()->X + 6;
      if (x - p->getSize() < Translation.X + getSize()  && x + p->getSize() > Translation.X - getSize())
      {
         float z = p->getTranslation()->Z + 3;
         if (z - p->getSize() < Translation.Z + getSize()  && z + p->getSize() > Translation.Z - getSize())
         {
            p->health -= 20;
            this->health = 0;
         }
      }
   }
}
void Turret::collideWith(Bullet* b)
{

}
void Turret::collideWithBullet(int damage)
{
   this->health -= damage*10;
   if (this-> health < 0) {
      this->health = 0;
   }
}
