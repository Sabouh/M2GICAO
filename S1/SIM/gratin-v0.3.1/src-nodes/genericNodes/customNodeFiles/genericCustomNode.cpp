// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "genericCustomNode.h"
#include <QDebug>

VertexarrayObject *GenericCustomNode::_unitPatchVao  = NULL;
int                GenericCustomNode::_nbPatches     = 0;

void GenericCustomNode::initGPUPatch() {
  _unitPatchVao = new VertexarrayObject();

  vector<Vector2f> boxesPts(4);
  vector<Vector2f> boxesCos(4);

  boxesPts[0]  = Vector2f(-1,-1);
  boxesPts[1]  = Vector2f(1,-1);
  boxesPts[2]  = Vector2f(1,1);
  boxesPts[3]  = Vector2f(-1,1);
  
  boxesCos[0]  = Vector2f(0.0f,0.0f);
  boxesCos[1]  = Vector2f(1.0f,0.0f);
  boxesCos[2]  = Vector2f(1.0f,1.0f);
  boxesCos[3]  = Vector2f(0.0f,1.0f);

  _unitPatchVao->addAttrib(4*sizeof(Vector2f),boxesPts[0].data(),2,0,GL_STATIC_DRAW);
  _unitPatchVao->addAttrib(4*sizeof(Vector2f),boxesCos[0].data(),2,0,GL_STATIC_DRAW);
}

void GenericCustomNode::clearGPUPatch() {
  delete _unitPatchVao; 
  _unitPatchVao    = NULL;
}

GenericCustomNode::GenericCustomNode(bool useVertex,bool useTesselation,bool useGeometry,bool useFragment,
				     PbGraph *parent,NodeHandle *handle,bool useDepthTest,bool useBlending,
				     const Vector2f tsize,const Vector2f toffset)
  : NodeTexture2D(parent,handle),
    _glslVersion(QString::number(OPENGL_MAJOR_VERSION)+QString::number(OPENGL_MINOR_VERSION)+"0"),
    _useVertex(useVertex),
    _useTesselation(useTesselation),
    _useGeometry(useGeometry),
    _useFragment(useFragment),
    _outFilter(GL_LINEAR),
    _outWrap(GL_CLAMP_TO_EDGE),
    _useDepthTest(useDepthTest),
    _useBlending(useBlending),
    _depthFunc(GL_LESS),
    _blendSrc(GL_ONE),
    _blendDst(GL_ZERO),
    _background(0.0f,0.0f,0.0f,0.0f),
    _tsize(tsize),
    _toffset(toffset),
    _p(NULL) {

  if(_nbPatches<=0) {
    initGPUPatch();
    _nbPatches = 0;
  }
  _nbPatches++;
}

GenericCustomNode::~GenericCustomNode() {
  _nbPatches--;
  if(_nbPatches<=0) {
    clearGPUPatch();
  }
}

void GenericCustomNode::reload() {
  if(_p) _p->reload();
}

void GenericCustomNode::init() {
  if(!_p) {
    initShaderSource();
    initProg();
  }
}

void GenericCustomNode::clean() {

}


void GenericCustomNode::initShaderSource() {
  initHeads();

  _vertBodyText  = defaultVertBody();
  _fragBodyText  = defaultFragBody();
  _tessCBodyText = defaultTessCBody();
  _tessEBodyText = defaultTessEBody();
  _geomBodyText  = defaultGeomBody();

  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;
}

void GenericCustomNode::initHeads() {
  assert(widget());

  QString header = "#version "+_glslVersion+"\n\n";
  QStringList uniformNames = defaultUniformNames();
  QStringList uniformTypes = defaultUniformTypes();
 
  QString uniforms = ((GenericCustomWidget *)widget())->generateUniformsHead();
  QString outLocs  = ((GenericCustomWidget *)widget())->generateOutLocationsHead();

  int nbUniforms = std::min(uniformTypes.size(),uniformNames.size());
  for(int i=0;i<nbUniforms;++i) {
    uniforms = uniforms + "uniform "+uniformTypes[i]+" "+uniformNames[i]+";\n"; 
  }
  
  _vertHeadText  = header+constantVertHead()+uniforms;
  _tessCHeadText = header+constantTessCHead()+uniforms;
  _tessEHeadText = header+constantTessEHead()+uniforms;
  _geomHeadText  = header+constantGeomHead()+uniforms;
  _fragHeadText  = header+outLocs+constantFragHead()+uniforms;
}

void GenericCustomNode::addUniformsToProg() {
  const QStringList userUniforms    = ((GenericCustomWidget *)widget())->getUniformNames();
  const QStringList defaultUniforms = defaultUniformNames();

  for(int i=0;i<userUniforms.size();++i) {
    _p->addUniform(userUniforms[i]);
  }

  for(int i=0;i<defaultUniforms.size();++i) {
    _p->addUniform(defaultUniforms[i]);
  }
}

