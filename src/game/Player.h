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

class Player : public Object3d, public Flyer {
public:

  /**
   * Constructor. This creates a player based on the given camera location.
   * @param fileName - The filename of the mesh for the player.
   * @param textureName - The filename of the texture for the player.
   * @param modules - The set of game modules
   * @param cam_pos - The position of the camera along the path
   * @param cam_up - The up vector of the camera along the path
   * @param cam_forw - The forward vector of the camera; it follows the
   * path directly
   */
   Player(string fileName, string textureName, Modules *modules, 
	Vector3<float> cam_pos, Vector3<float> cam_up, Vector3<float> cam_forw);
   
   /**
    * Deconstructor. This deletes the player and its data.
    */
   ~Player();

   /**
    * Increments the player's position based on the camera's new position, 
    * as well as having time-based side effects.
    * @param time - The time that has pass since the previous screen.
    * @param cam_position - The new position of the camera
    * @param cam_up - The new up vector of the camera
    * @param cam_forward - the new forward vector of the camera
    */
   void tic(uint64_t time, Vector3<float> cam_position, Vector3<float> cam_up, Vector3<float> cam_forward); 

//GOBJ/Flyer
//   void setProgressVelocity(Vector3<float> vel); Flyer
//   void setPosition(Vector3<float> pos); GOBJ
//   void setAcceleration(Vector3<float> acc); GOBJ
//   void setBearing(Vector3<float> headPos, Vector3<float> headUp); //Flyer
//   void setProgress(Vector3<float> pos); Flyer
//   void setUp(Vector3<float> upVal); GOBJ
//   void setFutureProgress(Vector3<float> pos);
//   Vector3<float> getPosition(); GOBJ
//   Vector3<float> getForward(); GOBJ/Flyer
//   Vector3<float> getUp(); GOBJ
//   Vector3<float> getProgress(); Flyer
//   Vector3<float> getProgressVelocity(); Flyer
//   void setFuturePosition(Vector3<float> pos);
//   void setHeads(Vector3<float> currHeadPos, Vector3<float> currHeadUp,
//                 Vector3<float> prevHeadPos, Vector3<float> prevHeadUp); Flyer
   //Vector3<float> position; GOBJ
   //Vector3<float> progressVelocity; Flyer
   //Vector3<float> shipVelocity; GOBJ
   //Vector3<float> forward; GOBJ
   //Vector3<float> acceleration; GOBJ
   //Vector3<float> progress; Flyer
   //Vector3<float> up; GOBJ
//	Vector3<float> currentHeadPos; Flyer
//	Vector3<float> previousHeadPos; Flyer
//	Vector3<float> currentHeadUp; Flyer
//	Vector3<float> previousHeadUp; Flyer

   /**
    * Gives the side vector of the ship
    * @return The side vector of the ship's current looks
    */
   Vector3<float> getSide();

   /**
    * Calculats the side. Useful for a lot of thing, so should 
    * probably be moved to a Utils class.
    */
   void calculateSide();

   /**
    * Turns the ship visible/invisible (specifically for fps camera usage)
    * @param The new visibility status of the player
    */
   void setVisible(bool visibility);

   /**
    * The collision handling code for the ship
    * @param The object it is colliding with
    */
   void doCollision(GameObject & other);

   /**
    * Changes the ship's look based on the changes in X and Y
    * @param diffX - The change in X screen coordinates
    * @param diffY - The change in Y screen coordinates
    */
   void updateVelocity(float diffX, float diffY);

   /**
    * Gives the player's direction of aiming, not movement
    * @return The direction the player is aiming
    */
   Vector3<float> getAimForward();

   /**
    * Gives the up vector of the player
    * @return The up vector of the player's aim
    */
   Vector3<float> getAimUp();
   
private:
   /**
    * The velocity in the left-right direction of the ship
    */
   float vx;

   /**
    * The velocity in the up-down direction of the ship
    */
   float vy; 
   
   /**
    * The current x coordinate on the screen of the reticle
    */
   float x;

   /**
    * The current y coordinate on the screen of the reticle
    */
   float y;
   
   /**
    * The horizontal position of the ship 
    */
   vec3 xPosition;

   /**
    * The vertical position of the ship
    */
   vec3 yPosition;

   /**
    * The mesh of the ship
    */
   Mesh *m_mesh;

   /**
    * The forward velocity of the ship
    */
   Vector3<float> m_shipVelocity;
   
   /**
    * The horizontal velocity of the ship
    */
   Vector3<float> m_sideVelocity;

   /**
    * The vertical velocity of the ship
    */
   Vector3<float> m_upVelocity;

   /**
    * The change to position to be made
    */
   Vector3<float> m_offsetPosition;

   /** 
    * The side vector of the ship
    */
   Vector3<float> side;

   /**
    * The look ahead progress
    * NEEDED?
    */
   Vector3<float> futureProgress;

   /**
    * The last known horizontal screen coordinate
    */
   float lastScreenX;
   
   /**
    * The last known vertical screen coordinate
    */
   float lastScreenY;

   /**
    * The last angle moved
    */
   float prevAngle;

   /**
    * The current angle taken
    */
   float currentAngle;
};

#endif
