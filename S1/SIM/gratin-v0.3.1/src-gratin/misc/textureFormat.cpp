// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "misc/textureFormat.h"


TextureFormat::TextureFormat(GLenum  target,
			     GLsizei width,
			     GLsizei height,
			     GLenum  internalformat,
			     GLenum  format,
			     GLenum  type,
			     GLsizei depth,
			     int     mipmapmode,
			     GLint   level,
			     GLint   border)
  : _target(target),
    _width(width),
    _height(height),
    _internalformat(internalformat),
    _format(format),
    _type(type),
    _depth(depth),
    _mipmapmode(mipmapmode),
    _level(level),
    _border(border) {
  
  
    }

TextureFormat::TextureFormat(const TextureFormat &tf)
  : _target(tf.target()),
    _width(tf.width()),
    _height(tf.height()),
    _internalformat(tf.internalformat()),
    _format(tf.format()),
    _type(tf.type()),
    _depth(tf.depth()),
    _mipmapmode(tf.mipmapmode()),
    _level(tf.level()),
    _border(tf.border()) {
  
}

TextureParams::TextureParams(GLint minfilter,
			     GLint maxfilter,
			     GLint wrapr,
			     GLint wraps,
			     GLint wrapt,
			     GLint mode)
  : _minfilter(minfilter),
    _maxfilter(maxfilter),
    _wrapr(wrapr),
    _wraps(wraps),
    _wrapt(wrapt),
    _mode(mode) {

  }

TextureParams::TextureParams(const TextureParams &tp)
  : _minfilter(tp.minfilter()),
    _maxfilter(tp.maxfilter()),
    _wrapr(tp.wrapr()),
    _wraps(tp.wraps()),
    _wrapt(tp.wrapt()),
    _mode(tp.mode()) {

}


