// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef WIDGET_PARAMS
#define WIDGET_PARAMS

#include "misc/extinclude.h"
#include "misc/trackballCamera.h"
#include "core/keyframedParams.h"
#include "core/nodeInterface.h"
#include "view/nodeWidget.h"
#include "view/animationWidget.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QString>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QColorDialog>

class FloatAttribWidget;
class IntAttribWidget;
class Vector2fAttribWidget;
class Vector2iAttribWidget;
class Vector3fAttribWidget;
class Vector3iAttribWidget;
class Vector4fAttribWidget;
class Vector4iAttribWidget;
class FloatSliderWidget;
class FloatSpinWidget;
class IntSliderWidget;
class IntSpinWidget;
class Vector2fSpinWidget;
class Vector2iSpinWidget; 
class Vector3fSpinWidget;
class Vector3fColorWidget;
class Vector3iSpinWidget;
class Vector4fSpinWidget;
class Vector4fColorWidget;
class Vector4iSpinWidget;

class LIBRARY_EXPORT WidgetFactory {
 public:
  static NodeWidget           *createAttribWidget         (NodeInterface *node,const QString &name,int type,
							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static FloatAttribWidget    *createFloatAttribWidget    (NodeInterface *node,const QString &name,
							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static IntAttribWidget      *createIntAttribWidget      (NodeInterface *node,const QString &name,
							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector2fAttribWidget *createVector2fAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector2iAttribWidget *createVector2iAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector3fAttribWidget *createVector3fAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector3iAttribWidget *createVector3iAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector4fAttribWidget *createVector4fAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);
  static Vector4iAttribWidget *createVector4iAttribWidget (NodeInterface *node,const QString &name,
  							   int view,float minv=0.0f,float maxv=1.0f,
							   float defaultv=0.0f,bool kf=false);

  /* static const int FloatID; */
  /* static const int IntID; */
  /* static const int Vector2fID; */
  /* static const int Vector2iID; */
  /* static const int Vector3fID; */
  /* static const int Vector3iID; */
  /* static const int Vector4fID; */
  /* static const int Vector4iID; */

  static QStringList types();
  static QStringList views(int type);

  enum {
    FloatID    = 0,
    IntID      = 1,
    Vector2fID = 2,
    Vector2iID = 3,
    Vector3fID = 4,
    Vector3iID = 5,
    Vector4fID = 6,
    Vector4iID = 7
  };

 private:
  static QStringList _types;
  static QStringList _floatViews;
  static QStringList _intViews;
  static QStringList _vector2fViews;
  static QStringList _vector2iViews;
  static QStringList _vector3fViews;
  static QStringList _vector3iViews;
  static QStringList _vector4fViews;
  static QStringList _vector4iViews;
};

// *** AttribLayout ***
class LIBRARY_EXPORT AttribLayout {
 public:
  static QGridLayout *create(QWidget *left,QWidget *center,QWidget *right);
  static QGridLayout *create(QWidget *left,QLayout *center,QWidget *right);
};

// *** KeyframButtons ***
class LIBRARY_EXPORT KeyframeButtons : public QWidget {
 public:
  KeyframeButtons(QWidget *parent=NULL);

  inline QPushButton *setButton() {return _setButton;}
  inline QPushButton *editButton() {return _editButton;}

 private:
  QPushButton *_setButton;
  QPushButton *_editButton;
};

// *** FloatAttribWidget ***
class LIBRARY_EXPORT FloatAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  FloatAttribWidget(NodeInterface *node, const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f,bool keyframed=true);

  typedef enum {NONE=0,SLIDER=1,SPIN=2} Type;

  inline virtual Type    type     () const {return NONE;}
  inline QString name     () const {return _kf.name();}
  inline float   val      () const {return _kf.get();}
  inline float   minVal   () const {return _minVal;}
  inline float   maxVal   () const {return _maxVal;}
  inline bool    keyframed() const {return !_kfButtons->isHidden();}

  inline virtual void setMin(float minVal) {_minVal = minVal;}
  inline virtual void setMax(float maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(float v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel          *_nameLabel;
  KeyframeButtons *_kfButtons;
  KeyframedFloat   _kf;
  float            _minVal;
  float            _maxVal;
  bool             _update;
  bool             _changed;
};

// *** IntAttribWidget ***
class LIBRARY_EXPORT IntAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  IntAttribWidget(NodeInterface *node, const QString &name,int minVal=0,int maxVal=100,int value=0,bool keyframed=true);

  typedef enum {NONE=0,SLIDER=1,SPIN=2} Type;

  inline virtual Type    type     () const {return NONE;}
  inline QString name     () const {return _kf.name();}
  inline int     val      () const {return _kf.get();}
  inline int     minVal   () const {return _minVal;}
  inline int     maxVal   () const {return _maxVal;}
  inline bool    keyframed() const {return !_kfButtons->isHidden();}

  inline virtual void setMin(int minVal) {_minVal = minVal;}
  inline virtual void setMax(int maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(int v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel          *_nameLabel;
  KeyframeButtons *_kfButtons;
  KeyframedInt     _kf;
  int              _minVal;
  int              _maxVal;
  bool             _update;
  bool             _changed;
};


// *** Vector2fAttribWidget ***
class LIBRARY_EXPORT Vector2fAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector2fAttribWidget(NodeInterface *node, const QString &name,
		       const Vector2f &minVal=Vector2f(0.0f,0.0f),
		       const Vector2f &maxVal=Vector2f(1.0f,1.0f),
		       const Vector2f &value=Vector2f(0.0f,0.0f),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector2f val      () const {return _kf.get();}
  inline Vector2f minVal   () const {return _minVal;}
  inline Vector2f maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector2f &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector2f &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector2f &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector2f _kf;
  Vector2f          _minVal;
  Vector2f          _maxVal;
  bool              _update;
  bool              _changed;
};



// *** Vector2iAttribWidget ***
class LIBRARY_EXPORT Vector2iAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector2iAttribWidget(NodeInterface *node, const QString &name,
		       const Vector2i &minVal=Vector2i(0,0),
		       const Vector2i &maxVal=Vector2i(100,100),
		       const Vector2i &value=Vector2i(0,0),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector2i val      () const {return _kf.get();}
  inline Vector2i minVal   () const {return _minVal;}
  inline Vector2i maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector2i &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector2i &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector2i &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector2i _kf;
  Vector2i          _minVal;
  Vector2i          _maxVal;
  bool              _update;
  bool              _changed;
};



// *** Vector3fAttribWidget ***
class LIBRARY_EXPORT Vector3fAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector3fAttribWidget(NodeInterface *node, const QString &name,
		       const Vector3f &minVal=Vector3f(0.0f,0.0f,0.0f),
		       const Vector3f &maxVal=Vector3f(1.0f,1.0f,1.0f),
		       const Vector3f &value=Vector3f(0.0f,0.0f,0.0f),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1,COLOR=2} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector3f val      () const {return _kf.get();}
  inline Vector3f minVal   () const {return _minVal;}
  inline Vector3f maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector3f &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector3f &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector3f &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector3f _kf;
  Vector3f          _minVal;
  Vector3f          _maxVal;
  bool              _update;
  bool              _changed;
};



// *** Vector3iAttribWidget ***
class LIBRARY_EXPORT Vector3iAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector3iAttribWidget(NodeInterface *node, const QString &name,
		       const Vector3i &minVal=Vector3i(0,0,0),
		       const Vector3i &maxVal=Vector3i(100,100,100),
		       const Vector3i &value=Vector3i(0,0,0),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector3i val      () const {return _kf.get();}
  inline Vector3i minVal   () const {return _minVal;}
  inline Vector3i maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector3i &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector3i &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector3i &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector3i _kf;
  Vector3i          _minVal;
  Vector3i          _maxVal;
  bool              _update;
  bool              _changed;
};

// *** Vector4fAttribWidget ***
class LIBRARY_EXPORT Vector4fAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector4fAttribWidget(NodeInterface *node, const QString &name,
		       const Vector4f &minVal=Vector4f(0.0f,0.0f,0.0f,0.0f),
		       const Vector4f &maxVal=Vector4f(1.0f,1.0f,1.0f,1.0f),
		       const Vector4f &value=Vector4f(0.0f,0.0f,0.0f,0.0f),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1,COLOR=2} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector4f val      () const {return _kf.get();}
  inline Vector4f minVal   () const {return _minVal;}
  inline Vector4f maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector4f &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector4f &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector4f &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector4f _kf;
  Vector4f          _minVal;
  Vector4f          _maxVal;
  bool              _update;
  bool              _changed;
};



// *** Vector4iAttribWidget ***
class LIBRARY_EXPORT Vector4iAttribWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  Vector4iAttribWidget(NodeInterface *node, const QString &name,
		       const Vector4i &minVal=Vector4i(0,0,0,0),
		       const Vector4i &maxVal=Vector4i(100,100,100,100),
		       const Vector4i &value=Vector4i(0,0,0,0),
		       bool keyframed=true);
  typedef enum {NONE=0,SPIN=1} Type;

  inline virtual Type    type      () const {return NONE;}

  inline QString  name     () const {return _kf.name();}
  inline Vector4i val      () const {return _kf.get();}
  inline Vector4i minVal   () const {return _minVal;}
  inline Vector4i maxVal   () const {return _maxVal;}
  inline bool     keyframed() const {return !_kfButtons->isHidden();}
  
  inline virtual void setMin(const Vector4i &minVal) {_minVal = minVal;}
  inline virtual void setMax(const Vector4i &maxVal) {_maxVal = maxVal;}

  inline virtual void setVal(const Vector4i &v,bool update=true) {
    _kf.set(v);
    if(update) {
      updateGraph();
    }
  } 

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    _changed = _kf.frameChanged(current);
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();

 protected:
  void checkChangeCurve();

  QLabel           *_nameLabel;
  KeyframeButtons  *_kfButtons;
  KeyframedVector4i _kf;
  Vector4i          _minVal;
  Vector4i          _maxVal;
  bool              _update;
  bool              _changed;
};




// *** FloatSliderWidget ***
class LIBRARY_EXPORT FloatSliderWidget : public FloatAttribWidget {
  Q_OBJECT
    
