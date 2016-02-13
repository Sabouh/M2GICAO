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

void main(void) {
 
  vec4 fvals[4];
  vec2 p2 = vec2(1.0)/vec2(textureSize(f,0)*2);
  fvals[0] = texture(f,coord+vec2(-p2.x,-p2.y));
  fvals[1] = texture(f,coord+vec2(-p2.x, p2.y));
  fvals[2] = texture(f,coord+vec2( p2.x, p2.y));
  fvals[3] = texture(f,coord+vec2( p2.x,-p2.y));

  // previous version
  // compute color contraints
  // float sum = 0.0;
  // float nb  = 0.0;
  // for(int i=0;i<4;++i) {
  //   sum += max(0.0,fvals[i].w);
  //   nb  += step(0.0,fvals[i].w);
  // }

  // float snb   = sign(nb); 
  // outbuff.xyz = fvals[0].xyz+fvals[1].xyz+fvals[2].xyz+fvals[3].xyz;
  // outbuff.w   = mix(-1.0,sum/(nb+1.0-snb),snb);

  // new version
  float sum = 0.;
  float nb = 0.;
  for(int i=0;i<4;++i) {
    if(fvals[i].w>0.5) {
      sum += fvals[i].z;
      nb ++;
    }
  }

  float snb = sign(nb); 
  outbuff = vec4(fvals[0].x+fvals[1].x+fvals[2].x+fvals[3].x,
		 0.,sum/(nb+1.0-snb),snb);
}
