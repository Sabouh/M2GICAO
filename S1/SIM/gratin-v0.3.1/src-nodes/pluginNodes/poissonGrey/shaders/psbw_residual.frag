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
uniform sampler2D u;

uniform float m;
uniform float c;
uniform float e;

void main(void) {
  vec4  g = texture(gxy,coord);
  float f = (g.x-textureOffset(gxy,coord,ivec2(-1, 0)).x +
  	     g.y-textureOffset(gxy,coord,ivec2( 0,-1)).y);

  // // residual (beginning: u=0), then res=f
  float ucurrent = texture(u,coord).x;
  float r = (ucurrent*m +
  	     (textureOffset(u,coord,ivec2( 1, 1)).x +
  	      textureOffset(u,coord,ivec2(-1, 1)).x +
  	      textureOffset(u,coord,ivec2( 1,-1)).x +
  	      textureOffset(u,coord,ivec2(-1,-1)).x)*c +
  	     (textureOffset(u,coord,ivec2( 1 ,0)).x +
  	      textureOffset(u,coord,ivec2(-1, 0)).x +
  	      textureOffset(u,coord,ivec2( 0, 1)).x +
  	      textureOffset(u,coord,ivec2( 0,-1)).x)*e);

  float fr = f-r;

  // previous version
  // Dirichlet constraints 
  //float dc = mix(-1.0,g.w-ucurrent,step(0.0,g.w));
  //outbuff = vec4(fr,fr,fr,dc);

  // new version
  outbuff = vec4(fr,0.,g.z-ucurrent,g.w);
}
