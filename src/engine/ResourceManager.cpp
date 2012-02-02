#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#include "ResourceManager.h"
#include <iostream>
#include <fstream>

#define DEFAULT_SCALE 2

ResourceManager::ResourceManager() {
   //This ugly code sets the working directory to the app bundle in OSX.
#ifdef __APPLE__
   CFBundleRef mainBundle = CFBundleGetMainBundle();
   CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
   char path[PATH_MAX];
   if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
   {
      // error!
   }
   CFRelease(resourcesURL);
   chdir(path);
   std::cout << "Current Path: " << path << std::endl;
#endif
   m_image = NULL;
}

MeshData* ResourceManager::readMeshData(string fileName, LOAD_NORMAL_TYPE normalType) {
   return readMeshData(fileName, normalType, DEFAULT_SCALE);
}


MeshData* ResourceManager::readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale) {
   return loadMesh(fileName, normalType, scale);
}

ImageData* ResourceManager::loadBMPImage(string fileName) {
   m_image = SDL_LoadBMP(fileName.c_str());
   if (!m_image) {
      cout << "Error loading image: " << fileName << "\n";
      return NULL;
   }
   
   ImageData *imageData = new ImageData();
   imageData->size = ivec2(m_image->w, m_image->h);
   imageData->pixels = m_image->pixels;
   
   return imageData;
}
/*
ImageData* ResourceManager::loadPngImage(string fileName) {
   //SDL_RWops *rwop;
   //rwop = SDL_RWFromFile(fileName.c_str(),"rb");
   //m_image = IMG_LoadPNG_RW(rwop);
   //SDL_LoadBMP_RW(rwop, 1);
   
   m_image = SDL_LoadBMP(fileName.c_str());
   //m_image = IMG_Load(fileName.c_str());
   if (!m_image) {
      //cout << "IMG_LoadPNG_RW: " << IMG_GetError() << "\n";
      cout << "Error loading image: " << fileName << "\n";
      return NULL;
   }
   
   ImageData *imageData = new ImageData();
   imageData->size = ivec2(m_image->w, m_image->h);
   imageData->pixels = m_image->pixels;
   
   
   return imageData;
}*/

void ResourceManager::unLoadImage() {
   delete m_image;
   m_image = NULL;
}

ResourceManager::~ResourceManager() {
   
}
