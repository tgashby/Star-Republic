#include "RenderingUtils.h"

const Surface *boundSurface;

LoadingJobs::LoadingJobs(IResourceManager *resourceManager) {
   m_resourceManager = resourceManager;
   lockIn = SDL_CreateMutex();
   lockOut = SDL_CreateMutex();
   newJob = SDL_CreateCond();
   in = list<LoadingJob*>(0);
   out = list<LoadingJob*>(0);
   m_loaderThread = SDL_CreateThread(loader_thread, this);
   if (m_loaderThread == NULL) {
      cerr << "Problem starting loader thread";
      exit(1);
   }
}

IResourceManager* LoadingJobs::GetResourceManager() {
   return m_resourceManager;
}

LoadingJobs::~LoadingJobs() {
   SDL_KillThread(m_loaderThread);
   SDL_DestroyMutex(lockIn);
   SDL_DestroyMutex(lockOut);
   SDL_DestroyCond(newJob);
}

void LoadingJobs::AddJob(LoadingJob *job) {
   SDL_mutexP(lockIn);
   in.push_back(job);
   SDL_mutexV(lockIn);
   SDL_CondSignal(newJob);
}

LoadingJob* LoadingJobs::GetJob() {
   SDL_mutexP(lockOut);
   LoadingJob *job = NULL;
   if (out.size() != 0) {
      job = out.front();
      out.pop_front();
   }
   SDL_mutexV(lockOut);
   return job;
}

LoadingJob* LoadingJobs::GetJobIn() {
   SDL_mutexP(lockIn);
   if (in.size() == 0) {
      //cout << "stop loading thread\n";
      SDL_CondWait(newJob, lockIn);
      //cout << "start loading thread\n";
   }
   LoadingJob *job = in.front();
   in.pop_front();
   SDL_mutexV(lockIn);
   return job;
}

void LoadingJobs::AddJobOut(LoadingJob *job) {
   SDL_mutexP(lockOut);
   out.push_back(job);
   SDL_mutexV(lockOut);
}

int loader_thread(void *jobs) {
   LoadingJobs *loadingJobs = (LoadingJobs *) jobs;
   IResourceManager *resourceManager = loadingJobs->GetResourceManager();
   LoadingJob *curJob;
   while (true) {
      // get a new job from the in list.
      curJob = loadingJobs->GetJobIn();
      
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
      loadingJobs->AddJobOut(curJob);
   }
   return 0;
}

void CreateSurface(Surface *surface, bool depth) {
   // Create the color render buffer
   glGenTextures(1, &surface->texture);
   glBindTexture(GL_TEXTURE_2D, surface->texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->size.x, surface->size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glBindTexture(GL_TEXTURE_2D, 0);
   CheckError("Creation of the color texture for the FBO");
   
   // Create the depth render buffer
   if (depth) {
      glGenRenderbuffersEXT(1, &surface->depth);
      glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, surface->depth);
      glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, surface->size.x, surface->size.y);
      CheckError("Creation of the depth renderbuffer for the FBO");
   }
   else {
      surface->depth = 0;
   }
   
   // Create the FBO
   glGenFramebuffersEXT(1, &surface->fbo);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, surface->texture, 0);
   if (depth) {
      glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, surface->depth);
   }
   CheckFBO();
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
   CheckError("Creation of the FBO");
}

void BindSurface(Surface *surface) {
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, surface->fbo);
   glViewport(surface->viewport.loc.x, surface->viewport.loc.y, surface->viewport.size.x, surface->viewport.size.y);
   // set matrixies
   
   boundSurface = surface;
}

void ClearSurface() {
   const Surface *surface = boundSurface;
   glClearColor(surface->clearColor.x, surface->clearColor.y, surface->clearColor.z, surface->clearColor.w);
   glClear(GL_COLOR_BUFFER_BIT | (surface->depth ? GL_DEPTH_BUFFER_BIT : 0));
}

void CheckFBO() {
   string enums[] = {
      "attachment",
      "missing attachment",
      "",
      "dimensions",
      "formats",
      "draw buffer",
      "read buffer",
      "unsupported format"
   };
   
   GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
   if (status == GL_FRAMEBUFFER_COMPLETE_EXT)
      return;
   status -= GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT;
   cerr << "incomplete framebuffer object due to " << enums[status] << "\n";
   exit(1);
}

void CheckError(string call) {
   string enums[] = {
      "invalid enumeration", // GL_INVALID_ENUM
      "invalid value",       // GL_INVALID_VALUE
      "invalid operation",   // GL_INVALID_OPERATION
      "stack overflow",      // GL_STACK_OVERFLOW
      "stack underflow",     // GL_STACK_UNDERFLOW
      "out of memory"        // GL_OUT_OF_MEMORY
   };
   
   GLenum errcode = glGetError();
   if (errcode == GL_NO_ERROR)
      return;
   errcode -= GL_INVALID_ENUM;
   cerr << "OpenGL " << enums[errcode] << " in " << call << "\n";
   exit(1);
}