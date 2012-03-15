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
#define FLASH_DURATION 30
#define TOTAL_FLASH_DURATION 120

//Forward declrations for enemnies
/**
 * Player class to handle the player
 */
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

   float getHealthPercent();
   
   /**
    * Gives the up vector of the player
    * @return The up vector of the player's aim
    */
   Vector3<float> getAimUp();

   /** returns the direction to the nearest enemy **/
   Vector3<float> getMagneticForward();

   /** sets the direction to the nearest enemy
    *  @param dir the direction to the enemy **/
   void setMagneticForward(vec3 dir);

   /**
    * Returns the vector off of the center of the ship
    */
   vec3 getOffSet();

   /**
    *  turns the magnetic aiming off
    **/
   void toggleMagnetic();

   /**
    *  returns the m_forward of the player (used in enemies)
   **/
   vec3 getMForward();
   
   bool getAlive();
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
    * The mesh of the ship and exhaust flames
    */
   Mesh *m_shipMesh;
   Mesh *m_exhaustMesh;

   //LOOK AT ME AND MY FANCY SHMANCY COMMENTS
   int m_count;
   bool m_isFlashing;

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
   Vector3<float> m_side;

   /**
    * The look ahead progress
    * NEEDED?
    */
   Vector3<float> futureProgress;

   /** the direction to aim at the nearest enemy **/
   Vector3<float> magnetic;

   /** whether you're using the magnetism **/
   bool magnet;

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

   /** 
    * Pointer to modules
    */
   Modules* m_modules;
   
   static const int health;
};

#endif
