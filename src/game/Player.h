#ifndef StarRepub_Player_h
#define StarRepub_Player_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../game/Flyer.h"
#include "../engine/Mesh.h"
#include <assert.h>
#include <cmath>
#define VELOCITY 0.04f
#define X_SCALAR 0.0005f
#define Y_SCALAR 0.0005f
#define MODEL_SCALE 0.8f
#define PLAYER_DISTANCE_FROM_CAMERA 170

/**
 * Player class to handle the player
 */
class Player : public Object3d, public Flyer {
public:
   /**
    * Player construct taking 2 strings representing the paths to the model
    * and texture files, a Modules pointer, and vectors representing the
    * camera state
    * @param fileName the path to the model file
    * @param textureName the path to the texture file
    * @param modules the Modules pointer
    * @param cam_pos the camera position
    * @param cam_up the camera's up vector
    * @param cam_forw the camera's forward vector
    */
   Player(string fileName, string textureName, Modules *modules, 
          Vector3<float> cam_pos, Vector3<float> cam_up, 
          Vector3<float> cam_forw);
   
   ~Player();
   
   /**
    * tic updates the player based on the time passed and the camera
    * @param time the time passed since the last update, in milliseconds
    * @param cam_position the current camera position
    * @param cam_up the current camera up vector
    * @param cam_forward the current camera forward vector
    */
   void tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward);
   
   /**
    * getSide returns the player's side vector
    */
   Vector3<float> getSide();
   
   /**
    * calculateSide determines the player's side vector
    */
   void calculateSide();
   
   /**
    * setVisible set's the player's visibility, used for determining if
    * we should draw the player
    */
   void setVisible(bool visibility);
   
   /**
    * doCollision determines the action that the player should take
    * based on what it has collided with
    * @param other the object being collided with
    */
   void doCollision(GameObject & other);
   
   /**
    * updateVelocity determines the player's new velocity in each direction
    * based on mouse input
    * @param diffX the difference in x
    * @param diffY the difference in y
    */
   void updateVelocity(float diffX, float diffY);
   
   /**
    * getAimForward returns where the player is aiming
    */
   Vector3<float> getAimForward();
   
   /**
    * getAimUp returns player's aim's up vector
    */
   Vector3<float> getAimUp();

private:
   float vx, vy; 
   float x, y;
   Vector3<float> xPosition, yPosition;
   Mesh *m_mesh;
   Vector3<float> m_shipVelocity;
   Vector3<float> m_sideVelocity;
   Vector3<float> m_upVelocity;
   Vector3<float> m_offsetPosition;
   Vector3<float> side;
   Vector3<float> futureProgress;
   float lastScreenX;
   float lastScreenY;
   float prevAngle;
   float currentAngle;
};

#endif
