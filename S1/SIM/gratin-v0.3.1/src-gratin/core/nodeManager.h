// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef NODE_MANAGER_H
#define NODE_MANAGER_H

#include "misc/extinclude.h"
#include "core/nodeHandle.h"
#include "core/nodeInterface.h"
#include "core/pbgraph.h"
#include "core/sceneLoader.h"
#include "core/sceneSaver.h"

#include <QString>
#include <vector>
#include <map>

class CustomNodeHandle;
class OrigNodeHandle;

class NodeManager {
  friend class CustomNodeHandle;
  friend class OrigNodeHandle;

public:
  ~NodeManager();

  // this class is a singleton
  static NodeManager *instance() { if(!_instance) _instance = new NodeManager(); return _instance; } 

  const std::vector<NodeHandleInterface *> nodeHandles() const;
  NodeInterface *createInstance(PbGraph *parent,const QString &stringId,unsigned int version) const;
  NodeInterface *createInstance(PbGraph *parent,const QString &name,const QString &path) const;
  NodeInterface *createInstance(PbGraph *parent,
				const QString &name,
				const QString &path,
				const QString &desc,
				const QString &help,
				const QStringList &inputNames,
				const QStringList &outputNames) const;

  NodeHandleInterface *createCustomNode(NodeInterface *node,const QString &stringId,unsigned int version,
					const QString &name,const QString &path,
					const QString &desc,const QString &help,
					const QStringList &inputNames,const QStringList &outputNames,
					const QString &dir=GRATIN_DEFAULT_CUSTOM_NODE_PATH);

  void saveNode(SceneSaver  *saver ,NodeInterface *node,PbGraph *parent,bool &ret,unsigned int id=0);
  void saveNodeData(SceneSaver  *saver,NodeInterface *node,bool &ret,unsigned int id=0);
  NodeInterface *loadNode(SceneLoader *loader,PbGraph *parent,bool &ret,unsigned int id=0);
  void loadNodeData(SceneLoader *loader,NodeInterface *node,bool &ret,unsigned int id=0);
  void loadCustomNodeData(NodeInterface *node);
  NodeInterface *loadGroup(CustomNodeHandle *handle,PbGraph *parent,bool &ret);

  bool isCoreNode(const QString &name,const QString &path) const;
  bool isCustomNode(const QString &name,const QString &path) const;
  bool isGroupedNode(const QString &name,const QString &path) const;
  bool isGroupedNode(NodeInterface *node);

  NodeHandleInterface *handle(const QString &name,const QString &path);
  NodeHandleInterface *handle(const QString &stringId,unsigned int version);

  const QStringList customNodePaths();
  const QStringList libNodePaths();
  const QStringList nodePaths();

  void setNodePaths(const QStringList &nodePaths);



private:
  NodeManager();
  SceneLoader *initNodeLoading(const QString &filename);
  const QString coreStringId(const QString &name,const QString &path,NodeInterface *node=NULL);
  unsigned int  coreVersion(const QString &name,const QString &path,NodeInterface *node=NULL);

  void loadCoreHandles();
  void cleanCoreHandles();
  void loadCustomHandles();
  void cleanCustomHandles();
  void loadNodePaths();

  bool conflict(const QString &name,const QString &path);
  bool conflict(const QString &stringId,unsigned int version);

  void loadCustomNode(CustomNodeHandle *handle,NodeInterface *node,
		      const QString &name,
		      const QString &path,
		      const QString &desc,
		      const QString &help,
		      const QStringList &inputNames,
		      const QStringList &outputNames);
  bool saveCustomNode(CustomNodeHandle *handle,NodeInterface *node);


  inline QString nodeStringIdVersion(const QString &stringId,unsigned int version) const { 
    return stringId+QString::number(version); 
  }

  inline QString nodePathName(const QString &path,const QString &name) const { 
    return path+"/"+name;
  }
  
