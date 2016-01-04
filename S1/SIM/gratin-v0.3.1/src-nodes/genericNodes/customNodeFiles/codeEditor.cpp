// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "codeEditor.h"
#include <QTextBlock>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <iostream>
#include <cmath>

using namespace std;

GLSLSyntaxHighLighter::GLSLSyntaxHighLighter(QTextDocument* document)
  : QSyntaxHighlighter(document) {	  


  _builtinList.setPattern("\\b(abs|acos|acosh|all|any|asin|asinh|atan|atanh|atomicCounter|atomicCounterDecrement|atomicCounterIncrement|barrier|bitCount|bitfieldExtract|bitfieldInsert|bitfieldReverse|ceil|clamp|cos|cosh|cross|degrees|determinant|dFdx|dFdy|distance|dot|EmitStreamVertex|EmitVertex|EndPrimitive|EndStreamPrimitive|equal|exp|exp2|faceforward|findLSB|findMSB|floatBitsToInt|floatBitsToUint|floor|fma|fract|frexp|fwidth|greaterThan|greaterThanEqual|imageAtomicAdd|imageAtomicAnd|imageAtomicCompSwap|imageAtomicExchange|imageAtomicMax|imageAtomicMin|imageAtomicOr|imageAtomicXor|imageLoad|imageSize|imageStore|imulExtended|intBitsToFloat|interpolateAtCentroid|interpolateAtOffset|interpolateAtSample|inverse|inversesqrt|isinf|isnan|ldexp|length|lessThan|lessThanEqual|log|log2|matrixCompMult|max|memoryBarrier|min|mix|mod|modf|noise|normalize|not|notEqual|outerProduct|packDouble2x32|packHalf2x16|packSnorm2x16|packSnorm4x8|packUnorm2x16|packUnorm4x8|pow|radians|reflect|refract|round|roundEven|sign|sin|sinh|smoothstep|sqrt|step|tan|tanh|texelFetch|texelFetchOffset|texture|textureGather|textureGatherOffset|textureGatherOffsets|textureGrad|textureGradOffset|textureLod|textureLodOffset|textureOffset|textureProj|textureProjGrad|textureProjGradOffset|textureProjLod|textureProjLodOffset|textureProjOffset|textureQueryLevels|textureQueryLod|textureSize|transpose|trunc|uaddCarry|uintBitsToFloat|umulExtended|unpackDouble2x32|unpackHalf2x16|unpackSnorm2x16|unpackSnorm4x8|unpackUnorm2x16|unpackUnorm4x8|usubBorrow)\\b");


  _typeList.setPattern("\\b(float|double|int|void|bool|true|false|mat2|mat3|mat4|dmat2|dmat3|dmat4|mat2x2|mat2x3|mat2x4|dmat2x2|dmat2x3|dmat2x4|mat3x2|mat3x3|mat3x4|dmat3x2|dmat3x3|dmat3x4|mat4x2|mat4x3|mat4x4|dmat4x2|dmat4x3|dmat4x4|vec2|vec3|vec4|ivec2|ivec3|ivec4|bvec2|bvec3|bvec4|dvec2|dvec3|dvec4|uint|uvec2|uvec3|uvec4|sampler1D|sampler2D|sampler3D|samplerCube|sampler1DShadow|sampler2DShadow|samplerCubeShadow|sampler1DArray|sampler2DArray|sampler1DArrayShadow|sampler2DArrayShadow|isampler1D|isampler2D|isampler3D|isamplerCube|isampler1DArray|isampler2DArray|usampler1D|usampler2D|usampler3D|usamplerCube|usampler1DArray|usampler2DArray|sampler2DRect|sampler2DRectShadow|isampler2DRect|usampler2DRect|samplerBuffer|isamplerBuffer|usamplerBuffer|sampler2DMS|isampler2DMS|usampler2DMS|sampler2DMSArray|isampler2DMSArray|usampler2DMSArray|samplerCubeArray|samplerCubeArrayShadow|isamplerCubeArray|usamplerCubeArray|image1D|iimage1D|uimage1D|image2D|iimage2D|uimage2D|image3D|iimage3D|uimage3D|image2DRect|iimage2DRect|uimage2DRect|imageCube|iimageCube|uimageCube|imageBuffer|iimageBuffer|uimageBuffer|image1DArray|iimage1DArray|uimage1DArray|image2DArray|iimage2DArray|uimage2DArray|imageCubeArray|iimageCubeArray|uimageCubeArray|image2DMS|iimage2DMS|uimage2DMS|image2DMSArray|iimage2DMSArray|uimage2DMSArray|long|short|half|fixed|unsigned|hvec2|hvec3|hvec4|fvec2|fvec3|fvec4|sampler3DRect)\\b");


  _modifierList.setPattern("\\b(attribute|const|uniform|varying|buffer|shared|coherent|volatile|restrict|readonly|writeonly|atomic_uint|layout|centroid|flat|smooth|noperspective|patch|sample|break|continue|do|for|while|switch|case|default|if|else|subroutine|in|out|inout|invariant|discard|return|lowp|mediump|highp|precision|struct|common|partition|active|asm|class|union|enum|typedef|template|this|packed|resource|goto|inline|noinline|public|static|extern|external|interface|superp|input|output|filter|sizeof|cast|namespace|using|row_major|early_fragment_tests)\\b");

  _preprocessList.setPattern("\\b(define|undef|if|ifdef|ifndef|else|elif|endif|error|pragma|extension|version|line)\\b");
  
  _blockSeparatorList.setPattern("[|{|(|)|}|]|;|,");

  _builtinFormat.setFontItalic(true);
  _builtinFormat.setForeground(QColor(110,135,225));
  _typeFormat.setForeground(QColor(185,115,235));
  _modifierFormat.setForeground(Qt::darkGreen);
  _blockSeparatorFormat.setForeground(Qt::blue);
  _preprocessFormat.setFontWeight(QFont::Bold);
  _preprocessFormat.setForeground(Qt::gray);
  _commentFormat.setForeground(Qt::red);
}

