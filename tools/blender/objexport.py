#"""
#Name: 'Custom Wavefront export (.obj)...'
#Blender: 261
#Group: 'Export'
#Tooltip: 'Export Wavefront'

# use
# Mesh: M_(MeshName)__Ignored
# Point: P_(Type)__Ignored

bl_info = {
    "name": "Custom Wavefront export (.obj)",
    "author": "Robert Crosby",
    "version": (1, 0, 0),
    "blender": (2, 6, 1),
    "api": 31847,
    "location": "File > Export > Custom Wavefront export (.obj)",
    "description": "Export Wavefront (.obj)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import bpy, struct, math, os, sys
from struct import*

#####################################
#Mesh exporter
#####################################

class vertex:
    def __init__(self, i, ver):
        self.index = i
        self.data = ver
        self.x = ver.co.x
        self.y = ver.co.y
        self.z = ver.co.z
        self.normx = ver.normal.x
        self.normy = ver.normal.y
        self.normz = ver.normal.z
        self.tangentx = 0.0
        self.tangenty = 0.0
        self.tangentz = 0.0
        self.u = 0
        self.v = 0
        self.faces = []
    def writeVertex(self, out):
        out.write('v %f %f %f\n' % (self.x , self.y, self.z))
    def writeNormal(self, out):
        out.write('vn %f %f %f\n' % (self.normx, self.normy, self.normz))
    def writeTangent(self, out):
        out.write('vnt %f %f %f\n' % (self.tangentx, self.tangenty, self.tangentz))
    def writeTexture(self, out):
        out.write('vt %f %f\n' % (self.u, self.v))
    def settoface(self, f):
        self.normx = f.x
        self.normy = f.y
        self.normz = f.z
        uv = f.getuv(self.index)
        self.u = uv.u
        self.v = uv.v
    def checkface(self, f):
        uv = f.getuv(self.index)
        coords = self.u == uv.u and self.v == uv.v
        return coords
    def extract(self):
        items = []
        for i, f in enumerate(self.faces):
            if(not self.checkface(f)):
                items.append(i)
        items.reverse()
        list = []
        for i in items:
            list.append(self.faces.pop(i))
        return list
    def duplicate(self, i):
        return vertex(i, self.data)
    def addface(self, f):
        self.faces.append(f)


class face:
    def __init__(self, i, f):
        self.index = i
        self.data = f
        self.verts = ['', '', '']
        self.verts[0] = f.vertices[0]
        self.verts[1] = f.vertices[1]
        self.verts[2] = f.vertices[2]
        self.normalx = f.normal.x
        self.normaly = f.normal.y
        self.normalz = f.normal.z
        self.uvs = []
    def writetofile(self, out):
        out.write('f %d/%d/%d/%d ' % (self.verts[0], self.uvs[0].i, self.verts[0], 0))
        out.write('%d/%d/%d/%d ' % (self.verts[1], self.uvs[1].i, self.verts[1], 0))
        out.write('%d/%d/%d/%d\n' % (self.verts[2], self.uvs[2].i, self.verts[2], 0))
    def adduvs(self, uvs):
        self.uvs = uvs
    def reverseFace(self):
        tmp = self.verts[0]
        self.verts[0] = self.verts[2]
        self.verts[2] = tmp
        tmp = self.uvs[0]
        self.uvs[0] = self.uvs[2]
        self.uvs[2] = tmp
    def swapvert(self, newIndex, oldIndex):
        if(self.verts[0] == oldIndex):
            self.verts[0] = newIndex
        if(self.verts[1] == oldIndex):
            self.verts[1] = newIndex
        if(self.verts[2] == oldIndex):
            self.verts[2] = newIndex
    def getuv(self, i):
        if(self.verts[0] == i):
            return self.uvs[0]
        if(self.verts[1] == i):
            return self.uvs[1]
        return self.uvs[2]

class uvcoord:
    def __init__(self, i, uv):
        self.data = uv
        self.i = i
        self.u = uv[0]
        self.v = uv[1]
    def writetofile(self, out):
        out.write('vt %f %f\n' % (self.u, self.v))
    def compare(self, other):
        return self.u == other.u and self.v == other.v


def orientFace(face, vertices):
    pt1 = [vertices[face.verts[0]].x, vertices[face.verts[0]].y, vertices[face.verts[0]].z]
    pt2 = [vertices[face.verts[1]].x, vertices[face.verts[1]].y, vertices[face.verts[1]].z]
    pt3 = [vertices[face.verts[2]].x, vertices[face.verts[2]].y, vertices[face.verts[2]].z]
    
    v1 = [pt1[0] - pt2[0], pt1[1] - pt2[1], pt1[2] - pt2[2]]
    v2 = [pt3[0] - pt2[0], pt3[1] - pt2[1], pt3[2] - pt2[2]]
    
    n1 = [v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]]
    n2 = [face.normalx, face.normaly, face.normalz]
    
    dot = n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2]
    if dot < 0:
        face.reverseFace()
    return face

def findDuplicateCoord(uva, uvs):
    for uvb in uvs:
        if uva.compare(uvb):
            return uvb
    uvs.append(uva)
    return uva

def exportMesh(filePath):
    obj = bpy.context.active_object
    mesh = obj.data
    
    vertices = []
    for i, v in enumerate(mesh.vertices):
        vertices.append(vertex(i, v))
    
    faces = []
    for i, f in enumerate(mesh.faces):
        f = face(i, f)
        faces.append(f)
    
    uvs = []
    for i, f in enumerate(mesh.uv_textures.active.data):
        fuvs = []
        for j, uv in enumerate(f.uv):
            coord = uvcoord(len(uvs), uv)
            coord = findDuplicateCoord(coord, uvs)
            fuvs.append(coord)
        faces[i].adduvs(fuvs)
    
    out = open(filePath, "w")
    
    for v in vertices:
        v.writeVertex(out)
    
    for v in vertices:
        v.writeNormal(out)
    
    #write a blank tangent for now.
    out.write('vnt %f %f %f\n' % (0.0, 0.0, 0.0))
    
    for uv in uvs:
        uv.writetofile(out)
    
    for f in faces:
        f = orientFace(f, vertices)
        f.writetofile(out)
    
    out.close()

#####################################
#class registration and interface
#####################################

from bpy.props import *
class ExportMap(bpy.types.Operator):
    '''Export Custom Wavefront (.obj)'''
    bl_idname = "export.obj"
    bl_label = 'Export Custom Wavefront'
    
    logenum = [("console", "Console", "log to console"),
               ("append", "Append", "append to log file"),
               ("overwrite", "Overwrite", "overwrite log file")]
             
    exportModes = []
    
    filepath = StringProperty(subtype='FILE_PATH', name="File Path", description="Filepath for exporting", maxlen=1024, default="")
    
    def execute(self, context):
        exportMesh(self.properties.filepath)
        return {'FINISHED'}
    
    def invoke(self, context, event):
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}  

def menu_func(self, context):
    default_path = os.path.splitext(bpy.data.filepath)[0]
    self.layout.operator(ExportMap.bl_idname, text="Custom Wavefront (.obj)", icon='BLENDER').filepath = default_path
  
def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
    register()
