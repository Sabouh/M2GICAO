// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "misc/extinclude.h"
#include "misc/mesh.h"

#include <string>
#include <map>
#include <vector>

class LIBRARY_EXPORT ObjLoader {
 public:  
  static Mesh *load(const std::string &filename);

 private:
  static Mesh *loadObjFile(const std::string &filename);
  
  static bool loadObjGeometry(const std::string &filename,
			      std::vector<Vector3f > &vertices,
			      std::vector<Vector3f > &normals,
			      std::vector<Vector2f > &uvs);
  
  static bool loadObjTopology(const std::string &filename, int type,
			      std::vector<Vector3i > &vertexIndices,
			      std::vector<Vector3i > &normalIndices,
			      std::vector<Vector3i > &uvIndices);
    
  static void sphericalMapping(const std::vector<Vector3f> &vertices,
			       const std::vector<Vector3i> &vertexIndices,
			       std::vector<Vector2f> &uvs,
			       std::vector<Vector3i> &uvIndices);
  
  inline static bool isObj(const std::string &filename);

  inline static Vector3f barycenter(const std::vector<Vector3f> &vertices);
};



inline bool ObjLoader::isObj(const std::string &filename) {
  std::string tmp = std::string(filename.begin()+filename.find_last_of("."),filename.end());
  return (tmp==".obj" || tmp==".OBJ");
}

inline Vector3f ObjLoader::barycenter(const std::vector<Vector3f> &vertices) {
  Vector3f c(0,0,0);
  for(unsigned int i=0;i<vertices.size();++i) {
    c = c+vertices[i];
  }

  if(!vertices.empty()) 
    c = c/(float)vertices.size();

  return c;
}
 
#endif // OBJ_LOADER_H