    public:
  FloatSliderWidget(NodeInterface *node, const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f,bool keyframed=true);

  inline Type type() const {return FloatAttribWidget::SLIDER;}

  inline virtual void setVal(float v,bool update=true) {
    _update = update;
    _slider->setValue(floatToInt(v));
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  inline float intToFloat(int   v) {return (((float)v/_scale)*(_maxVal-_minVal))+_minVal;}
  inline int   floatToInt(float v) {return (int)(((v-_minVal)/(_maxVal-_minVal))*_scale);}

  QSlider *_slider;
  QLabel  *_valLabel;
  float    _scale;
  int      _precision;
};

// *** FloatSpinWidget ***
class LIBRARY_EXPORT FloatSpinWidget : public FloatAttribWidget {
  Q_OBJECT
    
    public:
  FloatSpinWidget(NodeInterface *node, const QString &name,float minVal=0.0f,float maxVal=1.0f,float value=0.0f,bool keyframed=true);

  inline Type type() const {return FloatAttribWidget::SPIN;}

  inline virtual void setMin(float minVal) {
    FloatAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spin->setMinimum((double)minVal);
    _spin->setSingleStep((double)(maxVal()-minVal)/100.0);
    _update = tmp;
  }

  inline virtual void setMax(float maxVal) {
    FloatAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spin->setMaximum((double)maxVal);
    _spin->setSingleStep((double)(maxVal-minVal())/100.0);
    _update = tmp;
  }

