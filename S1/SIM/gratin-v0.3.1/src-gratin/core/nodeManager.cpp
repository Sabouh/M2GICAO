// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/nodeManager.h"
#include "core/nodeGrouped.h"

#include <iostream>
#include <QPluginLoader>
#include <QObject>
#include <QDir>
#include <QFileInfo>

using namespace std;

NodeManager  *NodeManager::_instance = NULL;

NodeManager::NodeManager()
  : _coreHandles(map<QString,NodeHandleInterface *>()),
    _customHandles(map<QString,CustomNodeHandle *> ()),
    _customGroupHandles(map<QString,CustomNodeHandle *> ()),
    _idConvert(map<QString,QString>()) {
  loadNodePaths();
  loadCoreHandles();
  loadCustomHandles();
}

NodeManager::~NodeManager() {
  cleanCoreHandles();
  cleanCustomHandles();
}

const QStringList NodeManager::customNodePaths() {
  return QStringList() << GRATIN_DEFAULT_CUSTOM_NODE_PATH << _nodePaths;
}

const QStringList NodeManager::libNodePaths() {
  return QStringList() << GRATIN_DEFAULT_LIB_NODE_PATH << _nodePaths;
}

const QStringList NodeManager::nodePaths() {
  return _nodePaths;
}

void NodeManager::loadNodePaths() {
  _nodePaths.clear();

  QFile file(GRATIN_NODE_PATHS_FILE);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  
  QTextStream in(&file);
  
  while(!in.atEnd()) {
    QString line = in.readLine();    
    if(!line.isEmpty()) {
      _nodePaths.push_back(line);
    }
  }
  
  file.close();
}


void NodeManager::setNodePaths(const QStringList &nodePaths) {
  QFile file(GRATIN_NODE_PATHS_FILE);

  if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    return;
  }
  
  QTextStream out(&file);
  
  foreach(QString path,nodePaths) {
    out << path << "\n";
  }

  file.close();

  _nodePaths = nodePaths;
}

bool NodeManager::conflict(const QString &name,const QString &path) {
  map<QString,NodeHandleInterface *>::const_iterator mit1(_coreHandles.begin()),mend1(_coreHandles.end());
  map<QString,CustomNodeHandle    *>::const_iterator mit2(_customHandles.begin()),mend2(_customHandles.end());
  map<QString,CustomNodeHandle    *>::const_iterator mit3(_customGroupHandles.begin()),mend3(_customGroupHandles.end());

  for(;mit1!=mend1;++mit1) { 
    NodeHandleInterface *h = mit1->second;
    if(h->name()==name && h->path()==path)
      return true;
  }

  for(;mit2!=mend2;++mit2) { 
    CustomNodeHandle *h = mit2->second;
    if(h->name()==name && h->path()==path)
      return true;
  }

  for(;mit3!=mend3;++mit3) { 
    CustomNodeHandle *h = mit3->second;
    if(h->name()==name && h->path()==path)
      return true;
  }

  return false;
}


bool NodeManager::conflict(const QString &stringId,unsigned int version) {
  map<QString,NodeHandleInterface *>::const_iterator mit1(_coreHandles.begin()),mend1(_coreHandles.end());
  map<QString,CustomNodeHandle    *>::const_iterator mit2(_customHandles.begin()),mend2(_customHandles.end());
  
  for(;mit1!=mend1;++mit1) { 
    NodeHandleInterface *h = mit1->second;
    if(h->stringId()==stringId && h->version()==version)
      return true;
  }

  for(;mit2!=mend2;++mit2) { 
    CustomNodeHandle *h = mit2->second;
    if(h->stringId()==stringId && h->version()==version)
      return true;
  }

  return false;
}

