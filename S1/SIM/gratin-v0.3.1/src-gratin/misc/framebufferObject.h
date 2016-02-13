// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef FRAMEBUFFER_OBJECT
#define FRAMEBUFFER_OBJECT

#include "misc/extinclude.h"
#include "misc/glutils.h"

#include <vector> 

class LIBRARY_EXPORT FramebufferObject : public GlContext {
 public:
  FramebufferObject();
  ~FramebufferObject();
  
  inline void bind();
  inline static void unbind();

  void attachTexture(GLenum tex_target, 
                     GLuint tex_id,
                     GLenum attachment = GL_COLOR_ATTACHMENT0,
                     int mip_level     = 0,
                     int z_slice       = 0);

  void attachRenderBuffer(GLuint rb_id,
                          GLenum attachment = GL_COLOR_ATTACHMENT0);
  
  void unattach(GLenum attachment);
  void unattachAll();

  bool isValid();

  GLenum getAttachedType(GLenum attachment);
  GLuint getAttachedId(GLenum attachment);
  GLint  getAttachedMipLevel(GLenum attachment);
  GLint  getAttachedCubeFace(GLenum attachment);
  GLint  getAttachedZSlice(GLenum attachment);
  
  static int getMaxColorAttachments();
  static GLenum *buffers(unsigned int i=0);

 private:
  void  unbindCurrentBindThis();
  void  unbindThisBindCurrent();

  GLuint fbo_id;
  GLint  current_fbo_id; 
};

inline void FramebufferObject::bind() {
  _glf->glBindFramebuffer(GL_FRAMEBUFFER,fbo_id);
}

inline void FramebufferObject::unbind() {
  _glf->glBindFramebuffer(GL_FRAMEBUFFER,0);
}


#endif //  FRAMEBUFFER_OBJECT