  inline virtual void setVal(float v,bool update=true) {
    _update = update;
    _spin->setValue((double)v);
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(double);

 private:
  QDoubleSpinBox *_spin;
};


// *** IntSliderWidget ***
class LIBRARY_EXPORT IntSliderWidget : public IntAttribWidget {
  Q_OBJECT
    
    public:
  IntSliderWidget(NodeInterface *node, const QString &name,int minVal=0,int maxVal=100,int value=0,bool keyframed=true);

  inline Type type() const {return IntAttribWidget::SLIDER;}

  inline virtual void setMin(int minVal) {
    IntAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _slider->setMinimum(minVal);
    _update = tmp;
  }

  inline virtual void setMax(int maxVal) {
    IntAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _slider->setMaximum(maxVal);
    _update = tmp;
  }

  inline virtual void setVal(int v,bool update=true) {
    _update = update;
    _slider->setValue(v);
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  QSlider *_slider;
  QLabel  *_valLabel;
};


// *** IntSpinWidget ***
class LIBRARY_EXPORT IntSpinWidget : public IntAttribWidget {
  Q_OBJECT
    
    public:
  IntSpinWidget(NodeInterface *node, const QString &name,int minVal=0,int maxVal=100,int value=0,bool keyframed=true);

  inline Type type() const {return IntAttribWidget::SPIN;}