void GLSLSyntaxHighLighter::modifyBlockFormat(const QString &text,QRegExp &regexp,const QTextCharFormat &f) {
  int index = text.indexOf(regexp);
  while(index>=0) {
    const int length = regexp.matchedLength();
    setFormat(index,length,f);
    index = text.indexOf(regexp,index+length);
  }
}

void GLSLSyntaxHighLighter::modifyComments(const QString &text,QRegExp &regexp,const QTextCharFormat &f) {
  int index = text.indexOf(regexp);
  if(index>=0) {
    setFormat(index,text.length()-index,f);
  }
}


void GLSLSyntaxHighLighter::highlightBlock(const QString &text) {

  QRegExp commentList("//");
  modifyBlockFormat(text,_preprocessList,_preprocessFormat);
  modifyBlockFormat(text,_builtinList,_builtinFormat);
  modifyBlockFormat(text,_typeList,_typeFormat);
  modifyBlockFormat(text,_modifierList,_modifierFormat);
  modifyBlockFormat(text,_blockSeparatorList,_blockSeparatorFormat);
  modifyComments(text,commentList,_commentFormat);
}

CodeEditor::CodeEditor(QWidget *parent,bool readOnly,unsigned int lineShift) 
  : QPlainTextEdit(parent),
    _lineShift(lineShift) {


  setMinimumHeight(200);
  setTabChangesFocus(false);
  setTabStopWidth(16);
  setReadOnly(readOnly);

  _lineNumberWidget = new LineNumberWidget(this);
  _glslHighlighter  = new GLSLSyntaxHighLighter(document());
  
  connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(updateLineNumberWidth()));
  connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(resizeNeeded()));
  connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(updateLineNumber(QRect,int)));


  updateLineNumberWidth();
}

void CodeEditor::setLineShift(unsigned int lineShift) {
  _lineShift = lineShift;
  repaint();
}

void CodeEditor::lineNumberPaintEvent(QPaintEvent *event) {
  QPainter painter(_lineNumberWidget);
  painter.fillRect(event->rect(),QColor(230,230,230));
  
  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber()+_lineShift;
  int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int)blockBoundingRect(block).height();
  
  while(block.isValid() && top<=event->rect().bottom()) {
    if(block.isVisible() && bottom>=event->rect().top()) {
      QString number = QString::number(blockNumber);
      painter.setPen(Qt::gray);
      painter.drawText(0,top,_lineNumberWidget->width(),fontMetrics().height(),Qt::AlignRight,number);
    }
    
    block = block.next();
    top = bottom;
    bottom = top + (int)blockBoundingRect(block).height();
    blockNumber++;
  }
}

int CodeEditor::lineNumberWidth() {
  const int nb = max((int)ceil(log10((double)(blockCount()+_lineShift))),1);
  return 3+nb*fontMetrics().width(QLatin1Char('9'));
}

void CodeEditor::updateLineNumberWidth() {
  setViewportMargins(lineNumberWidth(),0,0,0);
}

void CodeEditor::resizeNeeded() {
  //setMaximumHeight(contentsRect().height());
}

void CodeEditor::updateLineNumber(const QRect &rect,int dy) {
  if(dy)
    _lineNumberWidget->scroll(0,dy);
  else
    _lineNumberWidget->update(0,rect.y(),_lineNumberWidget->width(),rect.height());

  if(rect.contains(viewport()->rect()))
    updateLineNumberWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  _lineNumberWidget->setGeometry(QRect(cr.left(),cr.top(),lineNumberWidth(),cr.height()));
}




LineNumberWidget::LineNumberWidget(CodeEditor *editor) 
  : QWidget(editor),
    _editor(editor) {
  
    }
  
QSize LineNumberWidget::sizeHint() const {
  return QSize(_editor->lineNumberWidth(),0);
}
  
void LineNumberWidget::paintEvent(QPaintEvent *event) {
  _editor->lineNumberPaintEvent(event);
}
