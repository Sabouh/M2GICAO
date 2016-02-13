// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_INTERFACE_H
#define NODE_INTERFACE_H

#include <iostream>
#include <vector>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QWidget>
#include <assert.h>

#include "misc/extinclude.h"
#include "misc/gpuProgram.h"
#include "misc/vertexarrayObject.h"
#include "misc/texture2D.h"
#include "misc/textDrawer.h"
#include "view/nodeWidget.h"
#include "core/sceneIOTags.h"

class PbGraph;
class NodeHandle;
class NodeManager;

// default data class
class LIBRARY_EXPORT Data {
 public:
  enum {DEFAULT,TEX2D};

  Data(int t=DEFAULT,const Vector2f &p=Vector2f(0,0),const Vector2f &s=Vector2f(0,0))
    : _type(t), _pos(p), _size(s), _selected(false) {}
  virtual ~Data() {}

  inline void setType(int t)             {_type     = t;}
  inline void setPos(const Vector2f &p)  {_pos      = p;}
  inline void setSize(const Vector2f &s) {_size     = s;}
  inline void setSelected(bool s)        {_selected = s;}

  inline int  type()            const {return _type;}
  inline bool selected()        const {return _selected;}
  inline const Vector2f &pos()  const {return _pos;}
  inline const Vector2f &size() const {return _size;}
  inline virtual bool ready()   const {return true;}
  inline virtual bool sameAs(Data *d) const {return _type==d->type();}
  inline virtual void copy(Data *d) {_pos = d->pos();_size = d->size();}

 protected:
  int      _type;
  Vector2f _pos;
  Vector2f _size;
  bool     _selected;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// a simple class for sending/receving events
class LIBRARY_EXPORT NodeEvent {
 public:
  enum {OUTPUT_SIZE_HAS_CHANGED=0,FRAME_HAS_CHANGED=1};
  NodeEvent(int event=0) {_event=event;}
  
  inline int type () const {return _event;}

 private:
  int _event;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// a simple class for dealing with node conditions 
class LIBRARY_EXPORT NodeCondition {
 public:
  enum {PARAMETERS_CHANGED=0};
  NodeCondition(int condition=0) {_condition=condition;}

  inline int type() const {return _condition;}