  inline virtual void setMin(int minVal) {
    IntAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spin->setMinimum(minVal);
    _update = tmp;
  }

  inline virtual void setMax(int maxVal) {
    IntAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spin->setMaximum(maxVal);
    _update = tmp;
  }

  inline virtual void setVal(int v,bool update=true) {
    _update = update;
    _spin->setValue(v);
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  QSpinBox *_spin;
};


// *** Vector2fSpinWidget ***
class LIBRARY_EXPORT Vector2fSpinWidget : public Vector2fAttribWidget {
  Q_OBJECT
    
    public:
  Vector2fSpinWidget(NodeInterface *node, const QString &name,
		     const Vector2f &minVal=Vector2f(0.0f,0.0f),
		     const Vector2f &maxVal=Vector2f(1.0f,1.0f),
		     const Vector2f &value=Vector2f(0.0f,0.0f),
		     bool keyframed=true);

  inline Type type() const {return Vector2fAttribWidget::SPIN;}

  inline virtual void setMin(const Vector2f &minVal) {
    Vector2fAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum((double)minVal[0]);
    _spiny->setMinimum((double)minVal[1]);
    _spinx->setSingleStep((double)(maxVal()[0]-minVal[0])/100.0);
    _spiny->setSingleStep((double)(maxVal()[1]-minVal[1])/100.0);
    _update = tmp;
  }

  inline virtual void setMax(const Vector2f &maxVal) {
    Vector2fAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum((double)maxVal[0]);
    _spiny->setMaximum((double)maxVal[1]);
    _spinx->setSingleStep((double)(maxVal[0]-minVal()[0])/100.0);
    _spiny->setSingleStep((double)(maxVal[1]-minVal()[1])/100.0);
    _update = tmp;
  }

  inline virtual void setVal(const Vector2f &v,bool update=true) {
    _update = false;
    _spinx->setValue((double)v[0]);
    _spiny->setValue((double)v[1]);
    
    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(double);

 private:
  QDoubleSpinBox *_spinx;
  QDoubleSpinBox *_spiny;
};


// *** Vector2iSpinWidget ***
class LIBRARY_EXPORT Vector2iSpinWidget : public Vector2iAttribWidget {
  Q_OBJECT
    
    public:
  Vector2iSpinWidget(NodeInterface *node, const QString &name,
		     const Vector2i &minVal=Vector2i(0,0),
		     const Vector2i &maxVal=Vector2i(100,100),
		     const Vector2i &value=Vector2i(0,0),
		     bool keyframed=true);

  inline Type type() const {return Vector2iAttribWidget::SPIN;}

  inline virtual void setMin(const Vector2i &minVal) {
    Vector2iAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum(minVal[0]);
    _spiny->setMinimum(minVal[1]);
    _update = tmp;
  }

  inline virtual void setMax(const Vector2i &maxVal) {
    Vector2iAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum(maxVal[0]);
    _spiny->setMaximum(maxVal[1]);
    _update = tmp;
  }

