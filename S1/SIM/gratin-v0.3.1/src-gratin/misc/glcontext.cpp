// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/glcontext.h"
#include <iostream>

using namespace std;

QOpenGLContext     *GlContext::_glcontext = NULL;
GlContext::GlFuncs *GlContext::_glf       = NULL;
GLenum              GlContext::_glerr     = 0;

QSurfaceFormat GlContext::surfaceFormat() {
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setMajorVersion(OPENGL_MAJOR_VERSION);
  format.setMinorVersion(OPENGL_MINOR_VERSION);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setDepthBufferSize(24);
  format.setSamples(1);
  
  return format;
}

void GlContext::initOpenGLContext(QWindow *window) {
  if(_glcontext) return; // context already created

  // create context 
  _glcontext = new QOpenGLContext();

  if(!_glcontext) {
    cerr << "Error: OpenGL context initialization failed! -- exiting..." << endl;
    exit(1);
  }

  _glcontext->setFormat(surfaceFormat());

  if(!_glcontext->create()) {
    cerr << "Error: OpenGL context initialization failed! -- exiting..." << endl;
    exit(1);
  }

  _glcontext->makeCurrent(window);

  _glf = _glcontext->versionFunctions<GlFuncs>();
  
  if(!_glf) {
    cerr << "Error: OpenGL " << OPENGL_MAJOR_VERSION << "." << OPENGL_MINOR_VERSION << " functions initialization failed! -- exiting..." << endl;
    exit(1);
  } else {
    cout << "OpenGL " << OPENGL_MAJOR_VERSION << "." << OPENGL_MINOR_VERSION << " context successfully created" << endl;
  }

  _glf->initializeOpenGLFunctions();
}

void GlContext::cleanOpenGLContext() {
  //delete _glf; _glf = NULL;
  delete _glcontext; _glcontext = NULL;
}
