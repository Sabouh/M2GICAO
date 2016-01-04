// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) out vec4 outbuff;

in vec2 coord;

uniform sampler2D gxy; 

void main(void) {
  vec4  g = texture(gxy,coord);
  float f = (g.x-textureOffset(gxy,coord,ivec2(-1, 0)).x +
  	     g.y-textureOffset(gxy,coord,ivec2( 0,-1)).y);

  // previous version
  //outbuff = vec4(f,f,f,g.w);

  // new version
  outbuff = vec4(f,0.,g.z,g.w);
}