  inline virtual void setVal(const Vector2i &v,bool update=true) {
    _update = false;
    _spinx->setValue(v[0]);
    _spiny->setValue(v[1]);
    
    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  QSpinBox *_spinx;
  QSpinBox *_spiny;
};


// *** Vector3fSpinWidget ***
class LIBRARY_EXPORT Vector3fSpinWidget : public Vector3fAttribWidget {
  Q_OBJECT
    
    public:
  Vector3fSpinWidget(NodeInterface *node, const QString &name,
		     const Vector3f &minVal=Vector3f(0.0f,0.0f,0.0f),
		     const Vector3f &maxVal=Vector3f(1.0f,1.0f,1.0f),
		     const Vector3f &value=Vector3f(0.0f,0.0f,0.0f),
		     bool keyframed=true);

  inline Type type() const {return Vector3fAttribWidget::SPIN;}

  inline virtual void setMin(const Vector3f &minVal) {
    Vector3fAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum((double)minVal[0]);
    _spiny->setMinimum((double)minVal[1]);
    _spinz->setMinimum((double)minVal[2]);
    _spinx->setSingleStep((double)(maxVal()[0]-minVal[0])/100.0);
    _spiny->setSingleStep((double)(maxVal()[1]-minVal[1])/100.0);
    _spinz->setSingleStep((double)(maxVal()[2]-minVal[2])/100.0);
    _update = tmp;
  }

  inline virtual void setMax(const Vector3f &maxVal) {
    Vector3fAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum((double)maxVal[0]);
    _spiny->setMaximum((double)maxVal[1]);
    _spinz->setMaximum((double)maxVal[2]);
    _spinx->setSingleStep((double)(maxVal[0]-minVal()[0])/100.0);
    _spiny->setSingleStep((double)(maxVal[1]-minVal()[1])/100.0);
    _spinz->setSingleStep((double)(maxVal[2]-minVal()[2])/100.0);
    _update = tmp;
  }

  inline virtual void setVal(const Vector3f &v,bool update=true) {
    _update = false;
    _spinx->setValue((double)v[0]);
    _spiny->setValue((double)v[1]);
    _spinz->setValue((double)v[2]);

    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(double);

 private:
  QDoubleSpinBox *_spinx;
  QDoubleSpinBox *_spiny;
  QDoubleSpinBox *_spinz;
};


// *** Vector3fColorWidget ***
class LIBRARY_EXPORT Vector3fColorWidget : public Vector3fAttribWidget {
  Q_OBJECT
    
    public:
  Vector3fColorWidget(NodeInterface *node, const QString &name,
		     const Vector3f &minVal=Vector3f(0.0f,0.0f,0.0f),
		     const Vector3f &maxVal=Vector3f(1.0f,1.0f,1.0f),
		     const Vector3f &value=Vector3f(0.0f,0.0f,0.0f),
		     bool keyframed=true);

  inline Type type() const {return Vector3fAttribWidget::COLOR;}


  inline virtual void setVal(const Vector3f &v,bool update=true) {
    _update = false;
    setButtonColor(v);

    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void buttonClicked();
  void colorChanged(const QColor &color);

 private:
  QPushButton  *_button;
  QColorDialog *_colorPicker;

  inline void setButtonColor(const Vector3f &color) {
    int r = (int)(color[0]*255.0f);
    int g = (int)(color[1]*255.0f);
    int b = (int)(color[2]*255.0f);
    const QString c = "("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
    _button->setStyleSheet("background-color: rgb"+c+"; color: rgb(255, 255, 255)");
  }
};


// *** Vector3iSpinWidget ***
class LIBRARY_EXPORT Vector3iSpinWidget : public Vector3iAttribWidget {
  Q_OBJECT
    
    public:
  Vector3iSpinWidget(NodeInterface *node, const QString &name,
		     const Vector3i &minVal=Vector3i(0,0,0),
		     const Vector3i &maxVal=Vector3i(100,100,100),
		     const Vector3i &value=Vector3i(0,0,0),
		     bool keyframed=true);

  inline Type type() const {return Vector3iAttribWidget::SPIN;}

