// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef TEXTURE_FORMAT_H
#define TEXTURE_FORMAT_H

#include "misc/extinclude.h"

class LIBRARY_EXPORT TextureFormat {
 public:
  static const int MIPMAP_MANUAL    = 0;
  static const int MIPMAP_FBO_AUTOM = 1;
  
  TextureFormat(GLenum  target         = GL_TEXTURE_2D,
		GLsizei width          = 0,
		GLsizei height         = 0,
		GLenum  internalformat = GL_RGBA,
		GLenum  format         = GL_RGBA,
		GLenum  type           = GL_FLOAT,
		GLsizei depth          = 0,
		int     mipmapmode     = MIPMAP_MANUAL,
		GLint   level          = 0,
		GLint   border         = 0);
  
  TextureFormat(const TextureFormat &tf);
  
  inline void setTarget        (GLenum  target        ) {_target         = target;        } 
  inline void setWidth         (GLsizei width         ) {_width          = width;         }
  inline void setHeight        (GLsizei height        ) {_height         = height;        }
  inline void setInternalformat(GLenum  internalformat) {_internalformat = internalformat;}
  inline void setFormat        (GLenum  format        ) {_format         = format;        }
  inline void setType          (GLenum  type          ) {_type           = type;          }
  inline void setDepth         (GLsizei depth         ) {_depth          = depth;         }
  inline void setmipmapmode    (int     mipmapmode    ) {_mipmapmode     = mipmapmode;    }
  inline void setLevel         (GLint   level         ) {_level          = level;         }
  inline void setBorder        (GLint   border        ) {_border         = border;        }
  
  inline GLenum  target        () const {return _target;        }
  inline GLsizei width         () const {return _width;         }
  inline GLsizei height        () const {return _height;        }
  inline GLenum  internalformat() const {return _internalformat;}
  inline GLenum  format        () const {return _format;        }
  inline GLenum  type          () const {return _type;          }
  inline GLsizei depth         () const {return _depth;         }
  inline int     mipmapmode    () const {return _mipmapmode;    }
  inline GLint   level         () const {return _level;         }
  inline GLint   border        () const {return _border;        }
  
 protected:
  GLenum  _target;
  GLsizei _width;
  GLsizei _height;
  GLenum  _internalformat;
  GLenum  _format;
  GLenum  _type;
  GLsizei _depth;
  int     _mipmapmode;
  GLint   _level;
  GLint   _border;
};

class LIBRARY_EXPORT TextureParams {
 public:
  TextureParams(const TextureParams &tp);
  TextureParams(GLint minfilter = GL_LINEAR,
                GLint maxfilter = GL_LINEAR,
                GLint wrapr     = GL_CLAMP_TO_EDGE,
                GLint wraps     = GL_CLAMP_TO_EDGE,
                GLint wrapt     = GL_CLAMP_TO_EDGE,
                GLint mode      = GL_REPLACE);

  ~TextureParams() {}

  inline void setMinfilter(GLint minfilter) {_minfilter = minfilter;}
  inline void setMaxfilter(GLint maxfilter) {_maxfilter = maxfilter;}
  inline void setWrapr    (GLint wrapr    ) {_wrapr     = wrapr;    }
  inline void setWraps    (GLint wraps    ) {_wraps     = wraps;    }
  inline void setWrapt    (GLint wrapt    ) {_wrapt     = wrapt;    }
  inline void setMode     (GLint mode     ) {_mode      = mode;     }

  inline GLint minfilter() const {return _minfilter;}
  inline GLint maxfilter() const {return _maxfilter;}
  inline GLint wrapr    () const {return _wrapr;    }
  inline GLint wraps    () const {return _wraps;    }
  inline GLint wrapt    () const {return _wrapt;    }
  inline GLint mode     () const {return _mode;     }

 protected:
  GLint _minfilter;
  GLint _maxfilter;
  GLint _wrapr;
  GLint _wraps;
  GLint _wrapt;
  GLint _mode;
};



#endif // TEXTURE_FORMAT_H
