//
//  main.cpp
//
//  Created by Taggart Ashby on 1/10/12.
//

/* SDL */
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <iostream>

int main (int argc, const char * argv[])
{
   SDL_Window* window;
   SDL_GLContext glCtx;
   
   // Call SDL_INIT(SDL_EVERYTHING)
   SDL_Init(SDL_INIT_EVERYTHING);
   
   // Set the SDL GL Attributes
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
   SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   
   // Create the window
   window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                             800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
   
   // Create the GL context
   glCtx = SDL_GL_CreateContext(window);
   
   // Boring OGL
   glPushMatrix();
   
   glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
   
   glPopMatrix();
   
   // Swap buffers to display what you've drawn
   SDL_GL_SwapWindow(window);
   
   SDL_Event myEvent;
   bool running = true;
   
   while(running)
   {
      SDL_PollEvent(&myEvent);
      
      if (myEvent.type == SDL_QUIT)
         running = false;
   }
   
   SDL_GL_DeleteContext(glCtx);
   SDL_DestroyWindow(window);
   SDL_Quit();
   return 0;
}

