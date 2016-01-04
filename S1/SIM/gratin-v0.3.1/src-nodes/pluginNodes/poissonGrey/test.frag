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

uniform sampler2D t;

void main(void) {
  //vec2 pixel = vec2(1.0)/vec2(textureSize(u,0)*2);
  //vec2 p2 = pixel/2.0;

  // simple access to the currentLevel+1 in the texture using bilinear interpolation
  outbuff = texture(t,coord);
}
