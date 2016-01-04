// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EXT_INCLUDE_H
#define EXT_INCLUDE_H

#include "misc/glcontext.h"
#include <QObject>
#include <QCoreApplication>
#include <QStandardPaths>
#include <Eigen/Dense>
#include <Eigen/StdVector>

#define GRATIN_NAME "Gratin"
#define GRATIN_EXT  ".gra"
#define GRATIN_CUSTOM_EXT  ".grac"

#define GRATIN_HOME_DIR                 ( QStandardPaths::locate(QStandardPaths::HomeLocation,QString(),QStandardPaths::LocateDirectory) )
#define GRATIN_CONFIG_DIR               ( QStandardPaths::locate(QStandardPaths::GenericConfigLocation,QString(),QStandardPaths::LocateDirectory)+"/"+GRATIN_NAME )
#define GRATIN_APPLI_DIR                ( QCoreApplication::applicationDirPath() )
#define GRATIN_LOG_FILE                 ( GRATIN_CONFIG_DIR+"/gratin.log" )
#define GRATIN_NODE_PATHS_FILE          ( GRATIN_CONFIG_DIR+"/gratin.paths" )
#define GRATIN_DEFAULT_LIB_NODE_PATH    ( GRATIN_APPLI_DIR+"/nodes/lib" )
#define GRATIN_DEFAULT_CUSTOM_NODE_PATH ( GRATIN_APPLI_DIR+"/nodes/custom" )
#define GRATIN_APPLI_DATA_PATH          ( GRATIN_APPLI_DIR+"/data" )
#define GRATIN_USER_DATA_PATH           ( GRATIN_HOME_DIR )

#if defined(LIBRARY)
#  define LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define LIBRARY_EXPORT Q_DECL_IMPORT
#endif

using namespace Eigen;

#ifndef EIGEN_VECTOR_U
#define EIGEN_VECTOR_U
typedef Matrix< unsigned int , 2 , 1> Vector2u;
typedef Matrix< unsigned int , 3 , 1> Vector3u;
typedef Matrix< unsigned int , 4 , 1> Vector4u;
#endif

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector2u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector3u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4i)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4u)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXi)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXf)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(VectorXd)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix2d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix3d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix4f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Matrix4d)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(MatrixXf)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(MatrixXd)

#ifndef log2
#define log2(x) (log(x)/log(2))
#endif

#ifdef _WIN32
#define OPENEXR_DLL
#pragma warning(disable : 4996) // disable warnings for sscanf and sprintf
#endif

#endif // EXT_INCLUDE_H
