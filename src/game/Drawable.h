#ifndef DRAWABLE_H
#define DRAWABLE_H
/* An abstract class to define what methods a drawable class must 
 * implement. 
 *
 * @author Andrew J. Musselman
 */

class Drawable {
   public:
      virtual list<Model> getModels(const Coordinate translation) = 0;

   private:

   }

#endif
