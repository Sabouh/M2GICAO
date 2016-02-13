// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/objLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <list>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <locale.h>

using namespace std;


Mesh *ObjLoader::load(const string &filename) {

  if(isObj(filename)) {
    return loadObjFile(filename);
  }

  return NULL;
}


Mesh *ObjLoader::loadObjFile(const std::string &filename) {
  Mesh *mesh = NULL;

  vector<Vector3f > allVert;
  vector<Vector3f > allNorm;
  vector<Vector2f > allUV;
  vector<Vector3i>  allVertIndices;
  vector<Vector3i>  allNormIndices;
  vector<Vector3i>  allUVIndices;
  bool manuallyComputeUVs = false;

  // load obj geometry (pos,normals,uvs)
  if(!loadObjGeometry(filename,allVert,allNorm,allUV)) 
    return NULL;


  // check which kind of geometry we have 
  int type;
  if(!allVert.empty()) 
    type = 0;  // vertices 
  else 
    return NULL;

  if(!allNorm.empty()) 
    type += 1; // + normals 

  if(!allUV.empty())   
    type += 2; // + uvs
  else 
    manuallyComputeUVs = true;

  // load obj topology (indices)
  if(!loadObjTopology(filename,type,allVertIndices,allNormIndices,allUVIndices))
    return NULL;

  mesh = new Mesh();
  mesh->setVertices(allVert);
  mesh->setFaces(allVertIndices);
  mesh->computeNormals();
  allVert = mesh->getVertices();
  allNorm = mesh->getNormals();
  allVertIndices = mesh->getFaces();
  allNormIndices = mesh->getFaces();
  allNormIndices.clear();

  // init uvs with a simple spherical mapping if empty
  if(manuallyComputeUVs) {
    sphericalMapping(allVert,allVertIndices,allUV,allUVIndices);
  }

  // transform arrays into OpenGL indexed element array
  // --> one normal/uv per vertex
  // --> one array of indices needed
  vector<Vector3f> vertices = allVert;
  vector<Vector3f> normals = allNorm;
  vector<Vector2f> uvs(allVert.size(),Vector2f(0,0));
  vector<Vector3i> indices = allVertIndices;
  vector<bool>     seen(allVert.size(),false);

  // acceleration array for remembering mappings 
  vector<vector<int> > mappings(vertices.size());

  for(unsigned int i=0;i<allVertIndices.size();++i) {
    const Vector3i vi = allVertIndices[i];
    const Vector3i ti = allUVIndices[i];

    for(unsigned int j=0;j<3;++j) {
      const unsigned int indv  = vi[j];
      const unsigned int indt  = ti[j];
      const Vector2f currentUV = allUV[indt];

      if(!seen[indv]) {
	
	// never seen this uv, we can simply set it 
	uvs [indv] = currentUV;
	seen[indv] = true;
	mappings[indv].push_back((int)indv);
      
      } else {

	bool alreadyHandled = false;
	int  newIndex;

	// uv already set. search if it correspoond to a known vertex
	for(unsigned int k=0;k<mappings[indv].size();++k) {
	  const int inds = mappings[indv][k];
	  if(uvs[inds]==currentUV) {
	    alreadyHandled = true;
	    newIndex = inds;
	    break;
	  }
	}

	if(!alreadyHandled) {	  
	  // uv seen but does not correspond to any vertex --> duplicate 
      newIndex = static_cast<int>(vertices.size());
	  vertices.push_back(vertices[indv]);
	  normals.push_back(normals[indv]);
	  uvs.push_back(currentUV);
	  mappings[indv].push_back(newIndex);
	}
	
	indices[i][j] = newIndex;	  
      }
    }
  }

  // clear not needed arrays 
  allVert.clear();
  allNorm.clear();
  allUV.clear();
  allVertIndices.clear();
  allUVIndices.clear();
  mappings.clear();

  // build mesh 
  mesh->setVertices(vertices);
  mesh->setNormals(normals);
  mesh->setUvcoords(uvs);
  mesh->setFaces(indices);
  mesh->computeTangents();
  mesh->finalize();

  return mesh;
}




