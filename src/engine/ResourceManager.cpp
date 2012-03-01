#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#include "ResourceManager.h"
#include <iostream>
#include <fstream>

#define DEFAULT_SCALE 2

ResourceManager::ResourceManager() {
   chdir("../assets");
   
   m_image = NULL;

   /*
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
#endif*/
}


MeshData* ResourceManager::readMeshData(string fileName, LOAD_NORMAL_TYPE normalType, float scale) {
//    cout << "loading mesh: " << fileName << "\n";
   return loadMesh(fileName, normalType, scale);
}

TextureData* ResourceManager::loadBMPImage(string fileName) {
//   cout << "Load Texture: " << fileName << "\n";
   //if (m_image != NULL)
      //delete m_image; 
   
   m_image = SDL_LoadBMP(fileName.c_str());
   if (!m_image) {
      cout << "Error loading image: " << fileName << "\n";
      return NULL;
   }
   
   TextureData *textureData = new TextureData();
   textureData->size = ivec2(m_image->w, m_image->h);
   textureData->pixels = m_image->pixels;
   textureData->data = m_image;
   
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




MeshData* loadMesh(const string fileName, LOAD_NORMAL_TYPE type, float scale)
{
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
      std::cerr << "Could not open file: " << fileName << "\n";
      exit(EXIT_FAILURE);
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

WorldData* ResourceManager::readWorldData(string fileName) {
   WorldData *world = new WorldData();
   
   string line;
   streampos linestart;
   ifstream infile(fileName.c_str());
   
   if(infile.is_open())
   {
      while(getline(infile, line))
      {
         long start;
         
         if (!(start = line.find("p "))) {
            vec4 quant;
            Quaternion rot;
            mat3 rotMtx;
            vec3 loc, fwd, up;
            ivec4 link;
            int totalLinks;
            
            totalLinks = sscanf(line.c_str(), 
                                "p %d %f %f %f %f %f %f %f %d %d %d", 
                                &link.x, &loc.x, &loc.y, &loc.z, 
                                &quant.x, &quant.y, &quant.z, &quant.w,
                                &link.y, &link.z, &link.w);
            
            if (totalLinks == 9)
               link.z = link.w = -1;
            else if (totalLinks == 10)
               link.w = -1;
            
            rot = QuaternionT<float>(quant.x, quant.y, quant.z, quant.w);
            rotMtx = rot.ToMatrix();
            up = vec3(0, 0, 1);
            up = rotMtx.TranslatePoint(up);
            fwd = vec3(0, 1, 0);
            fwd = rotMtx.TranslatePoint(fwd);
            
            world->path.push_back(PathPointData(link, loc, fwd, up));
         }
         else if (!(start = line.find("u "))) {
            vec4 quant;
            Quaternion rot;
            mat3 rotMtx;
            vec3 loc, fwd, up;
            char buffer[64];
            string name;
            int read;
            int index;
            
            read = sscanf(line.c_str(), 
                          "u %d %f %f %f %f %f %f %f %s", 
                          &index, &loc.x, &loc.y, &loc.z, 
                          &quant.x, &quant.y, &quant.z, &quant.w,
                          buffer);
            
            if (read != 9)
            {
               std::cerr << "Short-read!\n";
            }
            
            name = string(buffer);
            
            rot = QuaternionT<float>(quant.x, quant.y, quant.z, quant.w);
            rotMtx = rot.ToMatrix();
            up = vec3(0, 0, 1);
            up = rotMtx.TranslatePoint(up);
            fwd = vec3(0, 1, 0);
            fwd = rotMtx.TranslatePoint(fwd);
            
            world->path[index].units.push_back(UnitData(UNIT_TURRET, loc, fwd, up));
         }
         else if (!(start = line.find("m "))) {
            vec4 quant;
            Quaternion rot;
            mat3 rotMtx;
            vec3 loc, fwd, up;
            char buffer[64];
            string name;
            int read;
            int index;
            
            read = sscanf(line.c_str(), 
                          "m %d %f %f %f %f %f %f %f %s", 
                          &index, &loc.x, &loc.y, &loc.z, 
                          &quant.x, &quant.y, &quant.z, &quant.w,
                          buffer);
            
            if (read != 9)
            {
               std::cerr << "Short-read!\n";
            }
            
            name = string(buffer);
            
            rot = QuaternionT<float>(quant.x, quant.y, quant.z, quant.w);
            rotMtx = rot.ToMatrix();
            up = vec3(0, 1, 0);
            up = rotMtx.TranslatePoint(up);
            fwd = vec3(0, 0, 1);
            fwd = rotMtx.TranslatePoint(fwd);
            
            world->path[index].props.push_back(PropData(name, loc, fwd, up));
         }
      }
   }
   else
   { cerr << "Problem is in World: the infile was never opened. \n"; }
   
   return world;
}

string ResourceManager::readShader(string name) {
   return readTextFile(name);
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

/**
 * Initalizes the sound system
 */
void initSound()
{
   // load support for the OGG and MOD sample/music formats
   //This is commented out to maintain compatibility with SDL_mixer 1.2.8  
   //As of SDL_mixer 1.2.10 or so, this should work, although we have not 
   // tested it extensively
   //int flags=MIX_INIT_OGG|MIX_INIT_MP3|MIX_INIT_MOD|MIX_INIT_FLAC;
   
   //int initted=Mix_Init(flags);
   
   /*
   if((initted&flags) != flags) {
      printf("Mix_Init: Failed to init required mp3 support!\n");
      printf("Mix_Init: %s\n", Mix_GetError());
      // handle error
   }
   */
   
   // open 22.05KHz, signed 16bit, system byte order,
   //      stereo audio, using 1024 byte chunks
   if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
   {
      cerr << "Mix_OpenAudio: "<< Mix_GetError() << "\n";
   }
}

/**
 * Loads a sound for SDL_mixer to use. This is intended for short sounds that 
 * need low latency, since the sound is loaded into memory by this function. 
 * @param fileName name of the sound file to load. Note that extension matters,
 * the file must have the file extension of .wav, .aiff, .aif, .aifc, .ogg or 
 * .voc
 */
Sound* loadSound(string fileName)
{
   Sound* sound = new Sound();
   
   string::size_type extBegin = fileName.find(".") + 1;
   string extension(fileName, extBegin, fileName.size() - extBegin);
   
   if ((SDL_strcasecmp(extension.c_str(), "wav") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "aiff") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "aif") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "aifc") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "ogg") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "voc") == 0))
   {
      sound->setChunk(Mix_LoadWAV(fileName.c_str()));
   }
   else
   {
      printf("Bad file format: %s\n", extension.c_str());
   }
   
   return sound;
}


