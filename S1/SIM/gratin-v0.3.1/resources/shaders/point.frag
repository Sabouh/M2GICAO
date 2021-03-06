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

uniform vec3 color;

void main() {   
  float l = min(distance(gl_PointCoord,vec2(0.5)),1.0);
  float a = 1.0-smoothstep(0.3,0.4,l);
  outbuff = vec4(color,a);
}