void NodeManager::loadCoreHandles() {
  const QStringList libPaths = libNodePaths();
  foreach(QString d,libPaths) {
    QDir pluginsDir(d);
    
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {

      QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
      QObject *plugin = loader.instance();
      if(plugin) {
	NodeHandleInterface *handle = qobject_cast<NodeHandleInterface *>(plugin);
	if(handle) {
	  if(conflict(handle->path(),handle->name())) {
	    cout << "Node conflict - path: " << handle->path().toStdString() << ", name: " << handle->name().toStdString() << " -- ignoring..." << endl;
	  } else if(conflict(handle->stringId(),handle->version())) {
	    cout << "Node conflict - ID: " << handle->stringId().toStdString() << ", version: " << handle->version() << " -- ignoring..." << endl;
	  } else {
	    _coreHandles[nodeStringIdVersion(handle->stringId(),handle->version())] = handle;
	    _idConvert[nodePathName(handle->path(),handle->name())] = nodeStringIdVersion(handle->stringId(),handle->version());
	  }
	} else {
	  cout << "Unable to load " << fileName.toStdString() << endl;
	}
      }
    }
  }
}

void NodeManager::loadCustomHandles() {
  const QStringList customPaths = customNodePaths();
  foreach(QString d,customPaths) {

    QDir customsDir(d);
    foreach(QString fileName, customsDir.entryList(QDir::Files)) {
      QString absfilename = customsDir.absoluteFilePath(fileName);
      SceneLoader *loader = initNodeLoading(absfilename);
      
      if(!loader) {
	continue;
      }
      
      // loading only handle      
      QString coreStringId,cstringId,cname,cpath,cdesc,chelp;
      QStringList cinputName,coutputName;
      unsigned int cversion,coreVersion;
      int cinputNameSize=0,coutputNameSize=0;

      if(!loader->loadString("coreHandleStringId",coreStringId) ||
	 !loader->loadUint("coreHandleVersion",coreVersion) ||
	 !loader->loadString(Tags::attribId("nodeStringId",0),cstringId) ||
	 !loader->loadUint(Tags::attribId("nodeVersion",0),cversion) ||
	 !loader->loadString(Tags::attribId("nodeName",0),cname) ||
	 !loader->loadString(Tags::attribId("nodePath",0),cpath)) {
	delete loader;
	continue;
      }

      if(conflict(cpath,cname)) {
	cout << "Node conflict - path: " << cpath.toStdString() << ", name: " << cname.toStdString() << " -- ignoring..." << endl;
	delete loader;
	continue;
      } else if(conflict(cstringId,cversion)) {
	cout << "Node conflict - ID: " << cstringId.toStdString() << ", version: " << cversion << " -- ignoring..." << endl;
	delete loader;
	continue;
      } 

      loader->loadString(Tags::attribId("nodeDesc",0),cdesc);
      loader->loadString(Tags::attribId("nodeHelp",0),chelp);
      loader->loadInt(Tags::attribId("nodeInputNameSize",0),cinputNameSize);
      loader->loadInt(Tags::attribId("nodeOutputNameSize",0),coutputNameSize);
      
      for(int i=0;i<cinputNameSize;++i) {
	QString currentName;
	loader->loadString(Tags::attribId("nodeInputName",0,i),currentName);
	cinputName.push_back(currentName);
      }
      
      for(int i=0;i<coutputNameSize;++i) {
	QString currentName;
	loader->loadString(Tags::attribId("nodeOutputName",0,i),currentName);
	coutputName.push_back(currentName);
      }
      
      CustomNodeHandle *chandle = new CustomNodeHandle(absfilename,coreStringId,coreVersion,cstringId,cversion,
						       cname,cpath,cdesc,chelp,cinputName,coutputName);

      if(coreStringId==NodeGrouped::GROUPED_NODE_ID) {
	_customGroupHandles[nodePathName(chandle->path(),chandle->name())] = chandle;
      } else {
	_customHandles[nodeStringIdVersion(chandle->stringId(),chandle->version())] = chandle;
      }

      _idConvert[nodePathName(chandle->path(),chandle->name())] = nodeStringIdVersion(chandle->stringId(),chandle->version());
      delete loader; loader = NULL;
    }
  }
}

void NodeManager::cleanCoreHandles() {
  map<QString,NodeHandleInterface *>::const_iterator mit(_coreHandles.begin()),mend(_coreHandles.end());
  
  for(;mit!=mend;++mit) { delete mit->second; }

  _coreHandles.clear();
}