/**
 * Loads a sound for SDL_mixer to use. Note that this is intended for background
 * music, thus might have some latency in starting a sound. Addtaionally,
 * SDL_mixer can only play a single music track at a time. 
 * @param fileName name of the sound file to load. Note that extension matters,
 * the file must have the file extension of .wav, .aiff, .aif, .aifc, .ogg or 
 * .voc
 */
Sound* loadMusic(string fileName)
{
   Sound* sound = new Sound();
   
   string::size_type extBegin = fileName.find(".") + 1;
   string extension(fileName, extBegin, fileName.size() - extBegin);
   
   // WAVE, MOD, MIDI, OGG, MP3, FLAC,
   
   if ((SDL_strcasecmp(extension.c_str(), "wav") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "mod") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "midi") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "ogg") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "mp3") == 0) ||
       (SDL_strcasecmp(extension.c_str(), "flac") == 0))
   {
      sound->setMusic(Mix_LoadMUS(fileName.c_str()));
   }
   else
   {
      printf("Bad file format: %s\n", extension.c_str());
   }
   
   return sound;
}

string readTextFile(string fileName) {
   //cout << fileName << "\n";
   
   const char *fn = fileName.c_str();
   char *data;
   int fileSize;
	
	if (fn != NULL) {
		ifstream input(fn);
      if (!input) {
         cerr << "error reading textfile: " << fileName << "\n";
         exit(1);
      }
      
      int stf = (int) input.tellg();
      input.seekg(0, ios::end);
      
      int endf = (int) input.tellg();
      
      fileSize = endf - stf;
      
      if(fileSize <= 0)
         return "";
      
      data = new char[fileSize+50];//50 added to just have buffer
      memset(data, '\0',fileSize+10);
      input.seekg(0, ios::beg);
      
      input.read(data,fileSize);
      input.close();
      data[fileSize]= '\0';	//appending the zero as the read does not append that      
	}
   
   string retStr = string(data);
   //cout << retStr << "\n";
   
	return retStr;
}