  inline virtual void setMin(const Vector3i &minVal) {
    Vector3iAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum(minVal[0]);
    _spiny->setMinimum(minVal[1]);
    _spinz->setMinimum(minVal[2]);
    _update = tmp;
  }

  inline virtual void setMax(const Vector3i &maxVal) {
    Vector3iAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum(maxVal[0]);
    _spiny->setMaximum(maxVal[1]);
    _spinz->setMaximum(maxVal[2]);
    _update = tmp;
  }

  inline virtual void setVal(const Vector3i &v,bool update=true) {
    _update = false;
    _spinx->setValue(v[0]);
    _spiny->setValue(v[1]);
    _spinz->setValue(v[2]);
    
    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  QSpinBox *_spinx;
  QSpinBox *_spiny;
  QSpinBox *_spinz;
};



// *** Vector4fSpinWidget ***
class LIBRARY_EXPORT Vector4fSpinWidget : public Vector4fAttribWidget {
  Q_OBJECT
    
    public:
  Vector4fSpinWidget(NodeInterface *node, const QString &name,
		     const Vector4f &minVal=Vector4f(0.0f,0.0f,0.0f,0.0f),
		     const Vector4f &maxVal=Vector4f(1.0f,1.0f,1.0f,1.0f),
		     const Vector4f &value=Vector4f(0.0f,0.0f,0.0f,0.0f),
		     bool keyframed=true);

  inline Type type() const {return Vector4fAttribWidget::SPIN;}

  inline virtual void setMin(const Vector4f &minVal) {
    Vector4fAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum((double)minVal[0]);
    _spiny->setMinimum((double)minVal[1]);
    _spinz->setMinimum((double)minVal[2]);
    _spinw->setMinimum((double)minVal[3]);
    _spinx->setSingleStep((double)(maxVal()[0]-minVal[0])/100.0);
    _spiny->setSingleStep((double)(maxVal()[1]-minVal[1])/100.0);
    _spinz->setSingleStep((double)(maxVal()[2]-minVal[2])/100.0);
    _spinw->setSingleStep((double)(maxVal()[3]-minVal[3])/100.0);
    _update = tmp;
  }

  inline virtual void setMax(const Vector4f &maxVal) {
    Vector4fAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum((double)maxVal[0]);
    _spiny->setMaximum((double)maxVal[1]);
    _spinz->setMaximum((double)maxVal[2]);
    _spinw->setMaximum((double)maxVal[3]);
    _spinx->setSingleStep((double)(maxVal[0]-minVal()[0])/100.0);
    _spiny->setSingleStep((double)(maxVal[1]-minVal()[1])/100.0);
    _spinz->setSingleStep((double)(maxVal[2]-minVal()[2])/100.0);
    _spinw->setSingleStep((double)(maxVal[3]-minVal()[3])/100.0);
    _update = tmp;
  }

  inline virtual void setVal(const Vector4f &v,bool update=true) {
    _update = false;
    _spinx->setValue((double)v[0]);
    _spiny->setValue((double)v[1]);
    _spinz->setValue((double)v[2]);
    _spinw->setValue((double)v[3]);

    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(double);

 private:
  QDoubleSpinBox *_spinx;
  QDoubleSpinBox *_spiny;
  QDoubleSpinBox *_spinz;
  QDoubleSpinBox *_spinw;
};







// *** Vector4fColorWidget ***
class LIBRARY_EXPORT Vector4fColorWidget : public Vector4fAttribWidget {
  Q_OBJECT
    
    public:
  Vector4fColorWidget(NodeInterface *node, const QString &name,
		     const Vector4f &minVal=Vector4f(0.0f,0.0f,0.0f,0.0f),
		     const Vector4f &maxVal=Vector4f(1.0f,1.0f,1.0f,1.0f),
		     const Vector4f &value=Vector4f(0.0f,0.0f,0.0f,0.0f),
		     bool keyframed=true);

  inline Type type() const {return Vector4fAttribWidget::COLOR;}