void NodeManager::cleanCustomHandles() {
  map<QString,CustomNodeHandle *>::const_iterator mit1(_customHandles.begin()),mend1(_customHandles.end());
  map<QString,CustomNodeHandle *>::const_iterator mit2(_customGroupHandles.begin()),mend2(_customGroupHandles.end());

  for(;mit1!=mend1;++mit1) { delete mit1->second; }
  for(;mit2!=mend2;++mit2) { delete mit2->second; }

  _customHandles.clear();
  _customGroupHandles.clear();
}

SceneLoader *NodeManager::initNodeLoading(const QString &filename) {
  QFile file(filename);
  QFileInfo info(filename);

  if(!file.open(QFile::ReadOnly | QFile::Text)) return NULL;
  SceneLoader *loader = new SceneLoader();
  
  if(!loader->readFrom(file) || !loader->init("node",info.absolutePath())) {
    delete loader;
    return NULL;
  }

  return loader;
}


bool NodeManager::saveCustomNode(CustomNodeHandle *handle,NodeInterface *node) {
  QFile file(handle->filename());
  
  if(!file.open(QFile::WriteOnly | QFile::Text)) return false;

  QFileInfo info(handle->filename());
  SceneSaver *saver = new SceneSaver();

  if(!saver->init("node",info.absolutePath())) {
    delete saver;
    return false;
  }

  bool ret = true;

  if(!saver->saveString("coreHandleStringId",handle->coreStringId())) ret = false;
  if(!saver->saveUint("coreHandleVersion",handle->coreVersion())) ret = false;

  saveNode(saver,node,node->parentGraph(),ret);
  saveNodeData(saver,node,ret);

  saver->writeTo(file);

  delete saver;
  return ret;
}

// apartir de la
const vector<NodeHandleInterface *> NodeManager::nodeHandles() const {
  vector<NodeHandleInterface *> handles;

  map<QString,NodeHandleInterface *>::const_iterator mit1(_coreHandles.begin()),mend1(_coreHandles.end());
  for(;mit1!=mend1;++mit1) {
    handles.push_back(mit1->second);
  }

  map<QString,CustomNodeHandle *>::const_iterator mit2(_customHandles.begin()),mend2(_customHandles.end());
  for(;mit2!=mend2;++mit2) {
    handles.push_back(mit2->second);
  }

  map<QString,CustomNodeHandle *>::const_iterator mit3(_customGroupHandles.begin()),mend3(_customGroupHandles.end());
  for(;mit3!=mend3;++mit3) {
    handles.push_back(mit3->second);
  }

  return handles;
}

