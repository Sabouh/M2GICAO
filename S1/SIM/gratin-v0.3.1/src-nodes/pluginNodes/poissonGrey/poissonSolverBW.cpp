// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "poissonSolverBW.h"

using namespace std;

PoissonSolverBW::PoissonSolverBW(const string &bwPath) 
  : _bwPath(bwPath),
    _gxy(NULL),
    _output(NULL),
    _residual(NULL),
    _divergence(NULL),
    _vao(NULL),
    _w(0), 
    _h(0),
    _nb(0) {
  
}

PoissonSolverBW::~PoissonSolverBW() {
  finalize();
}


void PoissonSolverBW::initialize(FloatTexture2D *gxy,FloatTexture2D *output) {

  // clean all first
  finalize();

  // get texture and sizes 
  _gxy = gxy;
  
  _w = gxy->w();
  _h = gxy->h();
  
  // init gpu data 
  initLevels(output);
  initShaders();

  // init VAO
  _vao = new VertexarrayObject();
  const Vector2f boxesPts[6] = {Vector2f(-1,-1),Vector2f( 1,-1),Vector2f( 1, 1),
				Vector2f( 1, 1),Vector2f(-1, 1),Vector2f(-1,-1)};
  _vao->addAttrib(6*sizeof(Vector2f),boxesPts[0].data(),2,0,GL_STATIC_DRAW);

  _glf->glClearColor(0.0f,0.0f,0.0f,0.0f);

  // init solution 
  Glutils::setViewport(_w,_h);
  _levels[0]->fbo()->bind();
  _glf->glDrawBuffer(GL_COLOR_ATTACHMENT1);
  _glf->glClear(GL_COLOR_BUFFER_BIT);
  FramebufferObject::unbind();
}

void PoissonSolverBW::update(bool init) {
  _glf->glClearColor(0.0f,0.0f,0.0f,0.0f);

  Glutils::setViewport(_w,_h);
  setTexWrap(_gxy,GL_CLAMP_TO_BORDER);

  _levels[0]->fbo()->bind();

  if(init) {
    // init solution to zero 
    _glf->glDrawBuffer(GL_COLOR_ATTACHMENT1);
    _glf->glClear(GL_COLOR_BUFFER_BIT);
  }

  // residual   
  drawQuad(GL_COLOR_ATTACHMENT0,_residual);

  // top-down process 
  for(unsigned int i=1;i<_nb;++i) {
    Glutils::setViewport(_levels[i]->w(),_levels[i]->h());
    _levels[i]->fbo()->bind();

    // restriction from level i to i+1     
    drawQuad(GL_COLOR_ATTACHMENT0,_levels[i]->restrictShader());
  }

  // init last level solution to black
  _levels[_nb-1]->fbo()->bind();
  _glf->glDrawBuffer(GL_COLOR_ATTACHMENT1);
  _glf->glClear(GL_COLOR_BUFFER_BIT);
 
  // first relaxation (tmp solution at last level)
  drawQuad(GL_COLOR_ATTACHMENT2,_levels[_nb-1]->refine1Shader());  

  // second relaxation (final solution at last level)
  drawQuad(GL_COLOR_ATTACHMENT1,_levels[_nb-1]->refine2Shader());  

  if(!init) {
    // We need to re-initialize divergence for the 1st level
    _levels[0]->fbo()->bind();
    Glutils::setViewport(_levels[0]->w(),_levels[0]->h());
    drawQuad(GL_COLOR_ATTACHMENT0,_divergence);  
  }

  // bottom-up process
  for(int i=_nb-2;i>=0;--i) {
    _levels[i]->fbo()->bind();
    Glutils::setViewport(_levels[i]->w(),_levels[i]->h());
    
    // interpolation from level i+1 to i
    drawQuad(GL_COLOR_ATTACHMENT1,_levels[i]->interpolateShader());  

    // first relaxation (tmp solution at level i)
    drawQuad(GL_COLOR_ATTACHMENT2,_levels[i]->refine1Shader());  
    
    // second relaxation (final solution at level i)
    drawQuad(GL_COLOR_ATTACHMENT1,_levels[i]->refine2Shader());  
  }

  FramebufferObject::unbind();
}

void PoissonSolverBW::finalize() {
  // clean all 
  cleanLevels();
  cleanShaders();
  delete _vao;_vao = NULL;
}

