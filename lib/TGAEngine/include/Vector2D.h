//
//  Vector2D.h
//  476 Game
//
//  Created by Taggart Ashby on 1/15/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _76_Game_Vector2D_h
#define _76_Game_Vector2D_h

namespace TGA {
   class Vector2D
   {
   public:
      Vector2D(float, float);
      ~Vector2D();
      
      Vector2D operator+(Vector2D);
      Vector2D operator-(Vector2D);
      Vector2D scalarProduct(int);
      Vector2D normalize();
      float dotProduct(Vector2D);
      float length();
      float distanceFrom(Vector2D);
      float getX();
      float getY();
      
   private:
      float x, y;
   };
}

#endif
