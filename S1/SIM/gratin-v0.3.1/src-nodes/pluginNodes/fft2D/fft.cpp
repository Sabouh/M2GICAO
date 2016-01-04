// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>

#include "fft.h"

FFT2DWidget::FFT2DWidget(NodeInterface *node) 
  : NodeWidget(node),
    _update(true) {
 
  _invertBox = new QComboBox();
  _invertBox->addItem("forward (fft)");
  _invertBox->addItem("backward (invfft)");

  _outputBox   = new QComboBox();
  _outputBox->addItem("Magnitude/Phase");
  _outputBox->addItem("Real/Imaginary");
  
  _shift = new QCheckBox("FFT Shift");
  _shift->setCheckState(Qt::Checked);
  QHBoxLayout *h = new QHBoxLayout();
  h->addWidget(_invertBox);
  h->addWidget(_shift);
  
  QVBoxLayout *l = new QVBoxLayout();
  l->addLayout(h);
  l->addWidget(_outputBox);
  l->addStretch(1);
  setLayout(l);
  
  connect(_invertBox,SIGNAL(currentIndexChanged(int)),this,SLOT(needSetShaders()));
  connect(_outputBox,SIGNAL(currentIndexChanged(int)),this,SLOT(needUpdate()));
  connect(_shift,SIGNAL(stateChanged(int)),this,SLOT(needUpdate()));
}

void FFT2DWidget::needSetShaders() {
  ((FFT2DNode *)node())->setShaderFiles(invert() ? FFT2DNode::FFT_BACKWARD : FFT2DNode::FFT_FORWARD);
  ((FFT2DNode *)node())->setShaderUniforms(invert() ? FFT2DNode::FFT_BACKWARD : FFT2DNode::FFT_FORWARD);
  if(node()->isReady() && _update) node()->update();
}

FFT2DNode::FFT2DNode(PbGraph *parent,NodeHandle *handle)
  : NodeTexture2D(parent,handle),
    _copInProg(new GPUProgram(":/copyIn.vert",":/copyIn.frag")),
    _copOutProg(new GPUProgram(":/copyOut.vert",":/copyOut.frag")),
    _fftProg(new GPUProgram(":/fft.vert",":/fft.frag")),
    _w(new FFT2DWidget(this)) {

  setShaderUniforms(FFT_FORWARD);
}

FFT2DNode::~FFT2DNode() {
  cleanFFTData();
  delete _copInProg;
  delete _copOutProg;
  delete _fftProg;
}

void FFT2DNode::setShaderFiles(int type) {
  delete _copInProg;  _copInProg  = NULL;
  delete _copOutProg; _copOutProg = NULL;
  delete _fftProg;    _fftProg    = NULL;

  if(type==FFT_FORWARD) {
    _copInProg  = new GPUProgram(":/copyIn.vert",":/copyIn.frag");
    _copOutProg = new GPUProgram(":/copyOut.vert",":/copyOut.frag");
    _fftProg    = new GPUProgram(":/fft.vert",":/fft.frag");
  } else {
    _copInProg  = new GPUProgram(":/copyInInv.vert",":/copyInInv.frag");
    _copOutProg = new GPUProgram(":/copyOutInv.vert",":/copyOutInv.frag");
    _fftProg    = new GPUProgram(":/fft.vert",":/fftInv.frag");
  }
}

void FFT2DNode::setShaderUniforms(int type) {
  if(type==FFT_FORWARD) {
    _copInProg->addUniform("img");
    
    _copOutProg->addUniform("img");
    _copOutProg->addUniform("shift");
    _copOutProg->addUniform("realimag");
    
    _fftProg->addUniform("fft");
    _fftProg->addUniform("ris");
    _fftProg->addUniform("dimension");
  } else {
    _copInProg->addUniform("img");
    _copInProg->addUniform("shift");
    _copInProg->addUniform("realimag");
    
    _copOutProg->addUniform("img");
    
    _fftProg->addUniform("fft");
    _fftProg->addUniform("ris");
    _fftProg->addUniform("dimension");
  }
}


void FFT2DNode::init() {
  NodeTexture2D::init();
  initFFTData();
}

void FFT2DNode::clean() {
  NodeTexture2D::clean();
  cleanFFTData();
}

void FFT2DNode::receiveEvent(const NodeEvent &event) {
  NodeTexture2D::receiveEvent(event);
  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    initFFTData();
  }
}