void NodeManager::saveNode(SceneSaver *saver,NodeInterface *node,PbGraph *,bool &ret,unsigned int id) {

  if(!saver->saveString  (Tags::attribId("nodeStringId" ,id),node->stringId() )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nodeVersion"  ,id),node->version()  )) ret = false;
  if(!saver->saveString  (Tags::attribId("nodeName"     ,id),node->name()     )) ret = false;
  if(!saver->saveString  (Tags::attribId("nodePath"     ,id),node->path()     )) ret = false;
  if(!saver->saveString  (Tags::attribId("nodeDesc"     ,id),node->desc()     )) ret = false;
  if(!saver->saveString  (Tags::attribId("nodeHelp"     ,id),node->help()     )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nodeId"       ,id),node->nodeId()   )) ret = false;
  if(!saver->saveUint    (Tags::attribId("nodeGraphId"  ,id),node->graphId()  )) ret = false;
  if(!saver->saveVector2f(Tags::attribId("nodePos"      ,id),node->pos()      )) ret = false;

  QStringList inputs  = node->inputNames();
  QStringList outputs = node->outputNames();

  if(!saver->saveInt(Tags::attribId("nodeInputNameSize",id),inputs.size()))    ret = false;
  if(!saver->saveInt(Tags::attribId("nodeOutputNameSize",id),outputs.size()))  ret = false;
  
  for(int i=0;i<inputs.size();++i) {
    if(!saver->saveString(Tags::attribId("nodeInputName",id,i),inputs[i]))  ret = false;
  }
  
  for(int i=0;i<outputs.size();++i) {
    if(!saver->saveString(Tags::attribId("nodeOutputName",id,i),outputs[i])) ret = false;
  }
}

void NodeManager::saveNodeData(SceneSaver *saver,NodeInterface *node,bool &ret,unsigned int id) {
  if(!saver->saveBool    (Tags::attribId("nodeHasWidget",id),node->widget())) ret = false;
  if(!saver->saveIOData  (Tags::attribId("node"         ,id),node          )) ret = false;
  if(node->widget()) 
    if(!saver->saveIOData(Tags::attribId("nodeWidget"   ,id),node->widget())) ret = false;


  for(unsigned int j=0;j<node->nbInputs();++j) {
    if(!saver->saveVector2f(Tags::attribId("nodeInputPos",id,j),node->inputPos(j))) 
      ret = false;
  }

  for(unsigned int j=0;j<node->nbOutputs();++j) {
    if(!saver->saveVector2f(Tags::attribId("nodeOutputPos",id,j),node->outputPos(j))) 
      ret = false;
  }
}

NodeInterface *NodeManager::loadGroup(CustomNodeHandle *handle,PbGraph *parent,bool &ret) {

  if(!handle || !isGroupedNode(handle->name(),handle->path())) {
    ret = false;
    return NULL;
  }

  SceneLoader *loader = initNodeLoading(handle->filename());
  if(!loader) {
    ret = false;
    return NULL;
  }

  return loadNode(loader,parent,ret);
}

NodeInterface *NodeManager::loadNode(SceneLoader *loader,PbGraph *parent,bool &ret,unsigned int id) {
  unsigned int nodeId=0,nodeGraphId=0,nodeVersion=1;
  QString      nodeStringId("noname"),nodeName("noname"),nodePath("nopath"),nodeDesc,nodeHelp;
  Vector2f     nodePos(0,0);
  int          inputNameSize=0,outputNameSize=0;
  QStringList  inputNames,outputNames;

  if(!loader->loadString  (Tags::attribId("nodeStringId" ,id),nodeStringId )) ret = false;
  if(!loader->loadUint    (Tags::attribId("nodeVersion"  ,id),nodeVersion  )) ret = false;
  if(!loader->loadString  (Tags::attribId("nodeName"     ,id),nodeName     )) ret = false;
  if(!loader->loadString  (Tags::attribId("nodePath"     ,id),nodePath     )) ret = false;
  if(!loader->loadString  (Tags::attribId("nodeDesc"     ,id),nodeDesc     )) ret = false;
  if(!loader->loadString  (Tags::attribId("nodeHelp"     ,id),nodeHelp     )) ret = false;
  if(!loader->loadUint    (Tags::attribId("nodeId"       ,id),nodeId       )) ret = false;
  if(!loader->loadUint    (Tags::attribId("nodeGraphId"  ,id),nodeGraphId  )) ret = false;
  if(!loader->loadVector2f(Tags::attribId("nodePos"      ,id),nodePos      )) ret = false;

  loader->loadInt(Tags::attribId("nodeInputNameSize",id),inputNameSize);
  loader->loadInt(Tags::attribId("nodeOutputNameSize",id),outputNameSize);

  for(int i=0;i<inputNameSize;++i) {
    QString currentName;
    loader->loadString(Tags::attribId("nodeInputName",id,i),currentName);
    inputNames.push_back(currentName);
  }

  for(int i=0;i<outputNameSize;++i) {
    QString currentName;
    loader->loadString(Tags::attribId("nodeOutputName",id,i),currentName);
    outputNames.push_back(currentName);
  }
  
  const bool nodeGrouped = isGroupedNode(nodeName,nodePath) || nodeStringId==NodeGrouped::GROUPED_NODE_ID;
  if(!nodeGrouped) {
    // the node is part of the library - direct loading
    if(nodeStringIdVersion(nodeStringId,nodeVersion)!=_idConvert[nodePathName(nodePath,nodeName)]) {
      NodeHandleInterface *h = handle(nodeStringId,nodeVersion);
      nodeName = h->name();
      nodePath = h->path();
    }

    NodeInterface *n = createInstance(parent,nodeName,nodePath,nodeDesc,nodeHelp,inputNames,outputNames);

    if(!n) {
      n = new DefaultNode(parent,nodeName,nodePath,inputNames,outputNames);
      ret = false;
    }
    n->setNodeId(nodeId);
    n->setGraphId(nodeGraphId);
    n->setPos(nodePos);
    return n;

  } else {
    // the node is grouped - load nodes inside the group
    NodeGrouped *g = new NodeGrouped(parent,nodeName,nodePath,nodeDesc,nodeHelp,inputNames,outputNames);

    g->setIoMode(NodeGrouped::IO_NODEGROUPED);
    if(!loader->loadIOData(Tags::attribId("node",id),(NodeInterface *)g)) 
      ret = false;

    g->setIoMode(NodeGrouped::IO_USER_DATA);
    g->setNodeId(nodeId);
    g->setGraphId(nodeGraphId);
    g->setPos(nodePos);

    return g;    
  } 

  return NULL;
}

void NodeManager::loadNodeData(SceneLoader *loader,NodeInterface *node,bool &ret,unsigned int id) {
  bool hasWidget = false;

  if(!loader->loadBool(Tags::attribId("nodeHasWidget",id),hasWidget))       ret = false;
  if(!loader->loadIOData(Tags::attribId("node",id),node))                   ret = false;
  if(hasWidget) {
    if(!loader->loadIOData(Tags::attribId("nodeWidget",id),node->widget())) ret = false;
  }
  for(unsigned int j=0;j<node->nbInputs();++j) {
    Vector2f p(0,0);
    if(!loader->loadVector2f(Tags::attribId("nodeInputPos",id,j),p)) ret = false;
    node->setInputPos(p,j);
  }

  for(unsigned int j=0;j<node->nbOutputs();++j) {
    Vector2f p(0,0);
    if(!loader->loadVector2f(Tags::attribId("nodeOutputPos",id,j),p)) ret = false;
    node->setOutputPos(p,j);
  }

  node->updatePositions();
}
 
void NodeManager::loadCustomNode(CustomNodeHandle *,NodeInterface *node,
				 const QString &name,
				 const QString &path,
				 const QString &desc,
				 const QString &help,
				 const QStringList &inputNames,
				 const QStringList &outputNames) {

  node->setName(name);
  node->setPath(path);
  node->setDesc(desc);
  node->setHelp(help);
  node->setInputNames(inputNames);
  node->setOutputNames(outputNames);
}

void NodeManager::loadCustomNodeData(NodeInterface *node) {
  if(!node) return; 

  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(node->path(),node->name()));
  if(n==_idConvert.end()) return;

  const QString stringIdVersion = n->second;

  map<QString,CustomNodeHandle *>::const_iterator i = _customHandles.find(stringIdVersion);
  if(i==_customHandles.end()) {
    i = _customGroupHandles.find(nodePathName(node->path(),node->name()));

    if(i==_customGroupHandles.end()) {
      return;
    }
  }

  CustomNodeHandle *handle = i->second;

  SceneLoader *loader = initNodeLoading(handle->filename());
  if(!loader) return;

  bool ret = true;
  loadNodeData(loader,node,ret);

  delete loader;
}

NodeInterface *NodeManager::createInstance(PbGraph *parent,const QString &stringId,unsigned int version) const {
  const QString stringIdVersion = nodeStringIdVersion(stringId,version);
  
  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(stringIdVersion);
  if(i!=_coreHandles.end()) return i->second->createInstance(parent);
  
  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(stringIdVersion);
  if(j!=_customHandles.end()) return j->second->createInstance(parent);
    
  // if(stringId==NodeGrouped::GROUPED_NODE_ID) {
  //   NodeGrouped *g = new NodeGrouped(parent);
  //   g->setIoMode(NodeGrouped::IO_NODEGROUPED);
  //   return g;
  // }

  return NULL;
}

NodeInterface *NodeManager::createInstance(PbGraph *parent,const QString &name,const QString &path) const {

  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(path,name));
  if(n==_idConvert.end()) return NULL;

  const QString stringIdVersion = n->second;

  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(stringIdVersion);
  if(i!=_coreHandles.end()) return i->second->createInstance(parent);
  
  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(stringIdVersion);
  if(j!=_customHandles.end()) return j->second->createInstance(parent);

  map<QString,CustomNodeHandle *>::const_iterator k = _customGroupHandles.find(nodePathName(path,name));
  //cout << __FILE__ << " - " << __FUNCTION__ << endl;
  if(k!=_customGroupHandles.end()) return k->second->createInstance(parent);
  //cout << __FILE__ << " - " << __FUNCTION__ << endl;

  return NULL;
}

