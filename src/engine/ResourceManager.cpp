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
   
#ifdef __unix__
   chdir("../assets/");
#endif
   
   m_image = NULL;
}


MeshData* ResourceManager::readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale) {
    cout << "loading mesh: " << fileName << "\n";
   return loadMesh(fileName, normalType, scale);
}

TextureData* ResourceManager::loadBMPImage(string fileName) {
   if (m_image != NULL)
      delete m_image; 
   
   m_image = SDL_LoadBMP(fileName.c_str());
   if (!m_image) {
      cout << "Error loading image: " << fileName << "\n";
      return NULL;
   }
   
   TextureData *textureData = new TextureData();
   textureData->size = ivec2(m_image->w, m_image->h);
   textureData->pixels = m_image->pixels;
   
   return textureData;
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

ResourceManager::~ResourceManager() {
   
}




MeshData* loadMesh(const string fileName, LOAD_NORMAL_TYPE type, float scale) {
   vector<vec3 *> vertices(0);
   vector<vec3 *> normals(0);
   vector<vec3 *> tangents(0);
   vector<vec2 *> textures(0);
   vector<Face *> faces(0);
   vec3 mean, min, max;
   mean.x = mean.y = mean.z = 0;
   max.x = max.y = max.z = 1.1754E-38F;
   min.x = min.y = min.z = 1.1754E+38F;
   
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());

   vec3 *v;
   vec2 *t;
   if (infile.is_open())  
   {
      while (!infile.eof())
      {
         long start;
         
         linestart = infile.tellg();
         getline(infile,line);
         
         // Ignore commented lines
         if (line.find("#") == 0)
            continue;
         
         if (!(start = line.find("v "))) // we found a vertex
         {
            v = parseVertex(line);
            *v = *v * scale;
            
            mean = mean + *v;
            if (v->x > max.x) max.x = v->x;
            if (v->y > max.y) max.y = v->y;
            if (v->z > max.z) max.z = v->z;
            if (v->x < min.x) min.x = v->x;
            if (v->y < min.y) min.y = v->y;
            if (v->z < min.z) min.z = v->z;
            
            vertices.push_back(v);
         }
         else if (!(start = line.find("vn "))) // we found a normal
         {
            // skip normals if loading a flat shaded mesh.
            if (type != LOAD_NORMAL_FACE) {
               v = parseNormal(line);
               normals.push_back(v);
            }
         }
         else if (!(start = line.find("vnt "))) // we found a tangent
         {
            if (type != LOAD_NORMAL_FACE) {
               v = parseTangent(line);
               tangents.push_back(v);
            }
         }
         else if (!(start = line.find("vt "))) // we found a vertex
         {
            t = parseTexture(line);
            textures.push_back(t);
         }
         else if (!(start = line.find("f "))) // we found a face
         {
            faces.push_back(parseFace(line));
         }
      }
      infile.close();
   } 
   else
   {
      throw("Could not open file ");
   }
   
   MeshData *meshData = new MeshData();
   meshData->vertexCount = (int) faces.size() * 3;
   meshData->vertices = new float[meshData->vertexCount * 11];
   meshData->indexCount = (int) faces.size() * 3;
   meshData->indices = new unsigned short[meshData->indexCount];
   
   meshData->bounds.mean = mean / (float) vertices.size();
   meshData->bounds.min = min;
   meshData->bounds.max = max;
   
   vector<Face *>::iterator face;
   float *vertLoc = meshData->vertices;
   unsigned short *indexLoc = meshData->indices;
   int curVertex = 0;
   for (face = faces.begin(); face != faces.end(); ++face) {
      Face *f = *face;
      vec3 emptyVec(0, 0, 0);
      if (type == LOAD_NORMAL_FACE) {
         f->setFaceNormal(vertices);
      }
      combineVertex(vertLoc,
                    vertices[f->vertices.x],
                    (type == LOAD_NORMAL_FACE) ? &f->normal : normals[f->normals.x],
                    (type == LOAD_NORMAL_FACE) ? &emptyVec : tangents[f->tangents.x],
                    textures[f->textures.x]);
      vertLoc += 11;
      combineVertex(vertLoc,
                    vertices[f->vertices.y],
                    (type == LOAD_NORMAL_FACE) ? &f->normal : normals[f->normals.y],
                    (type == LOAD_NORMAL_FACE) ? &emptyVec : tangents[f->tangents.y],
                    textures[f->textures.y]);
      vertLoc += 11;
      combineVertex(vertLoc,
                    vertices[f->vertices.z],
                    (type == LOAD_NORMAL_FACE) ? &f->normal : normals[f->normals.z],
                    (type == LOAD_NORMAL_FACE) ? &emptyVec : tangents[f->tangents.z],
                    textures[f->textures.z]);
      vertLoc += 11;
      
      indexLoc[0] = curVertex++;
      indexLoc[1] = curVertex++;
      indexLoc[2] = curVertex++;
      indexLoc += 3;
      
      //delete f;
   }
   
   //clean up
   vector<vec3 *>::iterator v3;
   vector<vec2 *>::iterator v2;
   for (v3 = vertices.begin(); v3 != vertices.end(); ++v3) {
      delete *v3;
   }
   for (v3 = normals.begin(); v3 != normals.end(); ++v3) {
      delete *v3;
   }
   for (v3 = tangents.begin(); v3 != tangents.end(); ++v3) {
      delete *v3;
   }
   for (v2 = textures.begin(); v2 != textures.end(); ++v2) {
      delete *v2;
   }
   
   return meshData;
}