void FFT2DNode::apply() {
  // set viewport 
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());

  _currPass = _initPass;

  // copy input image
  _copInProg->enable();
  if(_w->invert()) {
    _copInProg->setUniform1i("shift",_w->shift());
    _copInProg->setUniform1i("realimag",_w->realImag());
  }
  _copInProg->setUniformTexture("img",GL_TEXTURE_2D,inputTex(0)->id());
  drawOutputs(_outbuff[_currPass],1,true,false);
  _copInProg->disable();

  // compute FFT
  _fftProg->enable();
  for(int d=0;d<2;++d) {
    _fftProg->setUniform1i("dimension",d);
    for(unsigned int i=0;i<_stages[d];++i) {
      _currPass = 1-_currPass;
      _fftProg->setUniformTexture("fft",GL_TEXTURE_2D,_inSampl[_currPass]->id());
      _fftProg->setUniformTexture("ris",GL_TEXTURE_2D,_risGPU[d][i]->id());

      drawOutputs(_outbuff[_currPass],1,false,false);
    }
  }
  _fftProg->disable();

  // copy output image
  _copOutProg->enable();
  if(!_w->invert()) {
    _copOutProg->setUniform1i("shift",_w->shift());
    _copOutProg->setUniform1i("realimag",_w->realImag());
  }
  _copOutProg->setUniformTexture("img",GL_TEXTURE_2D,_inSampl[0]->id());
  drawOutputs(_outbuff[0],1,false,true);
  _copOutProg->disable();

  // disable everything
  FramebufferObject::unbind();
}

void FFT2DNode::initFFTData() {
  cleanFFTData();

  _size[0] = outputTex(0)->w();
  _size[1] = outputTex(0)->h();

  for(int i=0;i<2;++i) {
    unsigned int s = _size[i];
    _stages[i] = 0;

    while((s = s>>1)!=0)
      ++_stages[i];

    _risCPU[i] = vector<Vector4f>(_size[i]*_stages[i]);
    _risGPU[i] = vector<FloatTexture2D *>(_stages[i]);

    createButterflyTables(i);
    initTextures(i);
  }

  _outbuff[0]  = buffersOfOutputTex(0);
  _outbuff[1]  = buffersOfTmpTex(0);
  _inSampl[0]  = tmpTex(0);
  _inSampl[1]  = outputTex(0);
  _initPass    = (_stages[0]+_stages[1]+1)%2;
}

void FFT2DNode::cleanFFTData() {
  for(int i=0;i<2;++i) {
    for(unsigned int j=0;j<_risGPU[i].size();++j) {
      delete _risGPU[i][j]; _risGPU[i][j] = NULL;
    }
    _risCPU[i].clear();
    _risGPU[i].clear();
  }
}

void FFT2DNode::createButterflyTables(int d) {
  int n = 0;

  for(unsigned int i=0;i<_stages[d];++i) {
    int blocks = 1<<(_stages[d]-1-i);
    int blockInputs = 1<<i;

    for(int j=0;j<blocks;++j) {
      for(int k=0;k<blockInputs;++k) {
	  int i1 = j*blockInputs*2 + k;
	  int i2 = i1 + blockInputs;
	  float j1, j2;

	  if(i==0) {
	    j1 = static_cast<float>(bitReverse(i1,_size[d]));
	    j2 = static_cast<float>(bitReverse(i2,_size[d]));
	  } else {
	    j1 = static_cast<float>(i1);
	    j2 = static_cast<float>(i2);
	  }

	  i1 += n;
	  i2 += n;

	  double angle = 2.0*M_PI*k*blocks/static_cast<float>(_size[d]);
	  float wr = static_cast<float>( cos(angle));
	  float wi = static_cast<float>(-sin(angle));

	  _risCPU[d][i1] = Vector4f( wr, wi,j1,j2);
	  _risCPU[d][i2] = Vector4f(-wr,-wi,j1,j2);
	}
    }
    n += _size[d];
  }
}

void FFT2DNode::initTextures(int d) {
  GLsizei w = d == 1 ? 1 : _size[0];
  GLsizei h = d == 0 ? 1 : _size[1];
  TextureFormat tf(GL_TEXTURE_2D,w,h,GL_RGBA32F,GL_RGBA,GL_FLOAT);
  TextureParams tp(GL_NEAREST,GL_NEAREST);

  for(unsigned int i=0;i<_stages[d];++i) {
    _risGPU[d][i] = new FloatTexture2D(tf,tp,_risCPU[d][i*_size[d]].data());
  }
  
  if(!isReady()) return;
  tmpTex(0)->setFilterMode(GL_NEAREST);
  outputTex(0)->setFilterMode(GL_NEAREST);
}

bool FFT2DNode::load(SceneLoader *loader) {
  bool ret = NodeTexture2D::load(loader);
  bool invert = false;
  bool realImag = false;
  bool shift = true;

  if(!loader->loadBool("fftinv",invert))        ret = false;
  if(!loader->loadBool("fftrealimag",realImag)) ret = false;
  if(!loader->loadBool("fftshift",shift))       ret = false;

  _w->setUpdate(false);
  _w->setRealImag(realImag);
  _w->setShift(shift);
  _w->setUpdate(true);
  _w->setInvert(invert);
  setShaderFiles(invert ? FFT_BACKWARD : FFT_FORWARD);
  setShaderUniforms(invert ? FFT_BACKWARD : FFT_FORWARD);

  return ret;
}

bool FFT2DNode::save(SceneSaver  *saver) {
  bool ret = NodeTexture2D::save(saver);
  if(!saver->saveBool("fftinv",_w->invert()))        ret = false;
  if(!saver->saveBool("fftrealimag",_w->realImag())) ret = false;
  if(!saver->saveBool("fftshift",_w->shift()))       ret = false;
  return ret;
}