NodeInterface *NodeManager::createInstance(PbGraph *parent,const QString &name,const QString &path,
					   const QString &desc,
					   const QString &help,
					   const QStringList &inputNames,
					   const QStringList &outputNames) const {
  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(path,name));
  if(n==_idConvert.end()) return NULL;

  const QString stringIdVersion = n->second;

  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(stringIdVersion);
  if(i!=_coreHandles.end()) return i->second->createInstance(parent,name,path,desc,help,inputNames,outputNames);
  
  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(stringIdVersion);
  if(j!=_customHandles.end()) return j->second->createInstance(parent,name,path,desc,help,inputNames,outputNames);
  
  map<QString,CustomNodeHandle *>::const_iterator k = _customGroupHandles.find(nodePathName(path,name));
  if(k!=_customGroupHandles.end()) return k->second->createInstance(parent,name,path,desc,help,inputNames,outputNames);
 
  return NULL;
}


NodeHandleInterface *NodeManager::createCustomNode(NodeInterface *node,const QString &stringId,unsigned int version,
						   const QString &name,const QString &path,
						   const QString &desc,const QString &help,
						   const QStringList &inputNames,const QStringList &outputNames,
						   const QString &dir) {
  
  const QString filename = dir+"/"+nodeStringIdVersion(stringId,version)+GRATIN_CUSTOM_EXT;
  const QString origStringId = isGroupedNode(node) ? node->stringId() : coreStringId(node->name(),node->path(),node);
  const unsigned int origVersion = isGroupedNode(node) ? node->version() : coreVersion(node->name(),node->path(),node);
  CustomNodeHandle *chandle  = NULL;
  const QString strId = isGroupedNode(node) ? NodeGrouped::GROUPED_NODE_ID : stringId;

  map<QString,CustomNodeHandle *>::const_iterator i;
  bool alreadyExist = false;

  // get/create the new node handle 
  i = _customHandles.find(nodeStringIdVersion(strId,version));
  if(i!=_customHandles.end()) {
    alreadyExist = true; 
  } else {
    i = _customGroupHandles.find(nodePathName(path,name));
    if(i!=_customGroupHandles.end())
      alreadyExist = true; 
  }

  if(alreadyExist) {
    // Check if we simply replace an existing handle 
    chandle = i->second;

    chandle->setFilename(filename);
    chandle->setCoreStringId(origStringId);
    chandle->setCoreVersion(origVersion);
    chandle->setStringId(strId);
    chandle->setVersion(version);
    chandle->setName(name);
    chandle->setPath(path);
    chandle->setDesc(desc);
    chandle->setHelp(help);
    chandle->setInputNames(inputNames);
    chandle->setOutputNames(outputNames);
    

  } else {
    // create a new handle 
    chandle = new CustomNodeHandle(filename,origStringId,origVersion,strId,version,
				   name,path,desc,help,inputNames,outputNames);
  } 
  
  // set handle 
  if(isGroupedNode(node)) {
    _customGroupHandles[nodePathName(chandle->path(),chandle->name())] = chandle;
  } else {
    _customHandles[nodeStringIdVersion(strId,version)] = chandle;
  }
  
  _idConvert[nodePathName(path,name)] = nodeStringIdVersion(strId,version);

  node->setStringId(strId);
  node->setVersion(version);
  node->setName(name);
  node->setPath(path);
  node->setDesc(desc);
  node->setHelp(help);
  node->setInputNames(inputNames);
  node->setOutputNames(outputNames);

  saveCustomNode(chandle,node);

  return chandle;
}