void GenericCustomNode::initProg() {
  assert(widget());
  delete _p; _p = NULL;

  QString vertText  = _vertText;
  QString fragText  = _fragText;
  QString tessCText = useTesselation() ? _tessCText : "";
  QString tessEText = useTesselation() ? _tessEText : "";
  QString geomText  = useGeometry() ? _geomText : "";

  _p = new GPUProgram(vertText,fragText,geomText,tessCText,tessEText,false,false);
  ((GenericCustomWidget *)widget())->setLogFromProg(_p);
  addUniformsToProg();
}

void GenericCustomNode::settingsChanged() {
  assert(widget());
  
  GenericCustomWidget *w = (GenericCustomWidget *)widget();
  _glslVersion    = w->glslVersion();
  _useVertex      = w->useVertex();
  _useTesselation = w->useTesselation();
  _useGeometry    = w->useGeometry();
  _useFragment    = w->useFragment();
  _outFilter      = w->filterMode();
  _outWrap        = w->wrapMode();
  _useDepthTest   = w->useDepthTest();
  _useBlending    = w->useBlending();
  _depthFunc      = w->depthFunc();
  _blendSrc       = w->blendSrc();
  _blendDst       = w->blendDst();
  _background     = w->background();

  setName(w->newName());

  initHeads();

  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;

  initProg();
}

void GenericCustomNode::sourceChanged(const QString &vertBody,const QString &fragBody,
				      const QString &tessCBody,const QString &tessEBody,const QString &geomBody) {
  _vertBodyText  = vertBody;
  _fragBodyText  = fragBody;
  _tessCBodyText = tessCBody;
  _tessEBodyText = tessEBody;
  _geomBodyText  = geomBody;

  _vertText  = _vertHeadText+_vertBodyText;
  _fragText  = _fragHeadText+_fragBodyText;
  _tessCText = _tessCHeadText+_tessCBodyText;
  _tessEText = _tessEHeadText+_tessEBodyText;
  _geomText  = _geomHeadText+_geomBodyText;

  initProg();
}

bool GenericCustomNode::save(SceneSaver  *saver) {
  assert(widget());

  bool ret = true;

  // parameters 
  if(!saver->saveString  ("genericGlslVersion"    ,_glslVersion))    ret = false;
  if(!saver->saveString  ("genericNodeName"       ,name()))          ret = false;
  if(!saver->saveBool    ("genericUseVertex"      ,_useVertex))      ret = false;
  if(!saver->saveBool    ("genericUseTesselation" ,_useTesselation)) ret = false;
  if(!saver->saveBool    ("genericUseGeometry"    ,_useGeometry))    ret = false;
  if(!saver->saveBool    ("genericUseFragment"    ,_useFragment))    ret = false;
  if(!saver->saveInt     ("genericOutFilter"      ,_outFilter))      ret = false;
  if(!saver->saveInt     ("genericOutWrap"        ,_outWrap))        ret = false;
  if(!saver->saveBool    ("genericUseDepthTest"   ,_useDepthTest))   ret = false;
  if(!saver->saveBool    ("genericUseBlending"    ,_useBlending))    ret = false;
  if(!saver->saveUint    ("genericDepthFunc"      ,_depthFunc))      ret = false;
  if(!saver->saveUint    ("genericBlendSrc"       ,_blendSrc))       ret = false;
  if(!saver->saveUint    ("genericBlendDst"       ,_blendDst))       ret = false;
  if(!saver->saveVector4f("genericBackground"     ,_background))     ret = false;

  // sources 
  if(!saver->saveString("genericVertHead" ,_vertHeadText))  ret = false;
  if(!saver->saveString("genericVertBody" ,_vertBodyText))  ret = false;
  if(!saver->saveString("genericFragHead" ,_fragHeadText))  ret = false;
  if(!saver->saveString("genericFragBody" ,_fragBodyText))  ret = false;
  if(!saver->saveString("genericTessCHead",_tessCHeadText)) ret = false;
  if(!saver->saveString("genericTessCBody",_tessCBodyText)) ret = false;
  if(!saver->saveString("genericTessEHead",_tessEHeadText)) ret = false;
  if(!saver->saveString("genericTessEBody",_tessEBodyText)) ret = false;
  if(!saver->saveString("genericGeomHead" ,_geomHeadText))  ret = false;
  if(!saver->saveString("genericGeomBody" ,_geomBodyText))  ret = false;

  // save output size 
  if(!saver->saveVector2f("genericTSize",_tsize))     ret = false;
  if(!saver->saveVector2f("genericTOffset",_toffset)) ret = false;

  // widgets 
  if(!((GenericCustomWidget *)widget())->saveWidgets(saver)) ret = false;

  return ret;
}

