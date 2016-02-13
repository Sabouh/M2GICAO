// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef POISSON_SOLVER_BW_H
#define POISSON_SOLVER_BW_H

#include "misc/framebufferObject.h"
#include "misc/vertexarrayObject.h"
#include "misc/gpuProgram.h"
#include "misc/texture2D.h"
#include "misc/glutils.h"

class PoissonLevelBW : public GlContext {
 public:
  PoissonLevelBW(const std::string &bwPath,int level,int w,int h,FloatTexture2D *output=NULL);
  ~PoissonLevelBW();
  
  void initShaders(PoissonLevelBW *prev,PoissonLevelBW *next);
  void reloadShaders();

  inline int   level() const {return _level;}
  inline int   w()     const {return _w;    }
  inline int   h()     const {return _h;    }
  inline float m()     const {return _m;    }
  inline float e()     const {return _e;    }
  inline float c()     const {return _c;    }

  inline FramebufferObject *fbo() const {return _fbo;}

  inline FloatTexture2D *fTex() const {return _fTex;}
  inline FloatTexture2D *uTex() const {return _uTex;}
  inline FloatTexture2D *tTex() const {return _tTex;}
  
  inline GPUProgram *restrictShader()    const {return _myRestrict;   }
  inline GPUProgram *interpolateShader() const {return _interpolate;} 
  inline GPUProgram *refine1Shader()     const {return _refine1;    } 
  inline GPUProgram *refine2Shader()     const {return _refine2;    } 
  
 private:
  std::string _bwPath;
  PoissonLevelBW *_prev;
  PoissonLevelBW *_next;

  int   _level;
  int   _w;
  int   _h;
  bool  _no;
  float _m;
  float _e;
  float _c;
  float _x0;
  float _x1;
  float _t0;
  float _t1;

  FramebufferObject *_fbo;

  FloatTexture2D    *_fTex;
  FloatTexture2D    *_uTex;
  FloatTexture2D    *_tTex;
  
  GPUProgram        *_myRestrict;
  GPUProgram        *_interpolate;
  GPUProgram        *_refine1;
  GPUProgram        *_refine2;

  void sendUniforms();
};

class PoissonSolverBW : public GlContext {
 public:
  PoissonSolverBW(const std::string &bwPath);
  ~PoissonSolverBW();

  // input is a simple gradient texture with dirichlet boundaries in alpha channel 
  void initialize(FloatTexture2D *gxy,FloatTexture2D *output=NULL);

  // compute/update solution 
  void update(bool init=true);
  
  // clean all
  void finalize();

  // reload (usefull for test) 
  void reloadShaders();

  // output: reconstructed fonction u so that laplace(u) = divergence(_gxy) 
  inline FloatTexture2D *output() const {return _output;}

  // test functions 
  inline unsigned int nbLevels()  const {return _nb;}
  inline FloatTexture2D *getResidual(unsigned int l) const {return _levels[l]->fTex();}
  inline FloatTexture2D *getSolution(unsigned int l) const {return _levels[l]->uTex();}
  inline FloatTexture2D *getTmp(unsigned int l) const {return _levels[l]->tTex();}
  inline FloatTexture2D *getGxy() const {return _gxy;}

 private:
  // shader path
  std::string _bwPath;

  // input texture
  FloatTexture2D *_gxy;
  
  // output texture 
  FloatTexture2D *_output;

  GPUProgram *_residual;
  GPUProgram *_divergence;
  VertexarrayObject *_vao;

  // initial texture size 
  unsigned int _w;
  unsigned int _h;
  unsigned int _nb;

  std::vector<PoissonLevelBW *> _levels;

  // init and delete functions 
  void initShaders();
  void cleanShaders();
  void initLevels(FloatTexture2D *output=NULL);
  void cleanLevels();
  void sendUniforms();

  // common functions 
  inline void setTexWrap(FloatTexture2D *tex,GLint wrap);
  inline void drawQuad(GLenum buf,GPUProgram *p);
};

inline void PoissonSolverBW::drawQuad(GLenum buf,GPUProgram *p) {
  _glf->glDrawBuffer(buf);

  _vao->bind();

  p->enable();
  _glf->glDrawArrays(GL_TRIANGLES,0,6);
  p->disable();

  _vao->unbind();
}

inline void PoissonSolverBW::setTexWrap(FloatTexture2D *tex,GLint wrap) {
  tex->bind();
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrap);
  _glf->glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrap);
}

#endif // POISSON_SOLVER_BW_H
