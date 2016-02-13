// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/framebufferObject.h"
#include <iostream>
#include <assert.h>

using namespace std;

std::vector<GLenum> _buffers;

FramebufferObject::FramebufferObject()
  : fbo_id(0) {
  _glf->glGenFramebuffers(1,&fbo_id);
}

FramebufferObject::~FramebufferObject() {
  _glf->glDeleteFramebuffers(1,&fbo_id);
}

void FramebufferObject::attachTexture(GLenum tex_target,GLuint tex_id, 
                                      GLenum attachment,int mip_level,int z_slice) {
  unbindCurrentBindThis();
  
  _glf->glBindTexture(tex_target,tex_id);

  if(tex_target==GL_TEXTURE_1D)
    _glf->glFramebufferTexture1D(GL_FRAMEBUFFER,attachment,
				 GL_TEXTURE_1D,tex_id,mip_level);
  else if(tex_target == GL_TEXTURE_3D)
    _glf->glFramebufferTexture3D(GL_FRAMEBUFFER,attachment,
				 GL_TEXTURE_3D,tex_id,mip_level,z_slice);
  else
    _glf->glFramebufferTexture2D(GL_FRAMEBUFFER,attachment,
				 tex_target,tex_id,mip_level);
  
  unbindThisBindCurrent();
}

void FramebufferObject::attachRenderBuffer(GLuint buff_id,GLenum attachment) {  
  unbindCurrentBindThis();

  _glf->glFramebufferRenderbuffer(GL_FRAMEBUFFER,attachment,
				  GL_RENDERBUFFER,buff_id);
  
  unbindThisBindCurrent();
}

void FramebufferObject::unattach(GLenum attachment) {
  unbindCurrentBindThis();

  GLenum type = getAttachedType(attachment);
  switch(type){
  case GL_RENDERBUFFER:
    attachRenderBuffer(0, attachment);
    break;
  case GL_TEXTURE:
    attachTexture(GL_TEXTURE_2D, 0, attachment);
    break;
  default:
    break;
  }

  unbindThisBindCurrent();
}

void FramebufferObject::unattachAll() {
  int nb_attachments = getMaxColorAttachments();
  for(int i=0;i<nb_attachments;i++)
    unattach(GL_COLOR_ATTACHMENT0+i);
}

GLint FramebufferObject::getMaxColorAttachments() {
  GLint max_attach = 0;
  _glf->glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &max_attach);
  return max_attach;
}

void FramebufferObject::unbindCurrentBindThis() {
  _glf->glGetIntegerv(GL_FRAMEBUFFER_BINDING,&current_fbo_id);
  if(fbo_id != (GLuint)current_fbo_id)
    _glf->glBindFramebuffer(GL_FRAMEBUFFER,fbo_id);
}

void FramebufferObject::unbindThisBindCurrent() {
  if(fbo_id != (GLuint)current_fbo_id)
    _glf->glBindFramebuffer(GL_FRAMEBUFFER,(GLuint)current_fbo_id);
}

bool FramebufferObject::isValid() {
  unbindCurrentBindThis();

  bool res = false;

  GLenum status;                                            
  status = _glf->glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch(status){                                          
  case GL_FRAMEBUFFER_COMPLETE: // Everything's OK
    res = true;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
    res = false;
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "GL_FRAMEBUFFER_UNSUPPORTED\n";
    res = false;
    break;
  default:
    cerr << "glift::CheckFramebufferStatus() ERROR:\n\t"
         << "Unknown ERROR\n";
    res = false;
  }

  unbindThisBindCurrent();
  return res;
}

GLenum FramebufferObject::getAttachedType(GLenum attachment) {
  // GL_RENDERBUFFER or GL_TEXTURE
  unbindCurrentBindThis();

  GLint type = 0;
  _glf->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
					      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
					      &type);
  unbindThisBindCurrent();

  return GLenum(type);
}

GLuint FramebufferObject::getAttachedId(GLenum attachment) {
  unbindCurrentBindThis();

  GLint id = 0;
  _glf->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
					      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
					      &id);
  unbindThisBindCurrent();

  return GLuint(id);
}

GLint FramebufferObject::getAttachedMipLevel(GLenum attachment) {
  unbindCurrentBindThis();

  GLint level = 0;
  _glf->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
					      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
					      &level);
  unbindThisBindCurrent();

  return level;
}

GLint FramebufferObject::getAttachedCubeFace(GLenum attachment) {
  unbindCurrentBindThis();

  GLint level = 0;
  _glf->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
					      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
					      &level);
  unbindThisBindCurrent();

  return level;
}

GLint FramebufferObject::getAttachedZSlice(GLenum attachment) {
  unbindCurrentBindThis();

  GLint slice = 0;
  _glf->glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
					      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT,
					      &slice);
  unbindThisBindCurrent();

  return slice;
}

GLenum *FramebufferObject::buffers(unsigned int i) {
  if(_buffers.empty()) {
    for(int j=0;j<getMaxColorAttachments();++j) {
      _buffers.push_back(GL_COLOR_ATTACHMENT0+j);
    }
  }
    
  assert((int)i<getMaxColorAttachments());
    
  return &(_buffers[i]);
}
  
