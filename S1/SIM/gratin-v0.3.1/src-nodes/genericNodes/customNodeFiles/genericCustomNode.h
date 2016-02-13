// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GENERIC_CUSTOM_NODE
#define GENERIC_CUSTOM_NODE

#include "genericCustomWidget.h"
#include "misc/extinclude.h"

class GenericCustomNode : public NodeTexture2D {
 public:
  GenericCustomNode(bool useVertex,bool useTesselation,bool useGeometry,bool useFragment,
		    PbGraph *parent=NULL,NodeHandle *handle=NULL,bool useDepthTest=false,bool useBlending=false, 
		    const Vector2f tsize=Vector2f(1.0f,1.0f),const Vector2f toffset=Vector2f(0.0f,0.0f));
  virtual ~GenericCustomNode();

  virtual void reload();
  virtual void init();
  virtual void clean();

  inline virtual void mousePressEvent(const Vector2f &,QMouseEvent *);
  inline virtual void mouseMoveEvent (const Vector2f &,QMouseEvent *);
  inline virtual void keyPressEvent  (QKeyEvent *                   );
  inline bool outputSizeEqualInputSize() const {return false;}

  inline QString  vertHead ()   const {return _vertHeadText;}
  inline QString  vertBody ()   const {return _vertBodyText;}
  inline QString  fragHead ()   const {return _fragHeadText;}
  inline QString  fragBody ()   const {return _fragBodyText;}
  inline QString  tessCHead()   const {return _tessCHeadText;}
  inline QString  tessCBody()   const {return _tessCBodyText;}
  inline QString  tessEHead()   const {return _tessEHeadText;}
  inline QString  tessEBody()   const {return _tessEBodyText;}
  inline QString  geomHead ()   const {return _geomHeadText;}
  inline QString  geomBody ()   const {return _geomBodyText;}

  virtual const QString constantVertHead()  {return QString();}
  virtual const QString defaultVertBody()   {return QString();}
  virtual const QString constantFragHead()  {return QString();}
  virtual const QString defaultFragBody()   {return QString();}
  virtual const QString constantTessCHead() {return QString();}
  virtual const QString defaultTessCBody()  {return QString();}
  virtual const QString constantTessEHead() {return QString();}
  virtual const QString defaultTessEBody()  {return QString();}
  virtual const QString constantGeomHead()  {return QString();}
  virtual const QString defaultGeomBody()   {return QString();}

  virtual void initOutputData();
  virtual bool save(SceneSaver  *saver);
  virtual bool load(SceneLoader *loader);

  // specific functions 
  inline Vector2f tsize      () const { return _tsize;         }
  inline Vector2f toffset    () const { return _toffset;       }
  inline QString  glslVersion() const { return _glslVersion;   }
  inline bool useVertex      () const { return _useVertex;     }
  inline bool useTesselation () const { return _useTesselation;}
  inline bool useGeometry    () const { return _useGeometry;   }
  inline bool useFragment    () const { return _useFragment;   }
  inline GLint outFilter     () const { return _outFilter;     }
  inline GLint outWrap       () const { return _outWrap;       }
  inline bool useDepthTest   () const { return _useDepthTest;  }
  inline bool useBlending    () const { return _useBlending;   }
  inline GLenum depthFunc    () const { return _depthFunc;     }
  inline GLenum blendSrc     () const { return _blendSrc;      }
  inline GLenum blendDst     () const { return _blendDst;      }
  inline Vector4f background () const { return _background;    }

  virtual void settingsChanged();
  virtual void sourceChanged(const QString &vertBody,const QString &fragBody,
			     const QString &tessCBody,const QString &tessEBody,const QString &geomBody);
  virtual void setIONames(const QStringList &inames,const QStringList &onames);
  virtual void setImgSize(const Vector2f &tsize,const Vector2f &toffset);

 protected:
  virtual const QStringList defaultUniformNames() const { return QStringList(); }
  virtual const QStringList defaultUniformTypes() const { return QStringList(); }

  inline virtual void drawGenericOutputs();
  inline virtual void setOutputParams();
  inline virtual void initOpenGLState();
  inline virtual void cleanOpenGLState();

