// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GENERIC_SETTINGS
#define GENERIC_SETTINGS

#include <QWidget>
#include <QDialog>
#include <QToolButton>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "misc/extinclude.h"
#include "misc/glcontext.h"

class GenericCustomWidget;

class GenericCustomIOItemButtons : public QWidget {
Q_OBJECT

  public:
  GenericCustomIOItemButtons();

  inline QToolButton *up()   const {return _up;}
  inline QToolButton *down() const {return _down;}
  inline QToolButton *del()  const {return _del;}

  private:
  QToolButton *_up;
  QToolButton *_down;
  QToolButton *_del;
};


class GenericCustomIOItem : public QWidget {
Q_OBJECT
  
  public:
  GenericCustomIOItem(unsigned int id=0,const QString &name=QString());

  inline void setId(unsigned int id) {_id=id;_number->setText(QString::number(_id));} 
  inline unsigned int id() const {return _id;}
  inline const QString name() const {return _name->text();}
  inline void setName(const QString &name) {_name->setText(name);}

 signals:
  void upClicked(GenericCustomIOItem *);
  void downClicked(GenericCustomIOItem *);
  void delClicked(GenericCustomIOItem *);

  private slots:
  void needUpSignal();
  void needDownSignal();
  void needDelSignal();

 private:
  unsigned int            _id;
  QLabel                 *_number;
  QLineEdit              *_name;
  GenericCustomIOItemButtons *_buttons;
};


class GenericCustomParamItem : public QWidget {
Q_OBJECT
  
  public:
  GenericCustomParamItem(unsigned int id=0,const QString &name=QString(),int type=0,int view=0,bool keyframed=false,
			 float minVal=0.0f,float maxVal=1.0f,float defaultVal=0.0f);

  inline void setId(unsigned int id) {_id=id;} 
  inline void setName(const QString &name) {_name->setText(name);}

  inline unsigned int  id  () const {return _id;                           }
  inline const QString name() const {return _name->text();                 }
  inline bool  keyframed   () const {return _kf->checkState()==Qt::Checked;}
  inline int   type        () const {return _type->currentIndex();         }
  inline int   view        () const {return _view->currentIndex();         }
  inline float minVal      () const {return _minEdit->text().toFloat();    }
  inline float maxVal      () const {return _maxEdit->text().toFloat();    }
  inline float val         () const {return _valEdit->text().toFloat();    }

 signals:
  void upClicked(GenericCustomParamItem *);
  void downClicked(GenericCustomParamItem *);
  void delClicked(GenericCustomParamItem *);

  private slots:
  void needUpSignal();
  void needDownSignal();
  void needDelSignal();
  void typeChanged(int);


 private:
  unsigned int  _id;
  QLineEdit    *_name;
  QLineEdit    *_minEdit;
  QLineEdit    *_maxEdit;
  QLineEdit    *_valEdit;
  QLabel       *_minLabel;
  QLabel       *_maxLabel;
  QLabel       *_valLabel;
  QComboBox    *_type;
  QComboBox    *_view;
  QCheckBox    *_kf;
  GenericCustomIOItemButtons *_buttons;
};

class IOList : public QGroupBox {
  Q_OBJECT

    public:
  IOList(const QString &title,const std::vector<QString> &names,const QString &prefix,bool nodeConnected);

  inline std::vector<GenericCustomIOItem *> items() const {return _items;}

  private slots:
  void itemUp(GenericCustomIOItem *);
  void itemDown(GenericCustomIOItem *);
  void itemDel(GenericCustomIOItem *);
  void addClicked();

 private:
  void buildLayout();
  void cleanLayout();
  void initItems();
  void cleanItems();

  std::vector<QString> _names;
  QString              _prefix;
  bool                 _nodeConnected;
  QPushButton         *_add;
  QVBoxLayout         *_layout;
  std::vector<GenericCustomIOItem *> _items;
};

class ParamList : public QGroupBox {
  Q_OBJECT

    public:
  ParamList(const QString &title,GenericCustomWidget *nodeWidget);

  inline std::vector<GenericCustomParamItem *> items() const {return _items;}

  private slots:
  void itemUp(GenericCustomParamItem *);
  void itemDown(GenericCustomParamItem *);
  void itemDel(GenericCustomParamItem *);
  void addClicked();

 private:
  void buildLayout();
  void cleanLayout();
  void initItems();
  void cleanItems();

  GenericCustomWidget *_nodeWidget;
  QPushButton         *_add;
  QVBoxLayout         *_layout;
  QString              _prefix;
  std::vector<GenericCustomParamItem *> _items;
};

class PropertyList : public QGroupBox {
  Q_OBJECT
    
