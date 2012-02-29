#ifndef StarRepub_Box_h
#define StarRepub_Box_h

#include "../engine/Interfaces.h"
#include "../engine/Camera.h"
#include "../engine/Object3d.h"
#include "../game/Flyer.h"
#include "../engine/Mesh.h"
#include <assert.h>
#include <cmath>

/**
 * Skybox class to hold an outer-space textured box
 */
class SkyBox : public Object3d {
public:

  /**
   * Constructor. This creates a skybox centered at the player.
   * @param fileName - The filename of the mesh for the skybox.
   * @param textureName - The filename of the texture for the skybox.
   * @param modules - The set of game modules
   * @param player_pos - the player position
   * path directly
   */
   SkyBox(string fileName, string textureName, Modules *modules, Vector3<float> player_pos);
   
   /**
    * Deconstructor. This deletes the player and its data.
    */
   ~SkyBox();

   /**
    * tic updates the player based on the time passed and the camera
    * @param time the time passed since the last update, in milliseconds
    * @param player_position the current player position
    */
   void tic(uint64_t time, Vector3<float> player_position);

  
private:
   Mesh *m_mesh;
};

#endif