bool ObjLoader::loadObjGeometry(const string &filename,
				vector<Vector3f > &vertices,
				vector<Vector3f > &normals,
				vector<Vector2f > &uvs) {
  const int MAX_LEN = 1024;
  char line[MAX_LEN];
  char id  [MAX_LEN];
  FILE *fp = NULL;
  int returnValue;

  // open and check file
  if(!(fp = fopen(filename.c_str(),"r"))) 
    return false;

  // get vertices, normals, uv coords and materials
  while(1) {
    if((returnValue = fscanf(fp,"%s",id))==EOF) break;
    
    if(strcmp(id,"v")==0) {  // vertex
      Vector3f v;
      returnValue = fscanf(fp,"%f %f %f\n",&v[0],&v[1],&v[2]);
      vertices.push_back(v);
    } else if(strcmp(id,"vn")==0) { // normal
      Vector3f v;
      returnValue = fscanf(fp,"%f %f %f\n",&v[0],&v[1],&v[2]);
      normals.push_back(v);
    } else if(strcmp(id,"vt")==0) { // texture coord
      Vector2f v;
      float vtmp;
      returnValue = fscanf(fp,"%f %f %f\n",&v[0],&v[1],&vtmp);
      uvs.push_back(v);
    } else {                        // skip line
      if(!fgets(line,MAX_LEN,fp)) 
	cerr << "Error while reading " << filename << endl;
    }
  }
  
  fclose(fp);
  return true;
}


bool ObjLoader::loadObjTopology(const string &filename, int type,
				vector<Vector3i > &vertexIndices,
				vector<Vector3i > &normalIndices,
				vector<Vector3i > &uvIndices) {
  const int MAX_LEN = 1024;
  char line[MAX_LEN];
  char id  [MAX_LEN];
  FILE *fp = NULL;
  int returnValue;

  // open and check file
  if(!(fp = fopen(filename.c_str(),"r"))) 
    return false;

  // get vertex, normal, and uv indices 
  while(1) {
    if((returnValue = fscanf(fp,"%s",id))==EOF) break;
    
    if(strcmp(id,"f")==0 || strcmp(id,"fo")==0) {  
      // we found a face 
      Vector4i vi;
      Vector4i ni;
      Vector4i ti;

      switch(type) {
      case 0: // vertex only
	returnValue = fscanf(fp,"%d %d %d %d\n",&vi[0],&vi[1],&vi[2],&vi[3]);
	
	if(returnValue==3) {
	  // a simple triangle 
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));	  
	} else {
	  // consider this is a square... transform into 2 triangles
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[2]-1,vi[3]-1));
	}
	
	break;
      case 1: // vertex + normal
	returnValue = fscanf(fp,"%d//%d %d//%d %d//%d %d//%d\n",
			     &vi[0],&ni[0],&vi[1],&ni[1],&vi[2],&ni[2],&vi[3],&ni[3]);
	
	if(returnValue==6) {
	  // a simple triangle 
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));	  
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[1]-1,ni[2]-1));	  
	} else {
	  // consider this is a square... transform into 2 triangles	  
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[2]-1,vi[3]-1));
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[1]-1,ni[2]-1));
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[2]-1,ni[3]-1));
	}
	
	break;
      case 2: // vertex, texture
	returnValue = fscanf(fp,"%d/%d %d/%d %d/%d %d/%d\n",
			     &vi[0],&ti[0],&vi[1],&ti[1],&vi[2],&ti[2],&vi[3],&ti[3]);
	
	if(returnValue==6) {
	  // a simple triangle 
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[1]-1,ti[2]-1));
	} else {
	  // consider this is a square... transform into 2 triangles	  
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[2]-1,vi[3]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[1]-1,ti[2]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[2]-1,ti[3]-1));	  
	}
	
	break;
      case 3: // vertex, texture and normal
	returnValue = fscanf(fp,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
			     &vi[0],&ti[0],&ni[0],&vi[1],&ti[1],&ni[1],&vi[2],&ti[2],&ni[2],&vi[3],&ti[3],&ni[3]);
	
	if(returnValue==9) {
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[1]-1,ti[2]-1));	  
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[1]-1,ni[2]-1));	  
	} else {
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[1]-1,vi[2]-1));
	  vertexIndices.push_back(Vector3i(vi[0]-1,vi[2]-1,vi[3]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[1]-1,ti[2]-1));
	  uvIndices.push_back(Vector3i(ti[0]-1,ti[2]-1,ti[3]-1));
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[1]-1,ni[2]-1));
	  normalIndices.push_back(Vector3i(ni[0]-1,ni[2]-1,ni[3]-1));
	}
	
	break;
      }
      
    } else {
      // skip line
      if(!fgets(line,MAX_LEN,fp)) 
	cerr << "Error while reading " << filename << endl;
    }
  }
  
  fclose(fp);

  return true;
}