void PoissonSolverBW::initShaders() {
  // shader creation 
  _residual   = new GPUProgram(":/psbw.vert",":/psbw_residual.frag");
  _divergence = new GPUProgram(":/psbw.vert",":/psbw_divergence.frag");

  _residual->enable();
  _residual->addUniform("gxy");
  _residual->addUniform("u");
  _residual->addUniform("m");
  _residual->addUniform("c");
  _residual->addUniform("e");
  _residual->disable();

  _divergence->enable();
  _divergence->addUniform("gxy");
  _divergence->disable();

  for(unsigned int i=0;i<_nb;++i) {
   
    PoissonLevelBW *prev = i==0     ? NULL : _levels[i-1];
    PoissonLevelBW *next = i==_nb-1 ? NULL : _levels[i+1];
    
    _levels[i]->initShaders(prev,next);
  }

  sendUniforms();
}

void PoissonSolverBW::cleanShaders() {
  // delete shaders 
  delete _residual;   _residual   = NULL;
  delete _divergence; _divergence = NULL;
}

void PoissonSolverBW::sendUniforms() {
  if(_levels.empty()) 
    return;

  _residual->enable();
  _residual->setUniformTexture("gxy",GL_TEXTURE_2D,_gxy->id());
  _residual->setUniformTexture("u",GL_TEXTURE_2D,_levels[0]->uTex()->id());
  _residual->setUniform1f("m",_levels[0]->m());
  _residual->setUniform1f("c",_levels[0]->c());
  _residual->setUniform1f("e",_levels[0]->e());
  _residual->disable();

  _divergence->enable();
  _divergence->setUniformTexture("gxy",GL_TEXTURE_2D,_gxy->id());
  _divergence->disable();
}

void PoissonSolverBW::reloadShaders() {
  _residual->reload();
  _divergence->reload();

  for(unsigned int i=0;i<_nb;++i) {
    _levels[i]->reloadShaders();
  }

  sendUniforms();
}

void PoissonSolverBW::initLevels(FloatTexture2D *output) {
  int w = _w;
  int h = _h;
  int i =  0;

  if(_w<=0 || _h<=0) 
    return;

  do {    
    if(i==0) 
      _levels.push_back(new PoissonLevelBW(_bwPath,i,w,h,output));
    else 
      _levels.push_back(new PoissonLevelBW(_bwPath,i,w,h));
 
    w = std::max(1,w/2)+w%2;
    h = std::max(1,h/2)+h%2;
    i ++;
    
  } while(w!=1 && h!=1);

  _levels.push_back(new PoissonLevelBW(_bwPath,i,1,1));
  _nb     = static_cast<unsigned int>(_levels.size());
  _output = _levels[0]->uTex();
}

void PoissonSolverBW::cleanLevels() {
  for(unsigned int i=0;i<_nb;++i) {
    delete _levels[i];
  }
  _levels.clear();
}

