// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef PBGRAPH_H
#define PBGRAPH_H

#include "core/nodeInterface.h"
#include "core/sceneIOTags.h"

#include <vector>
#include <list>
#include <Eigen/Dense>
#include <QObject>

using namespace Eigen;

class NodeGrouped;

class PbGraph : public QObject, public IOData {
  Q_OBJECT

public:
  // a node is uniquely defined with an unsigned integer
  typedef unsigned int NodeID;

  // each node has input/output constraints defined as uint too
  typedef unsigned int IONodeID;

  // adjacency matrix representation for the connections
  // each cell contains a list of vec2 that store output id for the src node and input id for the dst node
  typedef std::vector<std::vector<std::vector<Vector2u> > > AdjacencyMatrix;

  // the node data structure is stored in a topological list
  typedef std::vector<NodeInterface *> TopologicalList;

  // acceleration structure for applying nodes
  typedef std::vector<std::vector<NodeID> > PerNodeTL;

  PbGraph();
  virtual ~PbGraph();


  // add a node to the graph
  virtual bool addNode   (NodeInterface *node);

  // remove a node from the graph (do not delete the node)
  virtual bool removeNode(NodeInterface *node,bool clean=true);

  // make a connection between src and dst (from output to input ids)
  virtual bool makeConnection  (NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId);

  // remove a connection between src and dst (from output to input ids) 
  virtual bool removeConnection(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId);

  /* // ***** deprecated ******* //  */
  /* // create a nodeGrouped with the connex part of the graph associated with node. */
  /* virtual NodeGrouped *connexPartToNodeGrouped(NodeInterface *node);  */

  /* // deploy the nodeGrouped and put it in the graph (remove the node from the graph too). */
  /* virtual NodeGrouped *nodeGroupedToConnexPart(NodeGrouped *nodeGrouped); */

  // create a group with given nodes 
  virtual NodeGrouped *groupNodes(const std::vector<NodeInterface *> &nodes); 

  // deploy the node and put it in the graph (remove the node from the graph too).
  virtual NodeGrouped *ungroupNode(NodeGrouped *nodeGrouped);

  // send events between nodes 
  inline virtual void sendEventFrom (NodeInterface *node,const NodeEvent &event);
  inline virtual void sendEventAll(const NodeEvent &event);

  // applying funtions 
  inline virtual void applyFrom (NodeInterface *node);
  inline virtual void applyAll();
  inline virtual void applyAllWithCond(const NodeCondition &condition);
  inline virtual void applyFromWithCond(NodeInterface *node,const NodeCondition &condition);

  // reloading
  inline virtual void reloadAll();

  inline bool         isEmpty() const {return _adjacencyMatrix.empty();}
  inline unsigned int nbNodes() const {return static_cast<unsigned int>(_adjacencyMatrix.size()); }
  inline NodeInterface *node(unsigned int i) {return _topologicalList[i];}
  NodeInterface *rootNode(NodeInterface *node);

  inline bool isNodePartlyConnected(NodeInterface *node) const;

  inline const AdjacencyMatrix &adjacencyMatrix() const {return _adjacencyMatrix;}
  inline const TopologicalList &topologicalList() const {return _topologicalList;}
  inline const PerNodeTL       &perNodeTL()       const {return _perNodeTL;}

  void print() const;

  inline virtual void refreshView() {emit nodeContentChanged();}
  
  // load and save 
  virtual bool save(SceneSaver  *saver );
  virtual bool load(SceneLoader *loader);

  // accessor for node input data 
  virtual const std::vector<std::pair<NodeInterface *,Vector2u> > inputNodeData(NodeID nodeId);
  
  inline virtual const QStringList graphNames() {return QStringList();}

 signals:
  void nodeContentChanged();

 protected:
  AdjacencyMatrix _adjacencyMatrix; // the graph...
  TopologicalList _topologicalList; // the associated topological list
  PerNodeTL       _perNodeTL;       // store the asociated nodes for each node