  std::map<QString,NodeHandleInterface* > _coreHandles;   // stringIdVersion to NodeHandleInterface
  std::map<QString,CustomNodeHandle*    > _customHandles; // stringIdVersion to CustomNodeHandle
  std::map<QString,CustomNodeHandle*    > _customGroupHandles; // path+name to CustomNodeHandle
  std::map<QString,QString>               _idConvert;     // from path+name to stringIdVersion  
  
  QStringList _nodePaths;

  static NodeManager *_instance;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

// this class is used to replace the initial handle in case the user creates a custom node 
class CustomNodeHandle : public NodeHandleInterface {

 public:
  CustomNodeHandle(const QString &filename,
		   const QString &coreStringId,
		   unsigned int coreVersion,
		   const QString &hstringId,
		   unsigned int hversion,
		   const QString &hname,
		   const QString &hpath,
		   const QString &hdesc,
		   const QString &hhelp,
		   const QStringList &hinputNames,
		   const QStringList &houtputNames)
    : _filename(filename),
    _coreStringId(coreStringId),
    _coreVersion(coreVersion),
    _stringId(hstringId),
    _version(hversion),
    _name(hname),
    _path(hpath),
    _desc(hdesc),
    _help(hhelp),
    _inputNames(hinputNames),
    _outputNames(houtputNames) {}

  inline void setFilename    (const QString     &filename    ) {_filename=filename;         }
  inline void setCoreStringId(const QString     &coreStringId) {_coreStringId=coreStringId; }
  inline void setStringId    (const QString     &hstringId   ) {_stringId=hstringId;        }
  inline void setName        (const QString     &hname       ) {_name=hname;                }
  inline void setPath        (const QString     &hpath       ) {_path=hpath;                }
  inline void setDesc        (const QString     &hdesc       ) {_desc=hdesc;                }
  inline void setHelp        (const QString     &hhelp       ) {_help=hhelp;                }
  inline void setInputNames  (const QStringList &hinputNames ) {_inputNames  = hinputNames; }
  inline void setOutputNames (const QStringList &houtputNames) {_outputNames = houtputNames;}
  inline void setVersion     (unsigned int hversion          ) {_version = hversion;        }
  inline void setCoreVersion (unsigned int coreVersion       ) {_coreVersion = coreVersion; }

  inline const QString     filename    () const {return _filename;    }
  inline const QString     stringId    () const {return _stringId;    }
  inline const QString     coreStringId() const {return _coreStringId;}
  inline const QString     name        () const {return _name;        }
  inline const QString     path        () const {return _path;        }
  inline const QString     desc        () const {return _desc;        }
  inline const QString     help        () const {return _help;        }
  inline const QStringList inputNames  () const {return _inputNames;  }
  inline const QStringList outputNames () const {return _outputNames; }
  inline unsigned int version          () const {return _version;     }
  inline unsigned int coreVersion      () const {return _coreVersion; }

  inline NodeInterface *createInstance(PbGraph *parent) {
    return createInstance(parent,_name,_path,_desc,_help,_inputNames,_outputNames);
  }

  inline NodeInterface *createInstance(PbGraph *parent,
				       const QString &name,
				       const QString &path,
				       const QString &desc,
				       const QString &help,
				       const QStringList &inputNames,
				       const QStringList &outputNames) {
    NodeManager *nm = NodeManager::instance();
    NodeInterface *node = NULL;

    if(nm->isGroupedNode(name,path)) {
      bool ret = true;
      node = nm->loadGroup(this,parent,ret);
    } else {
      node = nm->createInstance(parent,_coreStringId,_coreVersion);
    }

    if(!node) return NULL;
    node->setStringId(_stringId);
    node->setVersion(_version);
    nm->loadCustomNode(this,node,name,path,desc,help,inputNames,outputNames);
    return node;
  }

 private:
  QString      _filename;
  QString      _coreStringId;
  unsigned int _coreVersion;
  QString      _stringId;
  unsigned int _version;
  QString      _name;
  QString      _path;
  QString      _desc;
  QString      _help;
  QStringList  _inputNames;
  QStringList  _outputNames;

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // NODE_MANAGER_H