PoissonLevelBW::PoissonLevelBW(const string &bwPath,int level,int w,int h,FloatTexture2D *output) 
  : _bwPath(bwPath),
    _prev(NULL),
    _next(NULL),
    _level(level),
    _w(w),
    _h(h),
    _no(false) {
  
  // init other params 
  float s  = (float)(1 << level);
  float s2 = s*s;
  float m  = -8.0f*s2-4.0f;
  float e  = s2 + 2.0f;
  float c  = s2 - 1.0f;
  
  m /= 3*s2;
  e /= 3*s2;
  c /= 3*s2;
  
  _m = m;
  _e = e;
  _c = c;
  
  _x0 = -2.1532f + 0.5882f / s2 + 1.5070f / s;
  _x1 = 0.1138f + 1.5065f / s2 + 0.9529f / s;
  _t0 = 1.0f/(_m-_x0);
  _t1 = 1.0f/(_m-_x1);

  // init fbo and textures 
  GLint  filter = GL_LINEAR;
  GLint  wrap1  = GL_CLAMP_TO_BORDER;
  GLint  wrap2  = GL_CLAMP_TO_EDGE;
  GLenum format = Glutils::defFormat();

  TextureFormat tf1 = TextureFormat(GL_TEXTURE_2D,_w,_h,format,GL_RGBA,GL_FLOAT);
  TextureParams tp1 = TextureParams(filter,filter,wrap1,wrap1,wrap1);
  TextureParams tp2 = TextureParams(filter,filter,wrap2,wrap2,wrap2);
  
  _fbo  = new FramebufferObject();
  _fTex = new FloatTexture2D(tf1,tp1);
  _tTex = new FloatTexture2D(tf1,tp2);
  _uTex = output ? output : new FloatTexture2D(tf1,tp2);
  
  if(_uTex==output) {
    // init parameters 
    _uTex->setData(tf1,tp2,NULL);
    _no = true;
  }

  // attach textures 
  _fbo->bind();
  _fbo->attachTexture(GL_TEXTURE_2D,_fTex->id(),GL_COLOR_ATTACHMENT0);
  _fbo->attachTexture(GL_TEXTURE_2D,_uTex->id(),GL_COLOR_ATTACHMENT1);
  _fbo->attachTexture(GL_TEXTURE_2D,_tTex->id(),GL_COLOR_ATTACHMENT2);
  _fbo->isValid();
  FramebufferObject::unbind();

  // creates shader programs 
  _myRestrict = new GPUProgram(":/psbw.vert",":/psbw_restrict.frag");
  _refine1  = new GPUProgram(":/psbw.vert",":/psbw_refine.frag");
  _refine2  = new GPUProgram(":/psbw.vert",":/psbw_refine.frag");
 
  _myRestrict->enable();
  _myRestrict->addUniform("f");
  _myRestrict->disable();

  if(level>0) {
    _interpolate = new GPUProgram(":/psbw.vert",":/psbw_interpolate0.frag");
    _interpolate->enable();
    _interpolate->addUniform("u");
    _interpolate->disable();

  } else {
    _interpolate = new GPUProgram(":/psbw.vert",":/psbw_interpolate1.frag");
    _interpolate->enable();
    _interpolate->addUniform("u");
    _interpolate->addUniform("u0");
    _interpolate->disable();
  }
 
  _refine1->enable();
  _refine1->addUniform("f");
  _refine1->addUniform("u");
  _refine1->addUniform("c");
  _refine1->addUniform("e");
  _refine1->addUniform("x");
  _refine1->addUniform("t");
  _refine1->disable();

  _refine2->enable();
  _refine2->addUniform("f");
  _refine2->addUniform("u");
  _refine2->addUniform("c");
  _refine2->addUniform("e");
  _refine2->addUniform("x");
  _refine2->addUniform("t");
  _refine2->disable();
}

void PoissonLevelBW::initShaders(PoissonLevelBW *prev,PoissonLevelBW *next) {
  _prev = prev;
  _next = next;
  
  sendUniforms();
}

void PoissonLevelBW::reloadShaders() {
  _myRestrict->reload();
  _interpolate->reload();
  _refine1->reload();
  _refine2->reload();
  
  sendUniforms();
}

void PoissonLevelBW::sendUniforms() {
  
  // restriction only if this is not the first level
  if(_prev!=NULL) {
    _myRestrict->enable();
    _myRestrict->setUniformTexture("f",GL_TEXTURE_2D,_prev->fTex()->id());
    _myRestrict->disable();
  }
  
  // interpolation only if this is not the last level
  if(_next!=NULL) {
    _interpolate->enable();
    _interpolate->setUniformTexture("u",GL_TEXTURE_2D,_next->uTex()->id());

    if(_prev==NULL) 
      _interpolate->setUniformTexture("u0",GL_TEXTURE_2D,_uTex->id());
    
    _interpolate->disable();
  }
  
  // relaxation 1 & 2
  _refine1->enable();
  _refine1->setUniform1f("c",_c);
  _refine1->setUniform1f("e",_e);
  _refine1->setUniform1f("x",_x0);
  _refine1->setUniform1f("t",_t0);
  _refine1->setUniformTexture("f",GL_TEXTURE_2D,_fTex->id());
  _refine1->setUniformTexture("u",GL_TEXTURE_2D,_uTex->id());
  _refine1->disable();

  _refine2->enable();
  _refine2->setUniform1f("c",_c);
  _refine2->setUniform1f("e",_e);
  _refine2->setUniform1f("x",_x1);
  _refine2->setUniform1f("t",_t1);
  _refine2->setUniformTexture("f",GL_TEXTURE_2D,_fTex->id());
  _refine2->setUniformTexture("u",GL_TEXTURE_2D,_tTex->id());
  _refine2->disable();
}

PoissonLevelBW::~PoissonLevelBW() {
  delete _fTex;
  delete _tTex;
  delete _fbo;
  delete _myRestrict;
  delete _interpolate;
  delete _refine1;
  delete _refine2;
  // if I did not create this texture 
  if(!_no) delete _uTex;
}