    public:
  PropertyList(const QString &title,GenericCustomWidget *nodeWidget);
  const Vector2f tsize(bool *ok=0) const;
  const Vector2f toffset(bool *ok=0) const;
  GLint filterMode();
  GLint wrapMode();
  GLenum depthFunc();
  GLenum blendSrc();
  GLenum blendDst();

  QString name()         const {return _name->text().remove(QChar(' '));}
  bool needMouse()       const {return _mouse->checkState()==Qt::Checked;}
  bool needKeyboard()    const {return _keyboard->checkState()==Qt::Checked;}
  bool needVertex()      const {return _vertex->checkState()==Qt::Checked;}
  bool needTesselation() const {return _tesselation->checkState()==Qt::Checked;}
  bool needGeometry()    const {return _geometry->checkState()==Qt::Checked;}
  bool needFragment()    const {return _fragment->checkState()==Qt::Checked;}
  bool needDepthTest()   const {return _depth->checkState()==Qt::Checked;}
  bool needBlending()    const {return _blend->checkState()==Qt::Checked;}
  QString glslVersion()  const {return _glslVersion->text();}
  Vector4f background()  const {return _color;}

  public slots:
  void colorClicked();

 private:
  int indexFromFilterMode(GLint mode);
  int indexFromWrapMode(GLint mode);
  int indexFromDepthFunc(GLenum func);
  int indexFromBlendMode(GLenum mode);

  inline void setButtonColor(const Vector4f &color) {
    int r = (int)(color[0]*255.0f);
    int g = (int)(color[1]*255.0f);
    int b = (int)(color[2]*255.0f);
    const QString c = "("+QString::number(r)+","+QString::number(g)+","+QString::number(b)+")";
    _background->setStyleSheet("background-color: rgb"+c+"; color: rgb(255, 255, 255)");
  }

  GenericCustomWidget *_nodeWidget;
  QLineEdit       *_name;
  QLineEdit       *_xSize;
  QLineEdit       *_ySize;
  QLineEdit       *_xOffset;
  QLineEdit       *_yOffset;
  QComboBox       *_filter;
  QComboBox       *_wrap;
  QCheckBox       *_mouse;
  QCheckBox       *_keyboard;
  QCheckBox       *_vertex;
  QCheckBox       *_tesselation;
  QCheckBox       *_geometry;
  QCheckBox       *_fragment;
  QLineEdit       *_glslVersion;
  QCheckBox       *_depth;
  QCheckBox       *_blend;
  QComboBox       *_depthFunc;
  QComboBox       *_blendSrc;
  QComboBox       *_blendDst;
  QPushButton     *_background;
  Vector4f         _color;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


class GenericCustomWidgetSetting : public QDialog {
  Q_OBJECT
    
    public:
  GenericCustomWidgetSetting(GenericCustomWidget *nodeWidget);
  ~GenericCustomWidgetSetting();

  std::vector<GenericCustomIOItem *> inputItems() const;
  std::vector<GenericCustomIOItem *> outputItems() const;
  std::vector<GenericCustomParamItem *> paramItems() const;
  const Vector2f tsize  (bool *ok=0) const { return _propertyBox->tsize(ok);   }
  const Vector2f toffset(bool *ok=0) const { return _propertyBox->toffset(ok); }
  bool useMouse()       const { return _propertyBox->needMouse();    }
  bool useKeyboard()    const { return _propertyBox->needKeyboard(); }
  bool useVertex()      const { return _propertyBox->needVertex();}
  bool useTesselation() const { return _propertyBox->needTesselation();}
  bool useGeometry()    const { return _propertyBox->needGeometry();}
  bool useFragment()    const { return _propertyBox->needFragment();}
  GLint outFilter()     const { return _propertyBox->filterMode();}
  GLint outWrap()       const { return _propertyBox->wrapMode();}
  QString glslVersion() const { return _propertyBox->glslVersion();}
  QString name()        const { return _propertyBox->name();}
  bool useDepthTest()   const { return _propertyBox->needDepthTest();    }
  bool useBlending()    const { return _propertyBox->needBlending();    }
  GLenum depthFunc()    const { return _propertyBox->depthFunc();}
  GLenum blendSrc()     const { return _propertyBox->blendSrc();}
  GLenum blendDst()     const { return _propertyBox->blendDst();}
  Vector4f background() const { return _propertyBox->background();}

  private slots:
  void confirmClicked();
  void cancelClicked();

  protected:
  GenericCustomWidget *_nodeWidget;

  bool checkParams();

  // boxes 
  PropertyList *_propertyBox;
  IOList       *_inputBox;
  IOList       *_outputBox;
  ParamList    *_paramBox;

  // title
  QLabel *_title;
  
  // buttons
  QDialogButtonBox *_buttons;
};

#endif // GENERIC_SETTINGS