 private:
  int _condition;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class LIBRARY_EXPORT NodeInterface : public GlContext, public IOData {
  // declare PbGraph as friend (only the graph should be able to set graph ids)
  friend class PbGraph;
  friend class NodeGrouped;
  friend class NodeManager;

public:
  /* PART SPECIFIC TO USER DEFINED NODES */
  /* these functions might be overloaded */

  NodeInterface(PbGraph *parent,NodeHandle *handle);
  virtual ~NodeInterface();

  // processing functions (called only if isReady() returns true)
  inline virtual void apply () {}   // process the input(s)
  inline virtual void reload() {}   // reload data/shaders
  inline virtual void init  () {}   // init data (at connection)
  inline virtual void clean () {}   // clean data (at deconnection)

  // the widget, containing control parameters for the node
  inline virtual NodeWidget *widget() {return NULL;}

  // interaction events (only if isReady() is true)
  inline virtual void keyPressEvent    (QKeyEvent *          ) {}
  inline virtual void mousePressEvent  (const Vector2f &,QMouseEvent *) {}
  inline virtual void mouseMoveEvent   (const Vector2f &,QMouseEvent *) {}
  inline virtual void mouseReleaseEvent(const Vector2f &,QMouseEvent *) {}
  inline virtual void wheelEvent       (const Vector2f &,QWheelEvent *) {}

  // use these function to load and save your parameters 
  inline virtual bool save(SceneSaver  *) {return true;}
  inline virtual bool load(SceneLoader *) {return true;}

  // received if isReady() is true and if a special change has been made (like the content size has changed, etc)
  inline virtual void receiveEvent(const NodeEvent &event);

  // check if the node satisfies the given condition
  inline virtual bool satisfy(const NodeCondition &condition);

 public:
  /*   DATA ACCESSING FUNCTIONS   */
  /*  assume that indices are ok  */

  // node position / selected accessors
  inline bool selected() const            {return _selected;}
  inline void setSelected(bool s)         {_selected = s;   }
  inline const Vector2f &pos()  const     {return _pos;     }
  inline const Vector2f &size() const     {return _size;    }
  inline void setPos(const Vector2f &pos) {_pos = pos;      }
  inline void offsetPos(const Vector2f &offset);
  inline void offsetOutputPos(const Vector2f &offset,unsigned int i);
  
  // data accessors
  inline virtual Data *input (unsigned int i)         const {return _inputData[i]; }
  inline virtual Data *output(unsigned int i)         const {return _outputData[i];}
  inline virtual const std::vector<Data *> &inputs () {return _inputData;    }
  inline virtual const std::vector<Data *> &outputs() {return _outputData;   }

  // data position accessors
  inline const Vector2f &inputPos (unsigned int i) const {return input(i)->pos(); }
  inline const Vector2f &outputPos(unsigned int i) const {return output(i)->pos();}
  inline void setInputPos (const Vector2f &p,unsigned int i) {input(i)->setPos(p); }
  inline void setOutputPos(const Vector2f &p,unsigned int i) {output(i)->setPos(p);}

  // data size accessors
  inline const Vector2f &inputSize (unsigned int i) const {return input(i)->size(); }
  inline const Vector2f &outputSize(unsigned int i) const {return output(i)->size();}

  // data type accessors
  inline int inputType (unsigned int i) const {return input(i)->type(); }
  inline int outputType(unsigned int i) const {return output(i)->type();}

  // useful function to access informations from the node handle
  const QString     stringId       () const;
  unsigned int      version        () const;
  const QString     name           () const;
  const QString     path           () const;
  const QString     desc           () const;
  const QString     help           () const;
  const QStringList inputNames     () const;
  const QStringList outputNames    () const;
  const QString     stringIdVersion() const;
  unsigned int      nbInputs       () const;
  unsigned int      nbOutputs      () const;

  void setStringId(const QString &stringId);
  void setVersion(unsigned int version);
  void setName(const QString &name);
  void setPath(const QString &path);
  void setDesc(const QString &desc);
  void setHelp(const QString &help);
  void setInputNames(const QStringList &inputNames);
  void setOutputNames(const QStringList &outputNames);

  inline const QString nameId();  // return name+nodeId
  inline const QString plugDir(); // plugin directory
  const QStringList namesId();  // return parentname+nameId

  // node ids 
  inline unsigned int nodeId  () const {return _nodeId; }
  inline unsigned int graphId () const {return _graphId;}

 public:
  /* PART SPECIFIC TO EACH TYPE OF NODE  */
  /* these functions might be overloaded */

  // list of states that a node can take
  enum {NO_CHANGE,READY_TO_READY,NOT_READY_TO_NOT_READY,READY_TO_NOT_READY,NOT_READY_TO_READY};
  
  // check if the node is ready to be applied
  inline virtual bool isReady() const {return _ready;}

  // check if the node is ready to be applied
  virtual bool isPartlyConnected();

  // check wether the given data type fits with this node
  inline virtual bool checkDataType(NodeInterface *inputNode,unsigned int outputId,unsigned int inputId);

  // return true if sufficient data in the given array is available for the node to be applied
  inline virtual bool checkInputData(const std::vector<Data *> &data) const;

  // initialize default size and positions
  virtual void initDefaultPositions();
  
  // update node size and output positions
  virtual void updatePositions();

  // create input/output arrays (should be the first function called after the constructor)
  virtual void initEmptyData(const Vector2f &pos=Vector2f(0,0));
  
  // clean inputs and outputs 
  virtual void cleanData();

  // clean data + widget
  virtual void cleanEverything();
  
  // the vector is composed of <NodeInterface *,<outputId,inputId>>
  virtual void setInputData(const std::vector<std::pair<NodeInterface *,Vector2u> > &dinputs);

  // the vector is composed of ordered input data>
  //virtual void setInputData(const std::vector<Data *> &newData);

  // call this function when the pointer to the input data is not valid anymore 
  // (when input has been recreated)
  virtual void reloadInputData();

 // init all the output data and set the _ready variable (inputs should be ok)
  virtual void initOutputData() {}

  // clean data and set the _ready variable
  virtual void cleanOutputData() {}

  // node and data coordinates 
  inline virtual const Vector2f inputNodeCoord     (unsigned int i) const;
  inline virtual const Vector2f outputNodeCoord    (unsigned int i) const;
  inline virtual const Vector2f inputNodeDataCoord (unsigned int i) const;
  inline virtual const Vector2f outputNodeDataCoord(unsigned int i) const;

  // selection tests
  inline virtual bool pointInNode(const Vector2f &pt) const;
  inline virtual bool pointInOutput(const Vector2f &pt,unsigned int i) const;
  inline virtual bool nodeInSquare(const Vector2f &pt1,const Vector2f &pt2) const;
  inline virtual bool outputInSquare(const Vector2f &pt1,const Vector2f &pt2,unsigned int i) const;

  // dedicated function for drawing the node+output data when rendering (OpenGL)
  inline virtual void draw(const Matrix4f &mvp=Matrix4f::Identity());
  inline virtual void drawOutputContent(const Matrix4f &,const Vector2f &,unsigned int ) {}

  // size of the output content of the node (usefull for the user interaction)
  inline virtual const Vector2f outputContentSize(unsigned int) {return Vector2f(512.0f,512.0f);}

  // update the graph if this node has changed
  virtual void update();

  // send event to the connected nodes 
  virtual void sendEvent(const NodeEvent &event);

  // return the full path+filename in which the output i will be stored, or empty if no file 
  virtual const QString prepareToExportOutputContent(unsigned int) {return QString();}

  // save the outut i at the specified filename
  virtual bool exportOutputContent(const QString &,unsigned int) {return false;}

 private:
  /* PART ONLY USED BY THE PARENT GRAPH  */

  unsigned int _nodeId;    // id of this node inside the graph
  unsigned int _graphId;   // id of the connex graph to which this node belongs to

  // setting graph ids (only accessible from the friend PbGraph)
  inline void setNodeId   (unsigned int nodeId   ) {_nodeId    = nodeId; }
  inline void setGraphId  (unsigned int graphId  ) {_graphId   = graphId;}

  inline PbGraph *parentGraph() const {return _parent;}
  inline void setParentGraph(PbGraph *parent) {_parent=parent;}
  
  // state of the node (to remember if the node needs to be updated)
  inline virtual int state() const {return _state;}


 protected:
  std::vector<Data *> _inputData;  // input elements 
  std::vector<Data *> _outputData; // output elements
  
  PbGraph    *_parent;     // parent graph 
  NodeHandle *_handle;     // node handle
  bool        _ready;      // is this node ready?
  int         _state;      // does this node need to be updated?
  Vector2f    _pos;        // node position
  Vector2f    _size;       // node size
  bool        _selected;   // is this node currently selected?

 protected:
  // static GPU data 
  static VertexarrayObject *_rectangleVao;  // convenient buffer for drawing a textured rectangle
  static VertexarrayObject *_unitSquareVao; // convenient buffer for drawing a textured unit square 
  static GPUProgram        *_pconnect;      // shader to draw the connections
  static GPUProgram        *_pstyle;        // shader to draw the boxes
  static TextDrawer        *_textDrawer;    // allows to easily draw text 
  static int                _nbInstances;  

  // convenient VAO that allows to draw squares and rectangles 
  static void initGPUData();
  static void clearGPUData();

  // convenient function that copy p in pts and return the offset pointer position
  inline float *copyToBuffer(const Vector2f &p,float *pts) {*pts=p[0]; pts++; *pts=p[1]; pts++;return pts;}

  // init buffers necessary for displaying the node
  void initDrawingBuffers();

 private:
  // dynamic GPU data
  VertexarrayObject *_drawVao;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};








/* DEFINE AN EMPTY NODE */
class LIBRARY_EXPORT DefaultNode : public NodeInterface {
 public:
 DefaultNode(PbGraph *parent,const QString &name,const QString &path,
	     const QStringList &inames,const QStringList &onames);
  
