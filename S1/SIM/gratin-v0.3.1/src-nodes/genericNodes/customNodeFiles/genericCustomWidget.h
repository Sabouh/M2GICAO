// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef GENERIC_CUSTOM_WIDGET
#define GENERIC_CUSTOM_WIDGET

#include "genericSettings.h"
#include "codeEditor.h"
#include "misc/extinclude.h"
#include "view/nodeWidget.h"
#include "view/widgetParams.h"
#include "core/nodeTexture2D.h"

class GenericCustomNode;

class GenericCustomWidget : public NodeWidget {
  Q_OBJECT
    
    public:
  GenericCustomWidget(GenericCustomNode *node);
  
  const std::vector<QString> inputNames();
  const std::vector<QString> outputNames();
  const QStringList getUniformNames();
  QString generateUniformsHead();
  QString generateOutLocationsHead();

  inline std::vector<FloatAttribWidget    *> floatWidgets   () {return _floatWidgets;}
  inline std::vector<IntAttribWidget      *> intWidgets     () {return _intWidgets;}
  inline std::vector<Vector2fAttribWidget *> vector2fWidgets() {return _vector2fWidgets;}
  inline std::vector<Vector2iAttribWidget *> vector2iWidgets() {return _vector2iWidgets;}
  inline std::vector<Vector3fAttribWidget *> vector3fWidgets() {return _vector3fWidgets;}
  inline std::vector<Vector3iAttribWidget *> vector3iWidgets() {return _vector3iWidgets;}
  inline std::vector<Vector4fAttribWidget *> vector4fWidgets() {return _vector4fWidgets;}
  inline std::vector<Vector4iAttribWidget *> vector4iWidgets() {return _vector4iWidgets;}
  inline std::vector<std::pair<int,int> >    mapWidgets     () {return _mapWidgets;}

  //virtual void addUniformsToProg(GPUProgram *p);
  void setLogFromProg(GPUProgram *p);
  virtual void sendUniformsToProg(GPUProgram *p);

  bool nodePartiallyConnected() const;
  inline bool useMouse() const {return _mouseWidget;}
  inline bool useKeyboard() const {return _keyboardWidget;}

  Vector2f tsize() const;
  Vector2f toffset() const;

  // specific for save/created widgets (called by the node itself, before widgets have been created)
  bool saveWidgets(SceneSaver  *saver );
  bool loadWidgets(SceneLoader *loader);

  // called by nodeWidget after widgets have been created 
  bool loadUserDefinedWidgets(SceneLoader *);

  // mouse and keyboard
  inline void setMousePosition(const Vector2f &val);
  inline void setKeyboardValue(int val);

  // layout containing the user defined parameters
  inline QVBoxLayout *userLayout() {return _userLayout;}
  inline QString glslVersion() const { return _glslVersion;   }
  inline QString newName    () const { return _tmpName;       }
  inline bool useVertex     () const { return _useVertex;     }
  inline bool useTesselation() const { return _useTesselation;}
  inline bool useGeometry   () const { return _useGeometry;   }
  inline bool useFragment   () const { return _useFragment;   }
  inline GLint filterMode   () const { return _outFilter;     }
  inline GLint wrapMode     () const { return _outWrap;       }
  inline bool useDepthTest  () const { return _useDepthTest;  }
  inline bool useBlending   () const { return _useBlending;   }
  inline GLenum depthFunc   () const { return _depthFunc;     }
  inline GLenum blendSrc    () const { return _blendSrc;      }
  inline GLenum blendDst    () const { return _blendDst;      }
  inline Vector4f background() const { return _background;    }

  void setVertSource(const QString &head,const QString &body);
  void setTessCSource(const QString &head,const QString &body);
  void setTessESource(const QString &head,const QString &body);
  void setGeomSource(const QString &head,const QString &body);
  void setFragSource(const QString &head,const QString &body);
  
  virtual void updateWidget();
 
 protected:
  void cleanLayout();
  void initLayout();
  void cleanWidgets();
  void initWidgets(const GenericCustomWidgetSetting &settings);
  void initFromSettings(const GenericCustomWidgetSetting &settings);

  GenericCustomNode *_customNode;
  bool _useVertex;
  bool _useTesselation;
  bool _useGeometry;
  bool _useFragment;

  std::vector<FloatAttribWidget    *> _floatWidgets;
  std::vector<IntAttribWidget      *> _intWidgets;
  std::vector<Vector2fAttribWidget *> _vector2fWidgets;
  std::vector<Vector2iAttribWidget *> _vector2iWidgets;
  std::vector<Vector3fAttribWidget *> _vector3fWidgets;
  std::vector<Vector3iAttribWidget *> _vector3iWidgets;
  std::vector<Vector4fAttribWidget *> _vector4fWidgets;
  std::vector<Vector4iAttribWidget *> _vector4iWidgets;

  std::vector<std::pair<int,int> > _mapWidgets; // remember widgets ordering pair<type,index>

  QString  _glslVersion;
  QString  _tmpName;
  GLint    _outFilter;
  GLint    _outWrap;
  bool     _useDepthTest;
  bool     _useBlending;
  GLenum   _depthFunc;
  GLenum   _blendSrc;
  GLenum   _blendDst;
  Vector4f _background;

  QVBoxLayout        *_userLayout;
  Vector2iSpinWidget *_mouseWidget;
  IntSpinWidget      *_keyboardWidget;

  public slots:
  void settingsClicked();
  void applyClicked(bool update=true);

 protected:
  QLabel      *_vertHead;
  CodeEditor  *_vertSource;
  QLabel      *_tessCHead;
  CodeEditor  *_tessCSource;
  QLabel      *_tessEHead;
  CodeEditor  *_tessESource;
  QLabel      *_geomHead;
  CodeEditor  *_geomSource;
  QLabel      *_fragHead;
  CodeEditor  *_fragSource;
  QLabel      *_log;
  QPushButton *_settings;
  QPushButton *_apply;

  QScrollArea *_userArea;
  QScrollArea *_vertArea;
  QScrollArea *_tessCArea;
  QScrollArea *_tessEArea;
  QScrollArea *_geomArea;
  QScrollArea *_fragArea;
  QScrollArea *_logArea;

  QTabWidget  *_tabWidget;
};

inline void GenericCustomWidget::setMousePosition(const Vector2f &val) {
  if(useMouse()) {
    _mouseWidget->setVal(Vector2i((int)val[0],(int)val[1]));
  }
}

inline void GenericCustomWidget::setKeyboardValue(int val) {
  if(useKeyboard()) {
    _keyboardWidget->setVal(val);
  }
}




#endif // GENERIC_CUSTOM_WIDGET