float parseFloat(const std::string line) {
   
   float f;
   if (sscanf(line.c_str(), "%g", &f) != 1) {
      printf("error reading float\n");
   }
   return f;
}

vec3* parseVertex(const string line) {
   float x, y, z;
   if (sscanf(line.c_str(), "v %g %g %g", &x, &y, &z) != 3) {
      cout << "error reading vertex\n";
   }
   return new vec3(x, y, z);
}

vec3* parseNormal(const string line) {
   float x, y, z;
   if (sscanf(line.c_str(), "vn %g %g %g", &x, &y, &z) != 3) {
      cout << "error reading normal\n";
   }
   return new vec3(x, y, z);
}

vec3* parseTangent(const string line) {
   float x, y, z;
   if (sscanf(line.c_str(), "vnt %g %g %g", &x, &y, &z) != 3) {
      cout << "error reading tanget\n";
   }
   return new vec3(x, y, z);
}

vec2* parseTexture(const string line) {
   float u, v;
   if (sscanf(line.c_str(), "vt %g %g", &u, &v) != 2) {
      cout << "error reading texture coordinates\n";
   }
   return new vec2(u, v);
}

Face* parseFace(string line) {
   Face* f = new Face();
   size_t start;
   
   start = line.find(" ");
   line = line.substr(++start);
   f->vertices.x = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->textures.x = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->normals.x = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->tangents.x = static_cast<int>(parseFloat(line));
   
   start = line.find(" ");
   line = line.substr(++start);
   f->vertices.y = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->textures.y = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->normals.y = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->tangents.y = static_cast<int>(parseFloat(line));
   
   start = line.find(" ");
   line = line.substr(++start);
   f->vertices.z = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->textures.z = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->normals.z = static_cast<int>(parseFloat(line));
   start = line.find("/");
   line = line.substr(++start);
   f->tangents.z = static_cast<int>(parseFloat(line));
   
   return f;
}

void combineVertex(float *vertices, vec3 *vertex, vec3 *normal, vec3 *tangent, vec2 *texture) {
   vertices[0] = vertex->x;
   vertices[1] = vertex->y;
   vertices[2] = vertex->z;
   vertices[3] = normal->x;
   vertices[4] = normal->y;
   vertices[5] = normal->z;
   vertices[6] = tangent->x;
   vertices[7] = tangent->y;
   vertices[8] = tangent->z;
   vertices[9] = texture->x;
   vertices[10] = texture->y;
}