  // same functions as in the public interface, except that they do not call the prepareProcess
  // usefull when a big change has to be made (multiple connexions/add/etc...)
  bool addNodeNoProcess         (NodeInterface *node);
  bool removeNodeNoProcess      (NodeInterface *node);
  bool makeConnectionNoProcess  (NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId);
  bool removeConnectionNoProcess(NodeInterface *src,IONodeID outputId,NodeInterface *dst,IONodeID inputId);

  // re-order the graph in a topological order. init/clean node data if necessary
  void prepareForProcessing(bool needGraphIds=true,bool needTopList=true,bool updateNodes=true);

  // recompute and change the graph id associated with each node (at which connex graph each belongs to)
  void setGraphIds();

  // recompute and change the topological list
  void setTopologicalList();

  // recursive depth-first-search function for computing graph ids
  std::vector<unsigned int> computeGraphIds() const;
  bool computeGraphIdsDFS(NodeID nodeId,int graphId,std::vector<bool> &marked,std::vector<unsigned int> &graphIds) const;

  // recursive depth-first-search functions for testing cycles
  bool createCycle(NodeID srcId,NodeID dstId) const;
  bool createCycleDFS(NodeID startId,NodeID forbidenId,std::vector<bool> &marked) const;

  // recursives depth-first-search functions for computing the topological list
  std::vector<NodeID> computeTopologicalList() const;
  void computeTopologicalListDFS(NodeID nodeId,std::vector<bool> &marked,std::list<NodeID> &sorted) const;
  void computePerNodeTopologicalList();
  void computePerNodeTopologicalListDFS(NodeID nodeId,std::vector<bool> &marked);

  bool saveNodeParams     (SceneSaver   *saver );
  bool saveNodeConnections(SceneSaver   *saver );
  bool saveNodeData       (SceneSaver   *saver );
  bool loadNodeParams     (SceneLoader  *loader);
  bool loadNodeConnections(SceneLoader  *loader);
  bool loadNodeData       (SceneLoader  *loader);

  // useful function to test or get data from the graph
  inline unsigned int nbConnexGraphs() const;
  inline bool connected(NodeID srcId,NodeID dstId) const;
  inline std::vector<Vector2u> nodeConnections(NodeID srcId,NodeID dstId) const;
  inline std::vector<PbGraph::NodeID> ingoingNodes(NodeID id) const;
  inline std::vector<PbGraph::NodeID> outgoingNodes(NodeID id) const;
  inline std::vector<PbGraph::NodeID> inputNodes() const;
  inline std::vector<PbGraph::NodeID> outputNodes() const;

