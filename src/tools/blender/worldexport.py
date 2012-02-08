#"""
#Name: 'Star Republic world export (.wf)...'
#Blender: 261
#Group: 'Export'
#Tooltip: 'Export StarRepub World'

# use
# Mesh: M_(MeshName)__Ignored
# Point: P_(Type)__Ignored

bl_info = {
   "name": "Star Repubic World Exporter (.wf)",
   "author": "Robert Crosby",
   "version": (1, 0, 0),
   "blender": (2, 6, 1),
   "api": 31847,
   "location": "File > Export > Star Republic world export (.wf)",
   "description": "Export SR World (.wf)",
   "warning": "",
   "wiki_url": "",
   "tracker_url": "",
   "category": "Import-Export"}

import bpy, struct, math, os, sys
from mathutils import*
from math import*
from struct import*

#####################################
#World exporter
#####################################

class Mesh:
   def __init__(self, obj):
      self.obj = obj
   
   def writeMesh(self, out):
      out.write('m %f %f %f ' % (self.obj.location.x, self.obj.location.y, self.obj.location.z))
      out.write(self.obj.name)
      out.write('\n')

class Enemy:
   def __init__(self, obj):
      self.obj = obj
   
   def writeEnemy(self, out):
      out.write('e %f %f %f ' % (self.obj.location.x, self.obj.location.y, self.obj.location.z))
      out.write('turret\n')

class Path:
   def __init__(self):
      self.objs = []
   
   def addPoint(self, obj):
      self.objs.append(obj)
   
   def writePoints(self, index, out):
      for obj in self.objs:
         out.write('%d ' % (index))
         out.write('%f %f %f ' % (obj.location.x, obj.location.z, obj.location.y))
         fwd = Vector((0.0, 1.0, 0.0))
         fwd = fwd * obj.matrix_world
         out.write('%f %f %f ' % (fwd.x, fwd.z, -fwd.y))
         up = Vector((0.0, 0.0, 1.0))
         up = up * obj.matrix_world
         out.write('%f %f %f ' % (up.x, up.z, -up.y))
         side = Vector((1.0, 0.0, 0.0))
         side = side * obj.matrix_world
         out.write('%f %f %f ' % (side.x, side.z, -side.y))
         index += 1
         out.write('%d\n' % (index))
      return index

def exportWorld(filePath):
   path = Path()
   meshes = []
   enemies = []
   
   objs = sorted(bpy.context.selected_objects, key=lambda obj: obj.name)
   
   for obj in objs:
      if obj.name.find('P_') == 0:
         path.addPoint(obj)
      if obj.name.find('M_') == 0:
         meshes.append(Mesh(obj))
      if obj.name.find('E_') == 0:
         enemies.append(Enemy(obj))
   
   out = open(filePath, "w")
   
   path.writePoints(0, out)
   #out.write('meshes\n')
   #for mesh in meshes:
   #   mesh.writeMesh(out)
   #out.write('enemies\n')
   #for enemy in enemies:
   #   enemy.writeEnemy(out)
   
   
   out.close()

#####################################
#class registration and interface
#####################################

from bpy.props import *
class ExportMap(bpy.types.Operator):
   '''Export Star Republic World (.wf)'''
   bl_idname = "export.wf"
   bl_label = 'Export World'
   
   logenum = [("console", "Console", "log to console"),
              ("append", "Append", "append to log file"),
              ("overwrite", "Overwrite", "overwrite log file")]
   
   exportModes = []
   
   filepath = StringProperty(subtype='FILE_PATH', name="File Path", description="Filepath for exporting", maxlen=1024, default="")
   
   def execute(self, context):
      exportWorld(self.properties.filepath)
      return {'FINISHED'}
   
   def invoke(self, context, event):
      WindowManager = context.window_manager
      WindowManager.fileselect_add(self)
      return {"RUNNING_MODAL"}  

def menu_func(self, context):
   default_path = os.path.splitext(bpy.data.filepath)[0]
   self.layout.operator(ExportMap.bl_idname, text="Start Republic World (.wf)", icon='BLENDER').filepath = default_path

def register():
   bpy.utils.register_module(__name__)
   bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
   bpy.utils.unregister_module(__name__)
   bpy.types.INFO_MT_file_export.remove(menu_func)

if __name__ == "__main__":
   register()