// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "imgCustom.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <iostream>

#include "misc/glutils.h"
#include "core/pbgraph.h"

using namespace std;

ImgCustomWidget::ImgCustomWidget(ImgCustomNode *node)
  : GenericCustomWidget(node) {

}

ImgCustomNode::ImgCustomNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(false,false,false,true,parent,handle) {

  _w = new ImgCustomWidget(this);

  initShaderSource();
  _w->updateWidget();
}

ImgCustomNode::~ImgCustomNode() {
}

void ImgCustomNode::apply() {
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  setOutputParams();

  enableShaders();
  drawGenericOutputs();
  disableShaders();
}

const QString ImgCustomNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec2 vertex;\n"
		     "layout(location = 1) in vec2 coord;\n");
}

const QString ImgCustomNode::defaultVertBody()   {
  return QObject::tr("out vec2 texcoord;\n\n"
		     "void main() {\n"   
		     "\tgl_Position = vec4(vertex,0,1);\n"
		     "\ttexcoord    = coord;\n"
		     "}\n");
}

const QString ImgCustomNode::constantFragHead()  {
  return QString();
}

const QString ImgCustomNode::defaultFragBody()   {
  return QObject::tr("in vec2 texcoord;\n"
		     "void main() {\n"
		     "\toutBuffer0 = texture(inBuffer0,texcoord);\n"
		     "}\n");
}

const QString ImgCustomNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 4) out;\n");
}

const QString ImgCustomNode::defaultTessCBody()  {
return QObject::tr("void main(void) {\n"
		   "\tgl_TessLevelOuter[0] = 1;\n"
		   "\tgl_TessLevelOuter[1] = 1;\n"
		   "\tgl_TessLevelOuter[2] = 1;\n"
		   "\tgl_TessLevelOuter[3] = 1;\n"
		   "\tgl_TessLevelInner[0] = 1;\n"
		   "\tgl_TessLevelInner[1] = 1;\n"
		   "\tgl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
		   "}\n");
}

const QString ImgCustomNode::constantTessEHead() {
  return QString();
}

const QString ImgCustomNode::defaultTessEBody()  {
  return QObject::tr("layout(quads,equal_spacing) in;\n\n"
		     "void main() {\n"
		     "\tvec4 p1 = mix(gl_in[0].gl_Position,gl_in[1].gl_Position,gl_TessCoord.x);\n"
		     "\tvec4 p2 = mix(gl_in[3].gl_Position,gl_in[2].gl_Position,gl_TessCoord.x);\n"
		     "\tgl_Position = mix(p1,p2,gl_TessCoord.y);\n"
		     "}\n");
}

const QString ImgCustomNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString ImgCustomNode::defaultGeomBody()   {
  return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
		     "void main() {\n"
		     "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
		     "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
		     "\tEndPrimitive();\n"
		     "}");
}
