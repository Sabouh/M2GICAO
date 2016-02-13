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

in vec2 texcoord;

uniform sampler2D img;
uniform int direction;
uniform int halfsize;

void main() {
  float factor = inversesqrt(-2.0*log(0.05));
  
  vec2 dir    = vec2(1-direction,direction)/textureSize(img,0);
  vec2 cdir   = dir;
  float sigma = float(halfsize)*factor;
  float fac   = -1.0/(2.0*sigma*sigma);
  float sumW  = 1.0;
  
  vec4 v = texture(img,texcoord);
  for(int r=1;r<=halfsize;r+=1) {
    vec4  left  = texture(img,texcoord-cdir);
    vec4  right = texture(img,texcoord+cdir);
    float w     = exp(fac*float(r*r));
    
    v    += w*(left+right);
    sumW += 2.0*w;
    cdir = cdir+dir; 
  } 

  outbuff = v/sumW;  
}
