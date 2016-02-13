###########################################
############ LOOP SUBDIVISION #############
###########################################

import bpy
import bmesh
from math import cos, pi

def other_vert(f,v1,v2):
    # get the vertex of the face f which is not v1 nor v2
    for v in f.verts:
        if(v.index!=v1.index and v.index!=v2.index):
            return v

    # this function is already built-in for edges
    # used with: edge.other_vert(vert)
    
    return f.verts[0] # never goes there



def example_mesh_manip(obm):
    # print infos
    print("the mesh has ",len(obm.verts)," vertices, ",len(obm.edges)," edges and ",len(obm.faces)," faces")
    
    # access vertices 
    for v in obm.verts:
        print("vertex ",v.index,": ",v.co)
    
    # access edges 
    for e in obm.edges:
        print("edge ",e.index," has vertices: ",e.verts[0].index," -- ",e.other_vert(e.verts[0]).index)
    
    # access faces 
    for f in obm.faces:
        print("face ",f.index," has edges: ",f.edges[0].index," -- ",f.edges[1].index," -- ",f.edges[2].index)
        print("face ",f.index," has vertices: ",f.verts[0].index," -- ",f.verts[1].index," -- ",f.verts[2].index)
    
    # array of positions
    vertices = []
    for v in obm.verts:
        # rescale the x-coord, just for the example
        pos = (v.co.x*1.2,v.co.y,v.co.z)
        vertices.append(pos)
    
    # create a new mesh 
    nbm = bmesh.new()
    
    # initialize the new mesh with the array of vertices 
    for v in vertices:
        nbm.verts.new(v)
    
    # array of indices that represent faces
    for f in obm.faces:
        
        # get vertex indices (in the old mesh) 
        v1 = f.verts[0].index
        v2 = f.verts[1].index
        v3 = f.verts[2].index
        
        # create this face in the new mesh
        nbm.faces.new([nbm.verts[v1],nbm.verts[v2],nbm.verts[v3]])
    
    # return the new mesh 
    return nbm



def loop_subdivision(obm):
    ####################################
    # YOUR CODE/FUNCTIONS SHOULD GO HERE
    ####################################
    return example_mesh_manip(obm)


def subdivide():
    # get selected object
    obj = bpy.context.active_object
    
	# object verification 
    if(obj is not None and obj.type=='MESH'):
        
        # make sure to be in edit mode 
        bpy.ops.object.mode_set(mode='EDIT')
        
        # create mesh from data 
        obm = bmesh.new()
        obm.from_mesh(obj.data)
        
        # triangulate first and init tags
        bmesh.ops.triangulate(obm,faces=obm.faces)
        
        # the subdivision itself
        nbm = loop_subdivision(obm)
        
        # update mesh normals
        bmesh.ops.recalc_face_normals(nbm,faces=nbm.faces)
        
        # update object
        bpy.ops.object.mode_set(mode='OBJECT')
        nbm.to_mesh(obj.data)
        
        # free everything
        obm.free()
        nbm.free()
		
		
subdivide()