// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef IMGCUSTOM_H
#define IMGCUSTOM_H

#include <QObject>
#include "genericCustomNode.h"
#include "genericCustomWidget.h"
#include "core/nodeHandle.h"
#include "misc/gpuProgram.h"

class PingPongCustomNode;

class PingPongCustomWidget : public GenericCustomWidget {
  Q_OBJECT
    
    public:
  PingPongCustomWidget(PingPongCustomNode *node);

  inline int pingPongNumber() {return _pingPongNumber->value();}
  inline void setPingPongNumber(int v) {_pingPongNumber->setValue(v);}
  inline void setPingPongNumberLabel(unsigned int n) {
    _pingPongNumber->setSuffix(" (total="+QString::number(n)+")");
  }

  inline bool reinitWhenChanged() const {return _reinitWhenChanged->checkState()==Qt::Checked;}
  inline bool updateOnceChanged() const {return _updateOnceChanged->checkState()==Qt::Checked;}
  inline bool updateAllChanged() const {return _updateAllChanged->checkState()==Qt::Checked;}
  inline bool refreshAllFrames() const {return _refreshAllFrames->checkState()==Qt::Checked;}

  inline void setReinitWhenChanged(bool v) {_reinitWhenChanged->setCheckState(v ? Qt::Checked : Qt::Unchecked);}
  inline void setUpdateOnceChanged(bool v) {_updateOnceChanged->setCheckState(v ? Qt::Checked : Qt::Unchecked);}
  inline void setUpdateAllChanged(bool v) {_updateAllChanged->setCheckState(v ? Qt::Checked : Qt::Unchecked);}
  inline void setRefreshAllFrames(bool v) {_refreshAllFrames->setCheckState(v ? Qt::Checked : Qt::Unchecked);}

  protected slots:
  void initClicked();
  void applyPingPongClicked();

 private:
  QPushButton   *_init;
  QPushButton   *_applyPingPong;
  QSpinBox      *_pingPongNumber;
  QCheckBox     *_reinitWhenChanged;
  QCheckBox     *_updateOnceChanged;
  QCheckBox     *_updateAllChanged;
  QCheckBox     *_refreshAllFrames;
};


class PingPongCustomNode : public GenericCustomNode {
 public:
  PingPongCustomNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~PingPongCustomNode();

  enum {NORMAL_MODE=0,INIT_MODE=1,PP_MODE=2};

  void init();
  void apply ();
  void applyPingPong(bool refreshAll=false,bool refreshIt=false);
  void initPingPong(bool refresh=false);
  void applyOnce(bool refresh=false);
  void setMode(unsigned int m) {_mode=m;}

  inline NodeWidget *widget() {return _w;}

  virtual const QString constantVertHead();
  virtual const QString defaultVertBody(); 
  virtual const QString constantFragHead();
  virtual const QString defaultFragBody(); 
  virtual const QString constantTessCHead();
  virtual const QString defaultTessCBody(); 
  virtual const QString constantTessEHead();
  virtual const QString defaultTessEBody(); 
  virtual const QString constantGeomHead(); 
  virtual const QString defaultGeomBody(); 

  virtual bool save(SceneSaver  *saver);
  virtual bool load(SceneLoader *loader);
  void settingsChanged();

 protected:
  virtual void initFBO();
  virtual void cleanFBO();
  virtual const QStringList defaultUniformNames() const { return _defautUniformNames; }
  virtual const QStringList defaultUniformTypes() const { return _defautUniformTypes; }

  inline void copyResult(unsigned int);

 private:
  void generateUniforms();

  PingPongCustomWidget         *_w;
  unsigned int                  _mode;
  unsigned int                  _currentPingPong;
  FramebufferObject            *_pingPongFbo[2];
  std::vector<FloatTexture2D *> _pingPongTex[2];
  GPUProgram                   *_copyBuffers;
  QStringList                   _defautUniformNames;
  QStringList                   _defautUniformTypes;
  bool                          _initialized;
};

class PingPongCustomHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
  Q_INTERFACES(NodeHandleInterface)
  Q_PLUGIN_METADATA(IID "Gratin.PingPongCustom")

 public:
  const QString     stringId() const {return "pingPongGenericId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "genericPingPong";}
  const QString     path    () const {return "Generic/";}
  const QString     desc    () const {return "Generic node for ping-pong image processing";}
  const QString     help    () const {
    return tr("This node was designed to create custom shaders\n"
	      "The settings widget allows to modify input, output parameters\n"
	      "as well as the output image sizes.\n\n"
	      "WARNING1: you will be able to modify the number of inputs/outputs\n"
	      "only if the node is entirely disconnected.\n"
	      "WARNING2: input/output names will not be modified if a custom node is created\n"
	      "from this node because these names should be edited directly in the setting widget\n"
	      "WARNING3: modifying the settings of a node will remove all previously\n"
	      "defined keyframes\n\n" 
	      "* Special data allows to modify the output image (using a multiple of\n"
	      "the input size, plus an offset vector\n"
	      "Check use mouse/keyboard if you want to interact with the mouse\n\n"
	      "* Input data allows to add/remove/displace input data (only if disconnected)\n\n"
	      "* Output data allows to add/remove/displace output data (only if disconnected)\n\n"
	      "* Param data allows to add/remove parameters that will automatically be \n"
	      "included in your shader.\n"
	      "For each of them you may choose a proper name, type, and min/max/default values.\n"
	      "Check keyframes if you want your parameter to be accessible\n"
	      " through the animation widget\n\n"
	      "In the node interface itself:\n"
	      "Source tab contains the head and body of your GLSL source (only the body is editable)\n"
	      "Params tab contains the defined widgets according to your setting choices\n"
	      "Log tab contains compilation errors\n\n"
	      "Once your settings and source completed, click on apply to see the result\n"
	      );
  }
  
  const QStringList inputNames () const {return QStringList() << "inBuffer0";}
  const QStringList outputNames() const {return QStringList() << "outBuffer0";}
  
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new PingPongCustomNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new PingPongCustomNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};

inline void PingPongCustomNode::copyResult(unsigned int currentOTex) {
  // need to copy results in outputs here
  setOutputParams();
  _copyBuffers->enable();

  // send textures here 
  for(unsigned int i=0;i<nbOutputs();++i) {
    _copyBuffers->setUniformTexture(_defautUniformNames[i],GL_TEXTURE_2D,_pingPongTex[currentOTex][i]->id());
  }
  
  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));
  _unitSquareVao->bind();
  _unitSquareVao->drawArrays(GL_TRIANGLES,0,6);
  _unitSquareVao->unbind();
  _copyBuffers->disable();

  FramebufferObject::unbind();
}

#endif // IMGCUSTOM_H