  void addUniformsToProg();
  void initShaderSource();
  void initProg();
  void initHeads();

  inline void enableShaders();
  inline void disableShaders();

  QString      _glslVersion;
  bool         _useVertex;
  bool         _useTesselation;
  bool         _useGeometry;
  bool         _useFragment;
  GLint        _outFilter;
  GLint        _outWrap;
  bool         _useDepthTest;
  bool         _useBlending;
  GLenum       _depthFunc;
  GLenum       _blendSrc;
  GLenum       _blendDst;
  Vector4f     _background;

  QString      _vertHeadText;
  QString      _vertBodyText;
  QString      _fragHeadText;
  QString      _fragBodyText;
  QString      _tessCHeadText;
  QString      _tessCBodyText;
  QString      _tessEHeadText;
  QString      _tessEBodyText;
  QString      _geomHeadText;
  QString      _geomBodyText;

  QString      _vertText;
  QString      _fragText;
  QString      _tessCText;
  QString      _tessEText;
  QString      _geomText;

  Vector2f     _tsize;
  Vector2f     _toffset;

  GPUProgram  *_p;

  // convenient buffer for drawing a textured unit patch 
  static VertexarrayObject *_unitPatchVao; 
  static int                _nbPatches;  

  // convenient VAO that allows to draw squares and rectangles 
  static void initGPUPatch();
  static void clearGPUPatch();
};


inline void GenericCustomNode::mousePressEvent(const Vector2f &m,QMouseEvent *) {
  if(widget()) {
    ((GenericCustomWidget *)widget())->setMousePosition(m);
  }
  update();
}

inline void GenericCustomNode::mouseMoveEvent(const Vector2f &m,QMouseEvent *) {
  if(widget()) {
    ((GenericCustomWidget *)widget())->setMousePosition(m);
  }
  update();
}

inline void GenericCustomNode::keyPressEvent(QKeyEvent *ke) {
  if(widget()) {
    ((GenericCustomWidget *)widget())->setKeyboardValue(ke->key());
  }
  update();
}

inline void GenericCustomNode::setOutputParams() {
  for(unsigned int i=0;i<nbOutputs();++i) {
    outputTex(i)->setFilterMode(_outFilter);
    outputTex(i)->setWrapMode(_outWrap);
  }
}

inline void GenericCustomNode::initOpenGLState() {
  _glf->glClearColor(_background[0],_background[1],_background[2],_background[3]);

  if(_useDepthTest) {
    _glf->glEnable(GL_DEPTH_TEST);
    _glf->glDepthFunc(_depthFunc);
  }

  if(_useBlending) {
    _glf->glEnable(GL_BLEND);
    _glf->glBlendFunc(_blendSrc,_blendDst);
  }

  _glf->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void GenericCustomNode::cleanOpenGLState() {
  _glf->glDisable(GL_DEPTH_TEST);
  _glf->glDisable(GL_BLEND);
}

inline void GenericCustomNode::enableShaders() {
  _p->enable();

  QStringList l = inputNames();

  for(unsigned int i=0;i<nbInputs();++i) {
    _p->setUniformTexture(l[i],GL_TEXTURE_2D,inputTex(i)->id());
  }

  ((GenericCustomWidget *)widget())->sendUniformsToProg(_p);
}

inline void GenericCustomNode::disableShaders() {
  _p->disable();
}

inline void GenericCustomNode::drawGenericOutputs() {
  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  initOpenGLState();
  
  if(useTesselation()) {
    _glf->glPatchParameteri(GL_PATCH_VERTICES,4);
    _unitPatchVao->bind();
    _unitPatchVao->drawArrays(GL_PATCHES,0,4);
    _unitPatchVao->unbind();
    
  } else {
    _unitSquareVao->bind();
    _unitSquareVao->drawArrays(GL_TRIANGLES,0,6);
    _unitSquareVao->unbind();
  }

  cleanOpenGLState();
  FramebufferObject::unbind();
}

#endif // GENERIC_CUSTOM_NODE