bool NodeManager::isCoreNode(const QString &name,const QString &path) const {
  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(path,name));
  if(n==_idConvert.end()) return false;
  const QString id = n->second;

  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(id);
  return i!=_coreHandles.end();
}

bool NodeManager::isCustomNode(const QString &name,const QString &path) const {
  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(path,name));
  if(n==_idConvert.end()) return false;
  const QString id = n->second;

  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(id);

  return j!=_customHandles.end();
}

bool NodeManager::isGroupedNode(const QString &name,const QString &path) const {
  map<QString,CustomNodeHandle *>::const_iterator k = _customGroupHandles.find(nodePathName(path,name));

  return k!=_customGroupHandles.end();
}

NodeHandleInterface *NodeManager::handle(const QString &name,const QString &path) {
  map<QString,QString>::const_iterator n = _idConvert.find(nodePathName(path,name));
  if(n==_idConvert.end()) return NULL;
  const QString id = n->second;

  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(id);
  if(i!=_coreHandles.end()) return i->second;
  
  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(id);
  if(j!=_customHandles.end()) return j->second;

  map<QString,CustomNodeHandle *>::const_iterator k = _customGroupHandles.find(nodePathName(path,name));
  if(k!=_customGroupHandles.end()) return k->second;

  return NULL;
}

