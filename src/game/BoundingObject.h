/* 
 * BoundingObject - Define a hitbox for an object or portion of one
 */
#include <typeinfo>

class BoundingObject {
   public:
      virtual bool collidesWith(BoundingObject &other) = 0;
};