bool GenericCustomNode::load(SceneLoader *loader) {
  assert(widget());

  // the number of inputs/outputs should have already been set in thanks to the constructor
  bool ret = true;

  QString n = name();

  // parameters 
  if(!loader->loadString  ("genericGlslVersion"    ,_glslVersion))    ret = false;
  if(!loader->loadString  ("genericNodeName"       ,n           ))    ret = false;
  if(!loader->loadBool    ("genericUseVertex"      ,_useVertex))      ret = false;
  if(!loader->loadBool    ("genericUseTesselation" ,_useTesselation)) ret = false;
  if(!loader->loadBool    ("genericUseGeometry"    ,_useGeometry))    ret = false;
  if(!loader->loadBool    ("genericUseFragment"    ,_useFragment))    ret = false;
  if(!loader->loadInt     ("genericOutFilter"      ,_outFilter))      ret = false;
  if(!loader->loadInt     ("genericOutWrap"        ,_outWrap))        ret = false;
  if(!loader->loadBool    ("genericUseDepthTest"   ,_useDepthTest))   ret = false;
  if(!loader->loadBool    ("genericUseBlending"    ,_useBlending))    ret = false;
  if(!loader->loadUint    ("genericDepthFunc"      ,_depthFunc))      ret = false;
  if(!loader->loadUint    ("genericBlendSrc"       ,_blendSrc))       ret = false;
  if(!loader->loadUint    ("genericBlendDst"       ,_blendDst))       ret = false;
  if(!loader->loadVector4f("genericBackground"     ,_background))     ret = false;

  // sources 
  if(!loader->loadString("genericVertHead" ,_vertHeadText))  ret = false;
  if(!loader->loadString("genericVertBody" ,_vertBodyText))  ret = false;
  if(!loader->loadString("genericFragHead" ,_fragHeadText))  ret = false;
  if(!loader->loadString("genericFragBody" ,_fragBodyText))  ret = false;
  if(!loader->loadString("genericTessCHead",_tessCHeadText)) ret = false;
  if(!loader->loadString("genericTessCBody",_tessCBodyText)) ret = false;
  if(!loader->loadString("genericTessEHead",_tessEHeadText)) ret = false;
  if(!loader->loadString("genericTessEBody",_tessEBodyText)) ret = false;
  if(!loader->loadString("genericGeomHead" ,_geomHeadText))  ret = false;
  if(!loader->loadString("genericGeomBody" ,_geomBodyText))  ret = false;

  // load output size 
  if(!loader->loadVector2f("genericTSize",_tsize))     ret = false;
  if(!loader->loadVector2f("genericTOffset",_toffset)) ret = false;
  setImgSize(_tsize,_toffset);

  // create widgets 
  GenericCustomWidget *w = (GenericCustomWidget *)widget();

  if(!w->loadWidgets(loader)) ret = false;

  w->setVertSource(_vertHeadText,_vertBodyText);
  w->setTessCSource(_tessCHeadText,_tessCBodyText);
  w->setTessESource(_tessEHeadText,_tessEBodyText);
  w->setGeomSource(_geomHeadText,_geomBodyText);
  w->setFragSource(_fragHeadText,_fragBodyText);

  w->updateWidget();
  w->applyClicked(false);
  settingsChanged();

  setName(n);

  return ret;
}



void GenericCustomNode::setImgSize(const Vector2f &tsize,const Vector2f &toffset) {
  _tsize = tsize;
  _toffset = toffset;

  if(isReady() && !_inputData.empty() && input(0)->type()==Data::TEX2D) {
    Vector2f texSize(((TextureData *)input(0))->tex()->w()*_tsize[0],
		     ((TextureData *)input(0))->tex()->h()*_tsize[1]);
    setOutputSize(texSize+_toffset);
  } else if(_inputData.empty() && _toffset!=Vector2f(0,0)) {
    setOutputSize(_toffset);
  }
}

void GenericCustomNode::initOutputData()   {
  if(!checkInputData(inputs())) {
    qDebug() << "Warning: trying to init output data while input not ready!" << endl;
    return;
  }

  // clean everything 
  cleanOutputData();

  //Vector2f size = outputContentSize(0);
  Vector2f size = outputSize();

  if(!_inputData.empty() && input(0)->type()==Data::TEX2D && !outputSizeEqualInputSize()) {
    Vector2f texSize(((TextureData *)input(0))->tex()->w()*_tsize[0],
		     ((TextureData *)input(0))->tex()->h()*_tsize[1]);
    size = texSize+_toffset;
  }

  //_offset = size/10.0f;

  for(unsigned int i=0;i<_outputData.size();++i) {
    TextureData *data = (TextureData *)output(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  for(unsigned int i=0;i<_tmpData.size();++i) {
    TextureData *data = (TextureData *)tmp(i);
    //data->setSize(size+offset());
    data->createTex(size);
  }

  // create tmp textures and FBO
  initFBO();
  //initVertices();
  _ready = true;

  updatePositions();
}

void GenericCustomNode::setIONames(const QStringList &inames,const QStringList &onames) {
  unsigned int currentNbIn = nbInputs();
  unsigned int currentNbOut = nbOutputs();

  setInputNames(inames);
  setOutputNames(onames);

  if(nbInputs()!=currentNbIn || nbOutputs()!=currentNbOut) {
    initEmptyData(pos());
    const vector<pair<NodeInterface *,Vector2u> > c; 
    setInputData(c);

    // if no input and nb nodes have changed, prepare the node...
    if(nbInputs()==0 && (currentNbIn>0 || currentNbOut!=nbOutputs())) {
      initOutputData();
    }
  }
}