  // the main functions written for facility here (assume that nodeId is in the graph)
  inline void initNode(NodeID nodeId);
  inline void applyNode(NodeID nodeId);
  inline void reloadNode(NodeID nodeId);
  inline void cleanNode(NodeID nodeId);
  inline void sendEventNode(NodeID nodeId,const NodeEvent &event);
  inline bool applyNodeWithCondition(NodeID nodeId,const NodeCondition &condition);

public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

inline unsigned int PbGraph::nbConnexGraphs() const {
  // assume that the topological list is ordered and node ids updated
  return isEmpty() ? 0 : _topologicalList[_topologicalList.size()-1]->graphId()+1;
}

inline bool PbGraph::connected(NodeID srcId,NodeID dstId) const {
  // assume that src and dst are contained in the adjacency matrix
  return !_adjacencyMatrix[srcId][dstId].empty();
}

inline std::vector<Vector2u> PbGraph::nodeConnections(NodeID srcId,NodeID dstId) const {
  // assume that src and dst are contained in the adjacency matrix
  return _adjacencyMatrix[srcId][dstId];
}

inline std::vector<PbGraph::NodeID> PbGraph::ingoingNodes(NodeID id) const {
  // assume that id is contained in the adjacency matrix
  std::vector<NodeID> list;
  for(NodeID i=0;i<nbNodes();++i) {
      if(connected(i,id))
        list.push_back(i);
  }
  return list;
}

inline std::vector<PbGraph::NodeID> PbGraph::outgoingNodes(NodeID id) const {
  // assume that id is contained in the adjacency matrix
  std::vector<NodeID> list;
  for(NodeID i=0;i<nbNodes();++i) {
      if(connected(id,i))
        list.push_back(i);
  }
  return list;
}

inline std::vector<PbGraph::NodeID> PbGraph::inputNodes() const {
  // all nodes that do not have any input connection
  std::vector<NodeID> list;
  for(NodeID i=0;i<nbNodes();++i) {
      if(ingoingNodes(i).empty())
        list.push_back(i);
  }
  return list;
}

inline std::vector<PbGraph::NodeID> PbGraph::outputNodes() const {
  // all nodes that do not have any output connection
  std::vector<NodeID> list;
  for(NodeID i=0;i<nbNodes();++i) {
      if(outgoingNodes(i).empty())
        list.push_back(i);
  }
  return list;
}

inline void PbGraph::initNode(NodeID nodeId) {
  if(_topologicalList[nodeId]->isReady()) {
    _topologicalList[nodeId]->init();
  }
}

inline void PbGraph::applyNode(NodeID nodeId) {
  if(_topologicalList[nodeId]->isReady())
    _topologicalList[nodeId]->apply();
}

inline void PbGraph::reloadNode(NodeID nodeId) {
  if(_topologicalList[nodeId]->isReady())
    _topologicalList[nodeId]->reload();
}

inline void PbGraph::cleanNode(NodeID nodeId) {
  _topologicalList[nodeId]->clean();
}

inline void PbGraph::sendEventNode(NodeID nodeId,const NodeEvent &event) {
  if(_topologicalList[nodeId]->isReady())
    _topologicalList[nodeId]->receiveEvent(event);
}

inline bool PbGraph::applyNodeWithCondition(NodeID nodeId,const NodeCondition &condition) {
  if(_topologicalList[nodeId]->isReady() && _topologicalList[nodeId]->satisfy(condition)) {
    _topologicalList[nodeId]->apply();
    return true;
  }
  return false;
}

inline void PbGraph::sendEventFrom(NodeInterface *node,const NodeEvent &event) {
  const unsigned int nodeId  = node->nodeId();

  for(unsigned int i=0;i<_perNodeTL[nodeId].size();++i) {
    sendEventNode(_perNodeTL[nodeId][i],event);
  }
}

inline void PbGraph::applyFrom(NodeInterface *node) {
  const unsigned int nodeId  = node->nodeId();

  for(unsigned int i=0;i<_perNodeTL[nodeId].size();++i) {
    applyNode(_perNodeTL[nodeId][i]);
  }
}

inline void PbGraph::sendEventAll(const NodeEvent &event) {
  for(unsigned int i=0;i<nbNodes();++i) {
    sendEventNode(i,event);
  }
}

inline void PbGraph::applyAll() {
  for(unsigned int i=0;i<nbNodes();++i) {
    applyNode(i);
  }
}

inline void PbGraph::reloadAll() {
  for(unsigned int i=0;i<nbNodes();++i) {
    reloadNode(i);
  }
}

inline void PbGraph::applyAllWithCond(const NodeCondition &condition) {
  std::vector<bool> seen(nbNodes(),false);

  for(unsigned int i=0;i<nbNodes();++i) {
    if(!seen[i] && applyNodeWithCondition(i,condition)) {
        seen[i] = true;
        // if the current node satistfies the condition,
        // we have to apply all its connected neighbors
        for(unsigned int j=0;j<_perNodeTL[i].size();++j) {
          if(!seen[_perNodeTL[i][j]]) {
            if(!applyNodeWithCondition(_perNodeTL[i][j],condition)) {
              applyNode(_perNodeTL[i][j]);
            }
            seen[_perNodeTL[i][j]] = true;
          }
        }
    }
  }
}

inline void PbGraph::applyFromWithCond(NodeInterface *node,const NodeCondition &condition) {
  if(applyNodeWithCondition(node->nodeId(),condition)) {
    //std::cout << __FILE__ << " - " << __FUNCTION__ << " - " << __LINE__ << std::endl;
    for(unsigned int i=0;i<_perNodeTL[node->nodeId()].size();++i) {
      applyNode(_perNodeTL[node->nodeId()][i]);
    }
  }
}

inline bool PbGraph::isNodePartlyConnected(NodeInterface *node) const {
  return !ingoingNodes(node->nodeId()).empty() || !outgoingNodes(node->nodeId()).empty();
}


#endif // PBGRAPH_H