  inline virtual void setVal(const Vector4f &v,bool update=true) {
    _update = false;
    setButtonColor(v);

    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void buttonClicked();
  void colorChanged(const QColor &color);

 private:
  QPushButton  *_button;
  QColorDialog *_colorPicker;

  inline void setButtonColor(const Vector4f &color) {
    int r = (int)(color[0]*255.0f);
    int g = (int)(color[1]*255.0f);
    int b = (int)(color[2]*255.0f);
    const QString c = "("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
    _button->setStyleSheet("background-color: rgb"+c+"; color: rgb(255, 255, 255)");
  }
};


// *** Vector4iSpinWidget ***
class LIBRARY_EXPORT Vector4iSpinWidget : public Vector4iAttribWidget {
  Q_OBJECT
    
    public:
  Vector4iSpinWidget(NodeInterface *node, const QString &name,
		     const Vector4i &minVal=Vector4i(0,0,0,0),
		     const Vector4i &maxVal=Vector4i(100,100,100,100),
		     const Vector4i &value=Vector4i(0,0,0,0),
		     bool keyframed=true);

  inline Type type() const {return Vector4iAttribWidget::SPIN;}

  inline virtual void setMin(const Vector4i &minVal) {
    Vector4iAttribWidget::setMin(minVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMinimum(minVal[0]);
    _spiny->setMinimum(minVal[1]);
    _spinz->setMinimum(minVal[2]);
    _spinw->setMinimum(minVal[3]);
    _update = tmp;
  }

  inline virtual void setMax(const Vector4i &maxVal) {
    Vector4iAttribWidget::setMax(maxVal);
    bool tmp = _update;
    _update = false;
    _spinx->setMaximum(maxVal[0]);
    _spiny->setMaximum(maxVal[1]);
    _spinz->setMaximum(maxVal[2]);
    _spinw->setMaximum(maxVal[3]);
    _update = tmp;
  }

  inline virtual void setVal(const Vector4i &v,bool update=true) {
    _update = false;
    _spinx->setValue(v[0]);
    _spiny->setValue(v[1]);
    _spinz->setValue(v[2]);
    _spinw->setValue(v[3]);
    
    if(update && v!=val()) {
      updateGraph();
    }
    
    _update = true;
  } 

 signals:
  void valChanged();
  
  private slots:
  void valueChanged(int);

 private:
  QSpinBox *_spinx;
  QSpinBox *_spiny;
  QSpinBox *_spinz;
  QSpinBox *_spinw;
};


// *** TrackballCameraWidget ***
class LIBRARY_EXPORT TrackballCameraWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  TrackballCameraWidget(NodeInterface *node, const QString &name,
			TrackballCamera *cam, bool keyframed=true);

  inline QString  name       () const {return _nameLabel->text();}
  inline TrackballCamera *val() const {return _cam;}
  inline bool     keyframed  () const {return !_kfButtons->isHidden();}
  
  inline void setVal(TrackballCamera *cam,bool update=true) {
    _update = false;
    _cam = cam;
    _projection->setCurrentIndex(_cam->projType());
    _rotation->setCurrentIndex(_cam->rotType());

    if(update)
      updateGraph();

    _update = true;
  }

  inline void computeNewFrameValues() {
    const float current = (float)AnimationWidget::instance()->currentFrame();
    //_changed = _kfq.frameChanged(current) || _kfs.frameChanged(current);
    
    if(_kfq.frameChanged(current) || _kfs.frameChanged(current)) {
      _cam->setShiftRotation(_kfs.get(),_kfq.get());
      _changed = true;
    }
  }
  
  inline bool currentValuesHaveChanged() {
    const bool tmp = _changed;
    _changed = false;
    return tmp;
  }

  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  private slots:
  void editClicked();
  void setClicked();
  void projectionChanged(int);
  void rotationChanged(int);
  void checkChangeCurve();

 protected:
  QLabel              *_nameLabel;
  KeyframeButtons     *_kfButtons;
  KeyframedQuaternionf _kfq;
  KeyframedVector3f    _kfs;
  bool                 _update;
  bool                 _changed;
  TrackballCamera     *_cam;
  QComboBox           *_projection;
  QComboBox           *_rotation; 
};


#endif // WIDGET_PARAMS
