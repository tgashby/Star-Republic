#ifndef SimpleGame_ApplicationEngine_h
#define SimpleGame_ApplicationEngine_h

#include "Interfaces.h"
#include "Object3d.h"
#include "Camera.h"

#define MOUSE_MOVE_SENSITIVITY 0.5f
#define MOUSE_WHEEL_SENSITIVITY 0.2f

class ApplicationEngine : public IApplicationEngine {
public:
   ApplicationEngine(IResourceManager* resourceManager,IRenderingEngine* renderingEngine);
   ~ApplicationEngine();
   void handleKeyDown(const char *k);
   void handleKeyUp(const char *k);
   void handleMouseDown(int button, int mouseX, int mouseY);
   void handleMouseMove(int mouseX, int mouseY);
   void handleMouseUp(int button, int mouseX, int mouseY);
   void handelMouseWheel(int dir);
   void tic(unsigned int td);
   void render();
private:
   IResourceManager* m_resourceManager;
   IRenderingEngine* m_renderingEngine;
   list<IObject3d *> m_objects;
   Camera *m_camera;
   bool upkey, downkey, leftkey, rightkey;
   bool mouseLeft, mouseMiddle, mouseRight;
   ivec2 m_lastMouseLoc;
};

#endif