void ObjLoader::sphericalMapping(const std::vector<Vector3f> &vertices,
				 const std::vector<Vector3i> &vertexIndices,
				 std::vector<Vector2f> &uvs,
				 std::vector<Vector3i> &uvIndices) {
  // 1: compute original uv coordinates
  Vector3f center = barycenter(vertices);
  
  uvs = vector<Vector2f>(vertices.size());
  
  for(unsigned int i=0;i<vertices.size();++i) {
    const Vector3f v = vertices[i];
    const Vector3f c = (v-center).normalized();
    const float    r = max(min(c[2]/sqrt(c[0]*c[0]+c[2]*c[2]),1.0f),-1.0f);
    Vector2f uv;
    uv[0] = c[0]<0.0f ? M_PI-asin(r) : asin(r);
    uv[0] = (uv[0]+(M_PI/2.0))/(2.0*M_PI);
    uv[1] = acos(c[1])/M_PI;
    uvs[i] = uv;
  }

  uvIndices = vertexIndices;

  // 2: fix boundary issues
  for(unsigned int i=0;i<uvIndices.size();++i) {
    Vector3i f = uvIndices[i];

    Vector2f uv1 = uvs[f[0]];
    Vector2f uv2 = uvs[f[1]];
    Vector2f uv3 = uvs[f[2]];
    
    if(fabs(uv1[0]-uv2[0])>0.5f && fabs(uv1[0]-uv3[0])>0.5f) {
      // uv1 is away from uv2 and uv3 --> duplicate
      uv1[0] = uv2[0]>0.5f ? uv1[0]+1.0f : uv1[0]-1.0f;
      uvs.push_back(uv1);
      f[0] = static_cast<int>(uvs.size())-1;
      uvIndices[i] = f;
    } else if(fabs(uv2[0]-uv1[0])>0.5f && fabs(uv2[0]-uv3[0])>0.5f) {
      // uv2 is away from uv1 and uv3 --> duplicate
      uv2[0] = uv1[0]>0.5f ? uv2[0]+1.0f : uv2[0]-1.0f;
      uvs.push_back(uv2);
      f[1] = static_cast<int>(uvs.size())-1;
      uvIndices[i] = f;
    } else if(fabs(uv3[0]-uv1[0])>0.5f && fabs(uv3[0]-uv2[0])>0.5f) {
      // uv3 is away from uv1 and uv2 --> duplicate
      uv3[0] = uv1[0]>0.5f ? uv3[0]+1.0f : uv3[0]-1.0f;
      uvs.push_back(uv3);
      f[2] = static_cast<int>(uvs.size())-1;
      uvIndices[i] = f;
    } 
  }
}
