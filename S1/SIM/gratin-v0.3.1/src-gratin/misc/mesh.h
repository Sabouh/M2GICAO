// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef MESH_H
#define MESH_H

#include "misc/extinclude.h"

#include <vector>
#include <map>
#include <iostream>
#include <assert.h>
#include <iostream>
#include <fstream>

class LIBRARY_EXPORT Mesh {
 public:
  Mesh();
  Mesh(Mesh *mesh);
  ~Mesh();

  inline unsigned int nbVertices() const;
  inline unsigned int nbNormals()  const;
  inline unsigned int nbTangents() const;
  inline unsigned int nbUvcoords() const;
  inline unsigned int nbFaces()    const;

  inline const float *verticesPtr() const;
  inline const float *normalsPtr()  const;
  inline const float *tangentsPtr() const;
  inline const float *uvcoordsPtr() const;
  inline const int   *facesPtr()    const;

  inline std::vector<Vector3f > getVertices() const;
  inline std::vector<Vector3f > getNormals()  const;
  inline std::vector<Vector3f > getTangents() const;
  inline std::vector<Vector2f > getUvcoords() const;
  inline std::vector<Vector3i > getFaces()    const;

  inline void setVertices(const std::vector<Vector3f > &vertices);
  inline void setNormals (const std::vector<Vector3f > &normals);
  inline void setTangents(const std::vector<Vector3f > &tangents);
  inline void setUvcoords(const std::vector<Vector2f > &uvcoords);
  inline void setFaces   (const std::vector<Vector3i > &faces);

  inline void addVertex (const Vector3f &vertex);
  inline void addNormal (const Vector3f &normal);
  inline void addTangent(const Vector3f &tangent);
  inline void addUvcoord(const Vector2f &uvcoord);
  inline void addFace   (const Vector3i &face);

  inline Vector3f getVertex (unsigned int pos) const;
  inline Vector3f getNormal (unsigned int pos) const;
  inline Vector3f getTangent(unsigned int pos) const;
  inline Vector3i getFace   (unsigned int pos) const;
  inline Vector2f getUvcoord(unsigned int pos) const;

  inline void setVertex (unsigned int pos,const Vector3f &vertex);
  inline void setNormal (unsigned int pos,const Vector3f &normal);
  inline void setTangent(unsigned int pos,const Vector3f &tangent);
  inline void setUvcoord(unsigned int pos,const Vector2f &uvcoord);
  inline void setFace   (unsigned int pos,const Vector3i &face);

  inline void clearVertices(); 
  inline void clearNormals();
  inline void clearTangents();
  inline void clearUvcoords(); 
  inline void clearFaces(); 
  inline void clearAll();

  inline Vector3f getPtMin() const;
  inline Vector3f getPtMax() const;
  inline Vector3f barycenter() const;
  inline float radius() const;
  inline float averageEdgeLength() const;

  void finalize();
  void computeNormals();
  void computeTangents();


 private:
  std::vector<Vector3f > _vertices;
  std::vector<Vector3f > _normals;
  std::vector<Vector3f > _tangents;
  std::vector<Vector2f > _uvcoords;
  std::vector<Vector3i > _faces;

  Vector3f _box[2];
  Vector3f _barycenter;
  float    _radius;
  float    _avEdgeLength;

  // normal computation functions
  void computeBox();
  void computeAverageEdgeLength();
  void computeBarycenter();
  void computeRadius();

