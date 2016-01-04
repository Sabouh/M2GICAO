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

in vec2  texcoord;

uniform vec4  backgroundcol;
uniform vec4  linecol;
uniform bool  selected;
uniform float roundness;

void main() {   
  vec2 c = texcoord-vec2(0.5);

  const float eps = 0.1;
  const float r = 0.8;
  const float l = 0.5;

  vec4 color = backgroundcol;

  if(selected) {
    float s = smoothstep(-0.1,0.1,sin(20.0*(c.x+c.y)))-0.5;
    color.xyz = clamp(color.xyz+color.xyz*s,vec3(0),vec3(1));
  }

  float d   = pow(abs(c.x/l),roundness)+pow(abs(c.y/l),roundness)-r;

  float sdc = step(0.0,d);
  float sdl = smoothstep(eps-0.1,eps+0.1,abs(d));

  outbuff = mix(color,vec4(0),sdc);
  outbuff = mix(linecol,outbuff,sdl);
}
