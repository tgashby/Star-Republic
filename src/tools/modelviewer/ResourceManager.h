#ifndef SimpleGame_ResourceManager_h
#define SimpleGame_ResourceManager_h

#include "Interfaces.h"
#include "MeshParser.h"
#include <SDL/SDL.h>
//#include <SDL_image/SDL_image.h>

class ResourceManager : public IResourceManager {
public:
   ResourceManager();
   static MeshData* readMeshData(string fileName, LOAD_NORMAL_TYPE normalType);
   static MeshData* readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale);
   ImageData* loadBMPImage(string fileName);
   //ImageData* loadPngImage(string fileName);
   void unLoadImage();
   ~ResourceManager();
private:
   SDL_Surface *m_image;
};

#endif
