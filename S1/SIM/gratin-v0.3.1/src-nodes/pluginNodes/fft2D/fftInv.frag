// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#version 330

layout(location = 0) out vec4 outbuff;

in vec2 texcoord;

uniform sampler2D fft;
uniform sampler2D ris;
uniform int dimension;

void main() {
  vec4 d  = texture(ris,texcoord);
  vec2 c1 = gl_FragCoord.xy;
  vec2 c2 = gl_FragCoord.xy;
  
  if(dimension==0) {
    c1.x = d.z;
    c2.x = d.w;
  } else {
    c1.y = d.z;
    c2.y = d.w;
  }

  vec2 i1 = texelFetch(fft,ivec2(c1),0).xy;
  vec2 i2 = texelFetch(fft,ivec2(c2),0).xy;

  vec2 res = vec2(d.x*i2.x+d.y*i2.y,-d.y*i2.x+d.x*i2.y)+i1;
  outbuff = vec4(0.5*res,vec2(0.,1.));
}
