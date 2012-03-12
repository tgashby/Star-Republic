#ifndef StarRepub_ResourceLoader_h
#define StarRepub_ResourceLoader_h

#include "Interfaces.h"
#include "SDL_thread.h"

enum LOAD_JOB_TYPE {
   LOAD_JOB_MESH,
   LOAD_JOB_TEXTURE,
};

struct LoadingJob {
   LoadingJob(LOAD_JOB_TYPE t, IRef *r) {
      data = NULL;
      type = t;
      ref = r;
   }
   void *data;
   LOAD_JOB_TYPE type;
   IRef *ref;
};

class ResourceLoader {
public:
   ResourceLoader(IResourceManager *resourceManager);
   ~ResourceLoader();
   IResourceManager* getResourceManager();
   void addJobIn(LoadingJob *job);
   LoadingJob* getJobIn();
   void addJobOut(LoadingJob *job);
   LoadingJob* getJobOut();
private:
   SDL_mutex *m_lockIn;
   SDL_mutex *m_lockOut;
   SDL_cond *m_sigNewJob;
   list<LoadingJob*> m_inList;
   list<LoadingJob*> m_outList;
   SDL_Thread *m_loaderThread;
   IResourceManager *m_resourceManager;
};

int loader_thread(void *jobs);

#endif
