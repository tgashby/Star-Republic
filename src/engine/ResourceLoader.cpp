#include "ResourceLoader.h"

ResourceLoader::ResourceLoader(IResourceManager *resourceManager) {
   m_resourceManager = resourceManager;
   m_lockIn = SDL_CreateMutex();
   m_lockOut = SDL_CreateMutex();
   m_sigNewJob = SDL_CreateCond();
   m_inList = list<LoadingJob*>(0);
   m_outList = list<LoadingJob*>(0);
   m_loaderThread = SDL_CreateThread(loader_thread, this);
   if (m_loaderThread == NULL) {
      cerr << "Problem starting loader thread";
      exit(1);
   }
}

IResourceManager* ResourceLoader::getResourceManager() {
   return m_resourceManager;
}

ResourceLoader::~ResourceLoader() {
   SDL_KillThread(m_loaderThread);
   SDL_DestroyMutex(m_lockIn);
   SDL_DestroyMutex(m_lockOut);
   SDL_DestroyCond(m_sigNewJob);
}

void ResourceLoader::addJobIn(LoadingJob *job) {
   SDL_mutexP(m_lockIn);
   m_inList.push_back(job);
   SDL_mutexV(m_lockIn);
   SDL_CondSignal(m_sigNewJob);
}

LoadingJob* ResourceLoader::getJobIn() {
   SDL_mutexP(m_lockIn);
   if (m_inList.size() == 0) {
      //cout << "stop loading thread\n";
      SDL_CondWait(m_sigNewJob, m_lockIn);
      //cout << "start loading thread\n";
   }
   LoadingJob *job = m_inList.front();
   m_inList.pop_front();
   SDL_mutexV(m_lockIn);
   return job;
}

void ResourceLoader::addJobOut(LoadingJob *job) {
   SDL_mutexP(m_lockOut);
   m_outList.push_back(job);
   SDL_mutexV(m_lockOut);
}

LoadingJob* ResourceLoader::getJobOut() {
   SDL_mutexP(m_lockOut);
   LoadingJob *job = NULL;
   if (m_outList.size() != 0) {
      job = m_outList.front();
      m_outList.pop_front();
   }
   SDL_mutexV(m_lockOut);
   return job;
}

int loader_thread(void *loader) {
   ResourceLoader *resourceLoader = (ResourceLoader *) loader;
   IResourceManager *resourceManager = resourceLoader->getResourceManager();
   LoadingJob *curJob;
   while (true) {
      // get a new job from the in list.
      curJob = resourceLoader->getJobIn();
      
      // process the job
      switch (curJob->type) {
         case LOAD_JOB_MESH:
            // load a mesh
            curJob->data = resourceManager->readMeshData(curJob->ref->fileName, LOAD_NORMAL_VERTEX, 1.0);
            break;
            
         case LOAD_JOB_TEXTURE:
            // load a texture
            curJob->data = resourceManager->loadBMPImage(curJob->ref->fileName);
            break;
            
         default:
            cerr << "Unknown load job\n";
            exit(1);
            break;
      }
      
      // put the finished job in the out list.
      resourceLoader->addJobOut(curJob);
   }
   return 0;
}