  inline Vector3f faceNormal(int);
  inline std::vector<Vector3f > faceNormals(); 

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline Vector3f Mesh::getPtMin() const {
  return _box[0];
}

inline Vector3f Mesh::getPtMax() const {
  return _box[1];
}

inline Vector3f Mesh::barycenter() const {
  return _barycenter;
}

inline float Mesh::radius() const {
  return _radius;
}

inline float Mesh::averageEdgeLength() const {
  return _avEdgeLength;
}

inline unsigned int Mesh::nbVertices() const {
  return static_cast<unsigned int>(_vertices.size());
}

inline unsigned int Mesh::nbNormals() const {
  return static_cast<unsigned int>(_normals.size());
}

inline unsigned int Mesh::nbTangents() const {
  return static_cast<unsigned int>(_tangents.size());
}

inline unsigned int Mesh::nbFaces() const {
  return static_cast<unsigned int>(_faces.size());
}

inline unsigned int Mesh::nbUvcoords() const {
  return static_cast<unsigned int>(_uvcoords.size());
}

inline const float *Mesh::verticesPtr() const {
  return _vertices.empty() ? NULL : _vertices[0].data();
}

inline const float *Mesh::normalsPtr() const {
  return _normals.empty() ? NULL : _normals[0].data();
}

inline const float *Mesh::tangentsPtr() const {
  return _tangents.empty() ? NULL : _tangents[0].data();
}

inline const int *Mesh::facesPtr() const {
  return _faces.empty() ? NULL : _faces[0].data();
}

inline const float *Mesh::uvcoordsPtr() const {
  return _uvcoords.empty() ? NULL : _uvcoords[0].data();
}

inline std::vector<Vector3f > Mesh::getVertices() const {
  return _vertices;
}

inline std::vector<Vector3f > Mesh::getNormals() const {
  return _normals;
}

inline std::vector<Vector3f > Mesh::getTangents() const {
  return _tangents;
}

inline std::vector<Vector3i > Mesh::getFaces() const {
  return _faces;
}

inline std::vector<Vector2f > Mesh::getUvcoords() const {
  return _uvcoords;
}

inline void Mesh::setVertices(const std::vector<Vector3f > &vertices) {
  _vertices.clear();
  _vertices = vertices;
}

inline void Mesh::setNormals(const std::vector<Vector3f > &normals) {
  _normals.clear();
  _normals = normals;
}

inline void Mesh::setTangents(const std::vector<Vector3f > &tangents) {
  _tangents.clear();
  _tangents = tangents;
}

inline void Mesh::setFaces(const std::vector<Vector3i > &faces) {
  _faces.clear();
  _faces = faces;
}

inline void Mesh::setUvcoords(const std::vector<Vector2f > &uvcoords) {
  _uvcoords.clear();
  _uvcoords = uvcoords;
}

inline Vector3f Mesh::getVertex(unsigned int pos) const {
  assert(pos<nbVertices());
  
  return _vertices[pos];
}

inline Vector3f Mesh::getNormal(unsigned int pos) const {
  assert(pos<nbNormals());
  
  return _normals[pos];
}

inline Vector3f Mesh::getTangent(unsigned int pos) const {
  assert(pos<nbTangents());
  
  return _tangents[pos];
}

inline Vector3i Mesh::getFace(unsigned int pos) const {
  assert(pos<nbFaces());
  
  return _faces[pos];
}

inline Vector2f Mesh::getUvcoord(unsigned int pos) const {
  assert(pos<nbUvcoords());
  
  return _uvcoords[pos];
}

inline void Mesh::addVertex(const Vector3f &vertex) {
  _vertices.push_back(vertex);
}

inline void Mesh::addNormal(const Vector3f &normal) {
  _normals.push_back(normal);
}

inline void Mesh::addTangent(const Vector3f &tangent) {
  _tangents.push_back(tangent);
}

inline void Mesh::addFace(const Vector3i &face) {
  _faces.push_back(face);
}

inline void Mesh::addUvcoord(const Vector2f &uvcoord) {
  _uvcoords.push_back(uvcoord);
}

inline void Mesh::setVertex(unsigned int pos,const Vector3f &vertex) {
  assert(pos<nbVertices());
  
  _vertices[pos] = vertex;
}

inline void Mesh::setNormal(unsigned int pos,const Vector3f &normal) {
  assert(pos<nbNormals());
  
  _normals[pos] = normal;
}

inline void Mesh::setTangent(unsigned int pos,const Vector3f &tangent) {
  assert(pos<nbTangents());
  
  _tangents[pos] = tangent;
}

inline void Mesh::setFace(unsigned int pos,const Vector3i &face) {
  assert(pos<nbFaces());
  
  _faces[pos] = face;
}

inline void Mesh::setUvcoord(unsigned int pos,const Vector2f &uvcoord) {
  assert(pos<nbUvcoords());
  
  _uvcoords[pos] = uvcoord;
}

inline void Mesh::clearVertices()  {
  _vertices.clear();
}

inline void Mesh::clearNormals() {
  _normals.clear();
}

inline void Mesh::clearTangents() {
  _tangents.clear();
}

inline void Mesh::clearFaces() {
  _faces.clear();
}

inline void Mesh::clearUvcoords() {
  _uvcoords.clear();
}

inline void Mesh::clearAll() {
  clearVertices();
  clearNormals();
  clearTangents();
  clearFaces();
  clearUvcoords();
}


inline std::vector<Vector3f> Mesh::faceNormals() {
  std::vector<Vector3f > res;

  for(unsigned int i=0;i<nbFaces();++i)
    res.push_back(faceNormal(i));

  return res;
}

inline Vector3f Mesh::faceNormal(int i) {
  const Vector3i f  = getFace(i);
  const Vector3f v1 = getVertex(f[0]);
  const Vector3f v2 = getVertex(f[1]);
  const Vector3f v3 = getVertex(f[2]);
  
  return (v1-v2).cross(v2-v3);
}




#endif //MESH_H
