// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_HANDLE_H
#define NODE_HANDLE_H

#include <QtPlugin>
#include <QString>
#include <QStringList>

#include "misc/extinclude.h"
#include "core/nodeInterface.h"
#include "core/pbgraph.h"

// class that has to declared in each plugin 
class LIBRARY_EXPORT NodeHandleInterface {
 public:
  virtual ~NodeHandleInterface() {}

  virtual const QString     stringId()    const = 0; // string that serves as id for this node (no spaces please)
  virtual unsigned int version()          const = 0; // node version

  virtual const QString     name()        const = 0; // node display name (no spaces please)
  virtual const QString     path()        const = 0; // node display path (no spaces please)
  virtual const QString     desc()        const = 0; // node description
  virtual const QString     help()        const = 0; // node help
  virtual const QStringList inputNames()  const = 0; // name of each input data
  virtual const QStringList outputNames() const = 0; // name of each output data

  // creates a new instance of this node (default)
  virtual NodeInterface *createInstance(PbGraph *parent)=0;

  // given handle values (typically loaded from file)
  virtual NodeInterface *createInstance(PbGraph *parent,
					const QString &name,
					const QString &path,
					const QString &desc,
					const QString &help,
					const QStringList &inputNames,
					const QStringList &outputNames)=0;
};

// class that has to be passed to each node ()
class LIBRARY_EXPORT NodeHandle {
 public:
 NodeHandle(NodeHandleInterface *handle) 
   : _stringId(handle->stringId()),
    _version(handle->version()),
    _name(handle->name()),
    _path(handle->path()),
    _desc(handle->desc()),
    _help(handle->help()),
    _inputNames(handle->inputNames()),
    _outputNames(handle->outputNames()),
    _nbInputs((unsigned int)_inputNames.size()),
    _nbOutputs((unsigned int)_outputNames.size()),
    _stringIdVersion(_stringId+QString::number(_version)) {
  }

 NodeHandle(const QString &stringIdp,
	    unsigned int versionp,
	    const QString &namep,
	    const QString &pathp,
	    const QString &descp,
	    const QString &helpp,
	    const QStringList &inputNamesp,
	    const QStringList &outputNamesp) 
   : _stringId(stringIdp),
    _version(versionp),
    _name(namep),
    _path(pathp),
    _desc(descp),
    _help(helpp),
    _inputNames(inputNamesp),
    _outputNames(outputNamesp),
    _nbInputs((unsigned int)_inputNames.size()),
    _nbOutputs((unsigned int)_outputNames.size()),
    _stringIdVersion(_stringId+QString::number(_version)) {
  }

  virtual ~NodeHandle() {}
  
  inline const QString     stringId   ()  const {return _stringId;   }
  inline unsigned int      version    ()  const {return _version;    }
  inline const QString     name       ()  const {return _name;       }
  inline const QString     path       ()  const {return _path;       }
  inline const QString     desc       ()  const {return _desc;       }
  inline const QString     help       ()  const {return _help;       }
  inline const QStringList inputNames ()  const {return _inputNames; }
  inline const QStringList outputNames()  const {return _outputNames;}

  inline void setStringId(const QString &s) {_stringId = s;}
  inline void setVersion(unsigned int v) {_version = v;}
  inline void setName(const QString &n) {_name = n;}
  inline void setPath(const QString &p) {_path = p;}
  inline void setDesc(const QString &d) {_desc = d;}
  inline void setHelp(const QString &h) {_help = h;}
  inline void setInputNames (const QStringList &in) {_inputNames = in ; _nbInputs = _inputNames.size()  ;}
  inline void setOutputNames(const QStringList &on) {_outputNames = on; _nbOutputs = _outputNames.size();}

  inline unsigned int nbInputs        ()  const {return _nbInputs;}
  inline unsigned int nbOutputs       ()  const {return _nbOutputs;}
  inline const QString stringIdVersion()  const {return _stringIdVersion;}

 private:
  QString      _stringId;
  unsigned int _version;
  QString      _name;
  QString      _path;
  QString      _desc;
  QString      _help;
  QStringList  _inputNames;
  QStringList  _outputNames;

  unsigned int _nbInputs;   // inputNames size 
  unsigned int _nbOutputs;  // outputNames size 
  QString _stringIdVersion; // concatenation of stringId + version
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(NodeHandleInterface,"Gratin.NodeHandle/1.0")
QT_END_NAMESPACE



#endif // NODE_HANDLE_H
