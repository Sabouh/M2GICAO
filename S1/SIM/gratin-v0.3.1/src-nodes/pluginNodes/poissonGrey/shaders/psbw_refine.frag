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

uniform sampler2D f; 
uniform sampler2D u;

uniform float x;
uniform float c;
uniform float e;
uniform float t;

void main(void) {
  // current solution for this level
  vec4 fi = texture(f,coord);
  
  // jacobi iteration
  vec4 lui = (texture(u,coord)*x +
	      (textureOffset(u,coord,ivec2( 1, 1)) +
	       textureOffset(u,coord,ivec2(-1, 1)) +
	       textureOffset(u,coord,ivec2( 1,-1)) +
	       textureOffset(u,coord,ivec2(-1,-1)))*c +
	      (textureOffset(u,coord,ivec2( 1, 0)) +
	       textureOffset(u,coord,ivec2(-1, 0)) +
	       textureOffset(u,coord,ivec2( 0, 1)) +
	       textureOffset(u,coord,ivec2( 0,-1)))*e );

  // previous version
  // refine... except if we are on a constraint
  //outbuff = mix((fi-lui)*t,fi.wwww,step(0.0,fi.w));

  // new version
  float s = step(0.5,fi.w);
  float r = mix((fi.x-lui.x)*t,fi.z,s);
  outbuff = vec4(r,0.,fi.z,s);
}
