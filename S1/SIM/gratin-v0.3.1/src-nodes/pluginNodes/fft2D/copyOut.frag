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

uniform sampler2D img;
uniform bool realimag;

void main() {
  vec2 c = mod(texcoord,1.);
  vec4 d = texture(img,c);
  vec2 r = realimag ? d.xy : vec2(length(d.xy),atan(d.y,d.x));
  outbuff = vec4(r,0.,1.);
}