NodeHandleInterface *NodeManager::handle(const QString &stringId,unsigned int version) {

  const QString id = nodeStringIdVersion(stringId,version);

  map<QString,NodeHandleInterface *>::const_iterator i = _coreHandles.find(id);
  if(i!=_coreHandles.end()) return i->second;
  
  map<QString,CustomNodeHandle *>::const_iterator j = _customHandles.find(id);
  if(j!=_customHandles.end()) return j->second;

  // TODO: loop on groupHandle?

  return NULL;
}

const QString NodeManager::coreStringId(const QString &name,const QString &path,NodeInterface *node) {
  if(isCoreNode(name,path)) {
    NodeHandleInterface *h = handle(name,path);
    return h->stringId();
  } else if(isCustomNode(name,path)) {
    CustomNodeHandle *h = static_cast<CustomNodeHandle *>(handle(name,path));
    return h->coreStringId();
  } else if(isGroupedNode(name,path)) {
    CustomNodeHandle *h = static_cast<CustomNodeHandle *>(handle(name,path));
    return h->coreStringId();
  } else {
    if(node) return node->stringId();
    cerr << __FILE__ << " - " << __LINE__ << ": unknown node type" << endl;
    return QString();
  }
}

unsigned int NodeManager::coreVersion(const QString &name,const QString &path,NodeInterface *node) {
  if(isCoreNode(name,path)) {
    return handle(name,path)->version();
  } else if(isCustomNode(name,path)) {
    CustomNodeHandle *h = static_cast<CustomNodeHandle *>(handle(name,path));
    return h->coreVersion();
  } else if(isGroupedNode(name,path)) {
    CustomNodeHandle *h = static_cast<CustomNodeHandle *>(handle(name,path));
    return h->coreVersion();
  } else {
    if(node) return node->version();
    cerr << __FILE__ << " - " << __LINE__ << ": unknown node type" << endl;
    return 0;
  }
}

bool NodeManager::isGroupedNode(NodeInterface *node) {
  if(!node) return false;
  cout<<"found : "<< node->stringId().toStdString() <<endl;
  cout<<"wanted : "<< NodeGrouped::GROUPED_NODE_ID.toStdString() <<endl;
  return node->stringId()==NodeGrouped::GROUPED_NODE_ID;
}
