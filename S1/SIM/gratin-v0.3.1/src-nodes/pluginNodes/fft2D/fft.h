// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2015-2016 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef FFT2D_H
#define FFT2D_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include "view/nodeWidget.h"
#include "view/widgetParams.h"
#include "core/nodeHandle.h"
#include "core/nodeTexture2D.h"

// **** a widget ****
class FFT2DWidget : public NodeWidget {
  Q_OBJECT

 public:
  FFT2DWidget(NodeInterface *node);

  inline bool invert  () const {return _invertBox->currentIndex()==1;}
  inline bool realImag() const {return _outputBox->currentIndex()==1;}
  inline bool shift   () const {return _shift->checkState()==Qt::Checked;}

  inline void setInvert  (bool i) {_invertBox->setCurrentIndex(i ? 1 : 0);}
  inline void setRealImag(bool r) {_outputBox->setCurrentIndex(r ? 1 : 0);}
  inline void setShift   (bool s) {_shift->setCheckState(s ? Qt::Checked : Qt::Unchecked);}

  inline void setUpdate(bool update) {_update = update;}

  public slots:
  void needUpdate() {if(node()->isReady() && _update) node()->update();}
  void needSetShaders();

 private:
  QComboBox *_invertBox;
  QComboBox *_outputBox;
  QCheckBox *_shift;
  bool       _update;
};




// **** the node itself, containing shaders and widgets ****
class FFT2DNode : public NodeTexture2D {
 public:
  enum {FFT_FORWARD=0,FFT_BACKWARD=1};

  FFT2DNode(PbGraph *parent=NULL,NodeHandle *handle=NULL);
  ~FFT2DNode();

  void init();
  void clean();
  void apply();
  void receiveEvent(const NodeEvent &event);
  bool save(SceneSaver  *saver);
  bool load(SceneLoader *loader);

  inline NodeWidget *widget() {return _w;} 
  
  void setShaderFiles(int type);
  void setShaderUniforms(int type);

 protected:
  // need 1 tmp texture
  inline unsigned int nbTmps()  const {return 1;} 

 private:
  void initFFTData();
  void cleanFFTData();
  void createButterflyTables(int d);
  void initTextures(int d);

  std::vector<Vector4f        > _risCPU[2];
  std::vector<FloatTexture2D *> _risGPU[2];

  GPUProgram     *_copInProg;
  GPUProgram     *_copOutProg;
  GPUProgram     *_fftProg;
  Vector2u        _size;
  Vector2u        _stages;  
  unsigned int    _initPass;
  unsigned int    _currPass;
  GLenum         *_outbuff[2];
  FloatTexture2D *_inSampl[2];
  FFT2DWidget    *_w;  

  inline int bitReverse(int i, int N);
};




// **** the node handle, containing information displayed in the interface ****
class FFT2DHandle : public QObject, public NodeHandleInterface {
  Q_OBJECT
    Q_INTERFACES(NodeHandleInterface)
    Q_PLUGIN_METADATA(IID "Gratin.FFT2D")

    public:
  const QString     stringId() const {return "FFT2DId";}
  unsigned int      version () const {return 1;}

  const QString     name    () const {return "2DFourierTransform";}
  const QString     path    () const {return "Analysis/";}
  const QString     desc    () const {return "Fast 2D Discrete Fourier Transfom";}
  const QString     help    () const {return tr("Fast 2D Discrete Fourier transform on  gray level image\n"
						"forwad mode: spatial to frequency transformation (the first input channel is considered)\n"
						"\t FFTShift automatically rearranges the image so that low and high \n"
						"\t frequencies are respectively located in the center and in the borders\n"
						"\t The output is in the form magnitude/phase or real/imaginary (located in the 2 first channels)\n"
						"backward mode: frequency to spatial transformation (the 2 first input channels are considered)\n"
						"\t FFTShift automatically rearranges the image so that low and high \n"
						"\t frequencies are respectively located in the borders and in the center\n"
						"\t The input can be either in the form magnitude/phase or real/imaginary (located in the 2 first channels)\n"
						"\t The output contains the result, duplicated in the 3 first channels\n"

						);}
  
  const QStringList inputNames () const {return QStringList() << "image"; }
  const QStringList outputNames() const {return (QStringList() << "FourierTransform");}
  
  NodeInterface *createInstance(PbGraph *parent) {
    return new FFT2DNode(parent,new NodeHandle(this));
  }

  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) {    
    return new FFT2DNode(parent,new NodeHandle(stringId(),version(),name,path,desc,help,
						   inputNames,outputNames));
  }
};

inline int FFT2DNode::bitReverse(int i,int n) {
  int j = 0;
  
  while((n = n>>1)!=0) {
    j = (j<<1) | (i&1);
    i = i>>1;
  }
  
  return j;
}

#endif // FFT2D_H
