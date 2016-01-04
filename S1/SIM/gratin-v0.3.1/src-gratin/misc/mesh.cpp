// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/mesh.h"
#include <iostream>

using namespace std;

Mesh::Mesh()
  : _barycenter(0,0,0),
    _radius(0),
    _avEdgeLength(0) {
  _box[0] = _box[1] = Vector3f(0,0,0);
}

Mesh::Mesh(Mesh *mesh) {
  _vertices = mesh->getVertices();
  _normals  = mesh->getNormals();
  _tangents = mesh->getTangents();
  _uvcoords = mesh->getUvcoords();
  _faces    = mesh->getFaces();

  _box[0] = mesh->getPtMin();
  _box[1] = mesh->getPtMax();

  _barycenter   = mesh->barycenter();
  _radius       = mesh->radius();
  _avEdgeLength = mesh->averageEdgeLength();
}

Mesh::~Mesh() {
  clearAll();
}

void Mesh::finalize() {
  computeBox();
  computeAverageEdgeLength();
  computeBarycenter();
  computeRadius();
}

void Mesh::computeNormals() {
  clearNormals();

  // get face normals 
  vector<Vector3f > fn = faceNormals();
  
  // init normals 
  _normals = vector<Vector3f >(nbVertices());
  for(unsigned int i=0;i<nbVertices();++i) {
    _normals[i] = Vector3f(0,0,0);
  }

  // need a counter for averaging
  vector<float> counter(nbVertices(),0.0f);
  
  // sum of all faces 
  for(unsigned int i=0;i<nbFaces();++i) {
    const Vector3i f = _faces[i]; 
    const Vector3f n = fn[i]; 

    // for each vertex of the current face 
    for(unsigned int j=0;j<3;++j) {
      _normals[f[j]] += n; 
      counter[f[j]] ++;
    }
  }

  // normalization
  for(unsigned int i=0;i<nbVertices();++i) {
    _normals[i] = _normals[i].normalized();
  }

  fn.clear();
  counter.clear();
}

void Mesh::computeTangents() {
  clearTangents();
  
  if(nbUvcoords()!=nbVertices()) {
    clearUvcoords();
    cout << "Unable to compute tangents: uv array is empty..." << endl;
    return;
  }

  // init tangents
  _tangents = vector<Vector3f >(nbVertices());
  for(unsigned int i=0;i<nbVertices();++i) {
    _tangents[i] = Vector3f(0,0,0);
  }

  // iterate over faces 
  for(unsigned int i=0;i<nbFaces();++i) {
    const Vector3i f  = getFace(i);
    
    const Vector3f v1 = getVertex(f[0]);
    const Vector3f v2 = getVertex(f[1]);
    const Vector3f v3 = getVertex(f[2]);
    
    const Vector2f w1 = getUvcoord(f[0]);
    const Vector2f w2 = getUvcoord(f[1]);
    const Vector2f w3 = getUvcoord(f[2]);
    
    const float x1 = v2[0] - v1[0];
    const float x2 = v3[0] - v1[0];
    const float y1 = v2[1] - v1[1];
    const float y2 = v3[1] - v1[1];
    const float z1 = v2[2] - v1[2];
    const float z2 = v3[2] - v1[2];
    
    const float s1 = w2[0] - w1[0];
    const float s2 = w3[0] - w1[0];
    const float t1 = w2[1] - w1[1];
    const float t2 = w3[1] - w1[1];
    
    const float r = 1.0f/(s1*t2 - s2*t1);
    
    for(unsigned int j=0;j<3;++j) {
      Vector3f t = getTangent(f[j]);
      t[0] += (t2*x1-t1*x2)*r;
      t[1] += (t2*y1-t1*y2)*r;
      t[2] += (t2*z1-t1*z2)*r;
      setTangent(f[j],t);
    }
  }

  // normalize and project tangents 
  for(unsigned int i=0;i<nbVertices();++i) {
    Vector3f t = getTangent(i);
    Vector3f n = getNormal(i);
    
    // project
    float r = n[0]*t[0]+n[1]*t[1]+n[2]*t[2];
    t = t-n*r;
    t = t.normalized();
    setTangent(i,t);
  }
}


void Mesh::computeAverageEdgeLength() {
  Vector3i  f;
  float  d = 0.0;
  float  n = 0.0;

  for(unsigned int i=0;i<nbFaces();++i) {
    f  = getFace(i);
    
    d += (getVertex(f[1])-getVertex(f[0])).norm();
    d += (getVertex(f[2])-getVertex(f[1])).norm();
    d += (getVertex(f[0])-getVertex(f[2])).norm();

    n += 3.0;
  }

  _avEdgeLength = d/n;
}

void Mesh::computeBox() {
  if(nbVertices()==0) {
    _box[0] = _box[1] = Vector3f(0.0,0.0,0.0);
    return;
  }

  Vector3f v;
  _box[0] = _box[1] = getVertex(0);
  
  for(unsigned int i=1;i<nbVertices();++i) {

    v = getVertex(i);
    _box[0][0] = v[0]<_box[0][0] ? v[0] : _box[0][0];
    _box[0][1] = v[1]<_box[0][1] ? v[1] : _box[0][1];
    _box[0][2] = v[2]<_box[0][2] ? v[2] : _box[0][2];
    
    _box[1][0] = v[0]>_box[1][0] ? v[0] : _box[1][0];
    _box[1][1] = v[1]>_box[1][1] ? v[1] : _box[1][1];
    _box[1][2] = v[2]>_box[1][2] ? v[2] : _box[1][2];
  }
}

void Mesh::computeBarycenter() {
  Vector3f b = Vector3f(0.0,0.0,0.0);
    
  for(unsigned int i=0;i<nbVertices();++i) {
    b += getVertex(i);
  }
    
  b /= ((float)nbVertices());
    
  _barycenter = b;
}

void Mesh::computeRadius() {
  _radius = 0.0f;

  for(unsigned int i=0;i<nbVertices();++i) {
    const float d = (getVertex(i)-_barycenter).norm();
    if(d>_radius) _radius = d;
  }
}
