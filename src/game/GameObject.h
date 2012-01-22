/**
 * Game Object - A largely-abstract class to represent game objects
 *
 * @author Andrew J. Musselman
 */

class GameObject {
   pubic:
      virtual void tic(int dt) = 0;
      virtual void doCollision(GameObject & other) = 0;
      virtual Model getModel() = 0;
      virtual BoundingObject getBoundingObject() = 0;
   private:
    
}
