// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#if defined(LIBRARY)
#  define LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#ifndef OPENGL_MAJOR_VERSION
  #define OPENGL_MAJOR_VERSION 4
#endif
#ifndef OPENGL_MINOR_VERSION
  #define OPENGL_MINOR_VERSION 2
#endif

#define OPENGL_CORE_FUNCS_HELPER2(a, b) QOpenGLFunctions_##a##_##b##_Core
#define OPENGL_CORE_FUNCS_HELPER(a, b) OPENGL_CORE_FUNCS_HELPER2(a, b)
#define OPENGL_CORE_FUNCS \
  OPENGL_CORE_FUNCS_HELPER(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION)
#define QUOTE(a) #a
#define QUOTE_AND_EXPAND(a) QUOTE(a)
#define OPENGL_CORE_FUNCS_INCLUDE QUOTE_AND_EXPAND(OPENGL_CORE_FUNCS)


#include <QWindow>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include OPENGL_CORE_FUNCS_INCLUDE

#define CHECK_GL_ERROR							\
  if((_glerr = _glf->glGetError()) != GL_NO_ERROR)			\
    {									\
      std::cout << "OpenGL error at " << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__  << ": "; \
      switch(_glerr) {							\
      case GL_INVALID_ENUM: std::cout << "invalid enum" << std::endl;break; \
      case GL_INVALID_VALUE: std::cout << "invalid value" << std::endl;break; \
      case GL_INVALID_OPERATION: std::cout << "invalid operation" << std::endl;break; \
      case GL_STACK_OVERFLOW: std::cout << "stack overflow" << std::endl;break; \
      case GL_STACK_UNDERFLOW: std::cout << "stack underflow" << std::endl;break; \
      case GL_INVALID_FRAMEBUFFER_OPERATION: std::cout << "invalid framebuffer operation" << std::endl;break; \
      case GL_OUT_OF_MEMORY: std::cout << "out of memory" << std::endl;break; \
      default: std::cout << "unknown error (" << _glerr << ")" << std::endl;break; \
      }									\
    }									\




// initialize OpenGL context, surfaces and functions 
// any class that uses OpenGL functions may inherit this one
// OpenGL functions will then be available through the _glf variable
class LIBRARY_EXPORT GlContext {
 public:
  typedef OPENGL_CORE_FUNCS GlFuncs;

  static void initOpenGLContext(QWindow *window); 
  static void cleanOpenGLContext();

  static QSurfaceFormat  surfaceFormat();
  static QOpenGLContext *context()     {return _glcontext;}
  static GlFuncs        *glFunctions() {return _glf;}
  static void            makeCurrent() {_glcontext->makeCurrent(_glcontext->surface());}

 protected:
  static QOpenGLContext *_glcontext;
  static GlFuncs        *_glf;
  static GLenum          _glerr;
};

#endif // GL_CONTEXT_H