  inline virtual void initOutputData () {_ready=true; }
  inline virtual void cleanOutputData() {_ready=false;}

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};















/* INLINE FUNCTIONS */
inline const QString NodeInterface::nameId() {
  return name()+QString(" (")+QString::number(nodeId())+QString(")");
}

inline const QString NodeInterface::plugDir() {
  return QCoreApplication::applicationDirPath()+"/nodes";
}

inline bool NodeInterface::checkDataType(NodeInterface *inputNode,unsigned int outputId,unsigned int inputId) {
  return inputNode->outputType(outputId)==inputType(inputId);
}

inline bool NodeInterface::checkInputData(const std::vector<Data *> &inputs) const {
  if(nbInputs()>inputs.size()) return false;

  for(unsigned int i=0;i<nbInputs();++i) {
    if(!(inputs[i]) || !(inputs[i]->ready())) return false;
  }

  return true;
}

inline void NodeInterface::offsetPos(const Vector2f &offset) {
  _pos += offset;
}

inline void NodeInterface::offsetOutputPos(const Vector2f &offset,unsigned int i) {
  assert(i<nbOutputs());
  output(i)->setPos(output(i)->pos()+offset);
}

inline const Vector2f NodeInterface::inputNodeCoord(unsigned int i) const {
  assert(i<nbInputs());
  const float s = 1.0f/(float)(nbInputs());
  const float v = s*((float)(nbInputs()-1-i)+0.5f)*_size[1];
  return Vector2f(_pos[0],_pos[1]+v);
}

inline const Vector2f NodeInterface::outputNodeCoord(unsigned int i) const {
  assert(i<nbOutputs());
  const float s = 1.0f/(float)(nbOutputs());
  const float v = s*((float)(nbOutputs()-1-i)+0.5f)*_size[1];
  return Vector2f(_pos[0]+_size[0],_pos[1]+v);
}

inline const Vector2f NodeInterface::inputNodeDataCoord(unsigned int i) const {
  assert(i<nbOutputs());
  const Vector2f apos = _pos+outputPos(i);
  return Vector2f(apos[0],apos[1]+outputSize(i)[1]/2.0f);
}

inline const Vector2f NodeInterface::outputNodeDataCoord(unsigned int i) const {
  assert(i<nbOutputs());
  const Vector2f apos = _pos+outputPos(i);
  return Vector2f(apos[0]+outputSize(i)[0],apos[1]+outputSize(i)[1]/2.0f);
}

inline bool NodeInterface::pointInNode(const Vector2f &pt) const {
  return (pt[0]>=_pos[0] && pt[0]<=_pos[0]+_size[0] && 
	  pt[1]>=_pos[1] && pt[1]<=_pos[1]+_size[1]);
}

inline bool NodeInterface::pointInOutput(const Vector2f &pt,unsigned int i) const {
  assert(i<nbOutputs());
  const Vector2f pos  = _pos+outputPos(i);
  const Vector2f size = outputSize(i);
  return (pt[0]>=pos[0] && pt[0]<=pos[0]+size[0] && 
	  pt[1]>=pos[1] && pt[1]<=pos[1]+size[1]);
}

inline bool NodeInterface::nodeInSquare(const Vector2f &pt1,const Vector2f &pt2) const {
  const Vector2f pmin(std::min(pt1[0],pt2[0]),std::min(pt1[1],pt2[1]));
  const Vector2f pmax(std::max(pt1[0],pt2[0]),std::max(pt1[1],pt2[1]));
  const Vector2f p[4] = {_pos,_pos+Vector2f(_size[0],0),_pos+Vector2f(0,_size[1]),_pos+_size};

  return (p[0][0]>=pmin[0] && p[0][0]<=pmax[0] && p[0][1]>=pmin[1] && p[0][1]<=pmax[1] && 
	  p[1][0]>=pmin[0] && p[1][0]<=pmax[0] && p[1][1]>=pmin[1] && p[1][1]<=pmax[1] && 
	  p[2][0]>=pmin[0] && p[2][0]<=pmax[0] && p[2][1]>=pmin[1] && p[2][1]<=pmax[1] && 
	  p[3][0]>=pmin[0] && p[3][0]<=pmax[0] && p[3][1]>=pmin[1] && p[3][1]<=pmax[1]);
}

inline bool NodeInterface::outputInSquare(const Vector2f &pt1,const Vector2f &pt2,unsigned int i) const {
  assert(i<nbOutputs());
  const Vector2f pmin(std::min(pt1[0],pt2[0]),std::min(pt1[1],pt2[1]));
  const Vector2f pmax(std::max(pt1[0],pt2[0]),std::max(pt1[1],pt2[1]));
  const Vector2f pos  = _pos+outputPos(i);
  const Vector2f size = outputSize(i);
  const Vector2f p[4] = {pos,pos+Vector2f(size[0],0),pos+Vector2f(0,size[1]),pos+size};

  return (p[0][0]>=pmin[0] && p[0][0]<=pmax[0] && p[0][1]>=pmin[1] && p[0][1]<=pmax[1] && 
	  p[1][0]>=pmin[0] && p[1][0]<=pmax[0] && p[1][1]>=pmin[1] && p[1][1]<=pmax[1] && 
	  p[2][0]>=pmin[0] && p[2][0]<=pmax[0] && p[2][1]>=pmin[1] && p[2][1]<=pmax[1] && 
	  p[3][0]>=pmin[0] && p[3][0]<=pmax[0] && p[3][1]>=pmin[1] && p[3][1]<=pmax[1]);
}

inline void NodeInterface::draw(const Matrix4f &mvp) {
  const float psize = 15.0f;

  const Vector3f pIn(0.4f,0.8f,0.2f);
  const Vector3f pOut(0.8f,0.3f,0.2f);
  const Vector4f nodebcol(0.75f,0.7f,0.85f,0.4f);
  const Vector4f nodelcol(0.0f,0.0f,0.0f,1.0f);
  const Vector4f outbucol(1.0f,1.0f,1.0f,0.5f);
  const Vector4f outbscol(0.8f,0.8f,0.5f,0.7f);

  float *pts = (float *)_drawVao->mapAttribUnsync(0);
  
  // update input coords 
  for(unsigned int i=0;i<nbInputs();++i) {
    pts = copyToBuffer(inputNodeCoord(i),pts); 
  }
  
  // update output coords 
  for(unsigned int i=0;i<nbOutputs();++i) { 
    pts = copyToBuffer(outputNodeCoord(i),pts); 
  }

  // update node coords 
  pts = copyToBuffer(_pos,pts);
  pts = copyToBuffer(_pos+Vector2f(_size[0],0),pts);
  pts = copyToBuffer(_pos+_size,pts);
  pts = copyToBuffer(_pos+_size,pts);
  pts = copyToBuffer(_pos+Vector2f(0,_size[1]),pts);
  pts = copyToBuffer(_pos,pts);

  // update node output coords
  for(unsigned int i=0;i<nbOutputs();++i) {
    const Vector2f tmpPos = _pos+outputPos(i);
    pts = copyToBuffer(tmpPos,pts);
    pts = copyToBuffer(tmpPos+Vector2f(outputSize(i)[0],0),pts);
    pts = copyToBuffer(tmpPos+outputSize(i),pts);
    pts = copyToBuffer(tmpPos+outputSize(i),pts);
    pts = copyToBuffer(tmpPos+Vector2f(0,outputSize(i)[1]),pts);
    pts = copyToBuffer(tmpPos,pts);
  }

  _drawVao->unmapAttrib();
  _drawVao->bind();

  _pconnect->enable();
  _pconnect->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pconnect->setUniform1f("size",psize);

  if(nbInputs()>0) {
    _pconnect->setUniform3fv("color",(GLfloat *)pIn.data());
    _drawVao->drawArrays(GL_POINTS,0,nbInputs());
  }

  if(nbOutputs()>0) {
    _pconnect->setUniform3fv("color",(GLfloat *)pOut.data());
    _drawVao->drawArrays(GL_POINTS,nbInputs(),nbOutputs());
  }

  _pconnect->disable();

  _pstyle->enable();
  _pstyle->setUniformMatrix4fv("mvp",(GLfloat *)mvp.data());
  _pstyle->setUniform4fv("backgroundcol",(GLfloat *)nodebcol.data());
  _pstyle->setUniform4fv("linecol",(GLfloat *)nodelcol.data());
  _pstyle->setUniform1i("selected",_selected);
  _pstyle->setUniform1f("roundness",20.0f);
  _drawVao->drawArrays(GL_TRIANGLES,nbInputs()+nbOutputs(),6);
  
  _pstyle->setUniform1f("roundness",30.0f);
  _pstyle->setUniform1i("selected",false);

  // outputs
  for(unsigned int i=0;i<nbOutputs();++i) {
    if(output(i)->selected()) {
      _pstyle->setUniform4fv("backgroundcol",(GLfloat *)outbscol.data());
      _pstyle->setUniform4fv("linecol",(GLfloat *)outbscol.data());
    } else {
      _pstyle->setUniform4fv("backgroundcol",(GLfloat *)outbucol.data());
      _pstyle->setUniform4fv("linecol",(GLfloat *)outbucol.data());
    }

    _drawVao->drawArrays(GL_TRIANGLES,nbInputs()+nbOutputs()+6+i*6,6);
  }

  _pstyle->disable();
  _drawVao->unbind();

  // node text
  _textDrawer->draw(nameId(),_pos+Vector2f(0,_size[1]+5),
  		    Vector2f(_size[0],_size[0]/10.0f),mvp,Vector4f(1,0,0,1));

  // output text
  const QStringList lt = outputNames();
  for(int i=0;i<lt.size();++i) {
    const Vector2f tmpPos = _pos+outputPos(i)+Vector2f(0,outputSize(i)[1]);
    _textDrawer->draw(lt[i],tmpPos,Vector2f(outputSize(i)[0],outputSize(i)[0]/10.0f),mvp,Vector4f(0,0,0,1));
  }

  // output contents
  if(isReady()) {
    for(unsigned int i=0;i<nbOutputs();++i) {
      const Vector2f o = (outputSize(i)-outputContentSize(i))/2.0f;
      drawOutputContent(mvp,_pos+outputPos(i)+o,i);
    }
  }
}

inline void NodeInterface::receiveEvent(const NodeEvent &event) {
  if(event.type()==NodeEvent::OUTPUT_SIZE_HAS_CHANGED) {
    initDrawingBuffers();
  } else if(event.type()==NodeEvent::FRAME_HAS_CHANGED && widget()) {
    widget()->computeNewFrameValues();
  }
}

inline bool NodeInterface::satisfy(const NodeCondition &condition) {
  if(condition.type()==NodeCondition::PARAMETERS_CHANGED && widget()) {
    return widget()->currentValuesHaveChanged();
  }
  
  return false;
}

#endif // NODE_INTERFACE_H
