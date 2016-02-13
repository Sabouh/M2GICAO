// This file is part of Gratin, a programmable Node-based System 
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "core/pbgraph.h"
#include "core/nodeGrouped.h"
#include "core/nodeManager.h"

using namespace std;

PbGraph::PbGraph()
  : _adjacencyMatrix(AdjacencyMatrix()),
    _topologicalList(TopologicalList()),
    _perNodeTL(PerNodeTL()){

}

PbGraph::~PbGraph() {
  _adjacencyMatrix.clear();
  _topologicalList.clear();
  _perNodeTL.clear();
}

bool PbGraph::addNode(NodeInterface *node) {
  // add the node into the adjacency matrix
  if(!addNodeNoProcess(node)) return false;
  // this new node may be ready to process - check it
  prepareForProcessing(false,false);
  return true;
}

bool PbGraph::addNodeNoProcess(NodeInterface *node) {
  // as this node has no connections, we can simply add it at the end of the matrix
  if(!node) return false;

  // set node ids
  node->setNodeId(static_cast<unsigned int>(_adjacencyMatrix.size()));
  node->setGraphId(nbConnexGraphs());

  // set adjacency matrix
  unsigned int n = nbNodes();
  _adjacencyMatrix.push_back(vector<vector<Vector2u> >(n));
  for(unsigned int i=0;i<n+1;++i) {
      _adjacencyMatrix[i].push_back(vector<Vector2u>());
  }

  // set topological list
  _topologicalList.push_back(node);

  return true;
}

bool PbGraph::removeNode(NodeInterface *node,bool clean) {

  // remove the node into the adjacency matrix
  if(!removeNodeNoProcess(node)) return false;
  if(clean) cleanNode(node->nodeId());

  // prepare the topological list and the graph for processing
  prepareForProcessing();

  return true;
}

bool PbGraph::removeNodeNoProcess(NodeInterface *node) {
  if(!node) return false;

  // get id for this node
  NodeID nodeId = node->nodeId();

  // remove all input connections
  vector<NodeID> inNodes = ingoingNodes(nodeId);
  for(NodeID i=0;i<inNodes.size();++i) {
      NodeInterface *src = _topologicalList[inNodes[i]];
      NodeInterface *dst = _topologicalList[nodeId];
      vector<Vector2u> connections = _adjacencyMatrix[src->nodeId()][dst->nodeId()];

      for(unsigned int j=0;j<connections.size();++j) {
          removeConnectionNoProcess(src,connections[j][0],dst,connections[j][1]);
      }
  }

  // remove all output connections
  vector<NodeID> outNodes = outgoingNodes(nodeId);
  for(NodeID i=0;i<outNodes.size();++i) {
      NodeInterface *src = _topologicalList[nodeId];
      NodeInterface *dst = _topologicalList[outNodes[i]];
      vector<Vector2u> connections = _adjacencyMatrix[src->nodeId()][dst->nodeId()];

      for(unsigned int j=0;j<connections.size();++j) {
          removeConnectionNoProcess(src,connections[j][0],dst,connections[j][1]);
      }
  }

  // remove the node in the graph itself (shift each node)
  for(unsigned int i=0;i<nbNodes();++i) {
      _adjacencyMatrix[i].erase(_adjacencyMatrix[i].begin()+nodeId);
  }
  _adjacencyMatrix.erase(_adjacencyMatrix.begin()+nodeId);

  // set the graph ids for each node
  for(unsigned int i=nodeId+1;i<nbNodes();++i) {
      _topologicalList[i]->setNodeId(i-1);
  }

  // remove the node from the topological list
  _topologicalList.erase(_topologicalList.begin()+nodeId);

  return true;
}

bool PbGraph::makeConnection(NodeInterface *src, IONodeID outputId, NodeInterface *dst, IONodeID inputId) {
  // make the connection inside the adjacency matrix
  if(!makeConnectionNoProcess(src,outputId,dst,inputId)) return false;

  // reinit the modified graph for rendering
  prepareForProcessing();

  return true;
}

bool PbGraph::makeConnectionNoProcess(NodeInterface *src, IONodeID outputId, NodeInterface *dst, IONodeID inputId) {
  if(!src || !dst) return false;

  NodeID srcId = src->nodeId();
  NodeID dstId = dst->nodeId();

  // check input/output viability
  if(outputId>=src->nbOutputs() || inputId>=dst->nbInputs()) return false;

  // check if this inputId is already connected (iterate over ingoing connections)
  vector<NodeID> inNodes = ingoingNodes(dstId);
  for(unsigned int i=0;i<inNodes.size();++i) {
      for(unsigned int j=0;j<_adjacencyMatrix[inNodes[i]][dstId].size();++j) {
          if(_adjacencyMatrix[inNodes[i]][dstId][j][1]==inputId)
            return false;
      }
  }

  // check if this connection creates a cycle
  if(createCycle(srcId,dstId)) return false;

  // check if the data format is ok between output and input
  if(!dst->checkDataType(src,outputId,inputId)) return false;

  // create the connection
  _adjacencyMatrix[srcId][dstId].push_back(Vector2u(outputId,inputId));

  return true;
}

bool PbGraph::removeConnection(NodeInterface *src, IONodeID outputId, NodeInterface *dst, IONodeID inputId) {
  // make the connection inside the adjacency matrix
  if(!removeConnectionNoProcess(src,outputId,dst,inputId)) return false;

  // reinit the modified graph for rendering
  prepareForProcessing();

  return true;
}

bool PbGraph::removeConnectionNoProcess(NodeInterface *src, IONodeID outputId, NodeInterface *dst, IONodeID inputId) {
  if(!src || !dst) return false;

  NodeID srcId = src->nodeId();
  NodeID dstId = dst->nodeId();

  // check input/output viability
  if(outputId>=src->nbOutputs() || inputId>=dst->nbInputs()) return false;

  vector<Vector2u> connections = _adjacencyMatrix[srcId][dstId];
  Vector2u c(outputId,inputId);
  for(vector<Vector2u>::iterator i=connections.begin();i!=connections.end();++i) {
      // remove if we find this connection
      if(*i==c) {
        connections.erase(i);
        break;
      }
  }

  // replace the new connections in the adjacency matrix
  _adjacencyMatrix[srcId][dstId] = connections;

  return true;
}

void PbGraph::prepareForProcessing(bool needGraphIds,bool needTopList,bool updateNodes) {
  // recompute graph ids if needed
  if(needGraphIds)
    setGraphIds();

  // recompute topological list if needed
  if(needTopList)
    setTopologicalList();

  computePerNodeTopologicalList();

  // in all cases, we have to check if some new nodes are ready to process
  for(NodeID i=0;i<nbNodes();++i) {
    
    vector<pair<NodeInterface *, Vector2u> >  inConnections = inputNodeData(i);
    
    // this array is then given to the node
    _topologicalList[i]->setInputData(inConnections);

    if(updateNodes) {
      switch(_topologicalList[i]->state()) {
      case NodeInterface::READY_TO_NOT_READY:
	cleanNode(i);break;
      case NodeInterface::NOT_READY_TO_READY:
	initNode(i);break;
      case NodeInterface::READY_TO_READY:
	cleanNode(i);initNode(i);break;
      default:break;
      }
    }
  }
}

const vector<pair<NodeInterface *,Vector2u> > PbGraph::inputNodeData(NodeID nodeId) {
  // get all the input connections (stored as <nodeId,outConnect,inConnect>)
  vector<NodeID> inNodes = ingoingNodes(nodeId);
  vector<pair<NodeInterface *, Vector2u> >  inConnections;
  
  // for each input connection...
  for(NodeID j=0;j<inNodes.size();++j) {
    NodeID id = inNodes[j];
    
    // ... we get all the in/out entries...
    vector<Vector2u> connections = _adjacencyMatrix[id][nodeId];
    for(unsigned int k=0;k<connections.size();++k) {
      // ... and put them in an array
      inConnections.push_back(pair<NodeInterface *,Vector2u>(_topologicalList[id],connections[k]));
    }
  }

  return inConnections;
}

void PbGraph::setGraphIds() {
  // get new graph ids
  vector<unsigned int> graphIds = computeGraphIds();

  // set graph ids for each node
  for(unsigned int i=0;i<nbNodes();++i) {
      _topologicalList[i]->setGraphId(graphIds[i]);
  }
}

void PbGraph::setTopologicalList() {
  // get new topological list
  vector<NodeID> newNodeIds = computeTopologicalList();

  // first set node ids
  for(NodeID i=0;i<nbNodes();++i) {
    _topologicalList[newNodeIds[i]]->setNodeId(i);
  }

  // copy graph 
  AdjacencyMatrix adjMatrixTmp = _adjacencyMatrix;
  TopologicalList topListTmp   = _topologicalList;

  // re-order the graph
  for(NodeID i=0;i<nbNodes();++i) {
      // the line oldId should be placed at the line newId
      NodeID newRowId = i;
      NodeID oldRowId = newNodeIds[i];

      for(NodeID j=0;j<nbNodes();++j) {
          NodeID oldColumnId = j;
          NodeID newColumnId = _topologicalList[j]->nodeId();

          _adjacencyMatrix[newRowId][newColumnId] = adjMatrixTmp[oldRowId][oldColumnId];
      }
  }

  // re-order the topological list
  for(unsigned int i=0;i<nbNodes();++i) {
      _topologicalList[i] = topListTmp[newNodeIds[i]];
  }
}

vector<unsigned int> PbGraph::computeGraphIds() const {
  // this array serves as marked and store the new graph ids
  vector<bool> marked = vector<bool>(nbNodes(),false);
  vector<unsigned int> graphIds = vector<unsigned int>(nbNodes(),0);

  // apply a depth first search on each node
  unsigned int currentId = 0;
  for(NodeID i=0;i<nbNodes();++i) {
    if(computeGraphIdsDFS(i,currentId,marked,graphIds))
      currentId++;
  }

  return graphIds;
}

bool PbGraph::computeGraphIdsDFS(NodeID nodeId,int graphId,vector<bool> &marked,vector<unsigned int> &graphIds) const {
  // do nothing if the node is already marked
  if(marked[nodeId]) return false;

  // then mark it and update graph id
  marked[nodeId]   = true;
  graphIds[nodeId] = graphId;

  // backward ...
  vector<NodeID> inNodes = ingoingNodes(nodeId);
  for(unsigned int i=0;i<inNodes.size();++i) {
     computeGraphIdsDFS(inNodes[i],graphId,marked,graphIds);
  }

  // ... and forward checking
  vector<NodeID> outNodes = outgoingNodes(nodeId);
  for(unsigned int i=0;i<outNodes.size();++i) {
     computeGraphIdsDFS(outNodes[i],graphId,marked,graphIds);
  }

  // return true if a non marked node was found
  return true;
}

vector<PbGraph::NodeID> PbGraph::computeTopologicalList() const {
  // tmp and final sorted list
  list<NodeID> sortedTmp;
  vector<NodeID> sorted;

  // apply a (reverse) depth first search, starting from output nodes
  vector<NodeID> outNodes = outputNodes();
  vector<bool>   marked   = vector<bool>(nbNodes(),false);

  for(unsigned int i=0;i<outNodes.size();++i) {
      computeTopologicalListDFS(outNodes[i],marked,sortedTmp);
  }

  // get the number of graphs (we cannot use the function nbConnexGraph, as the tlist is not already sorted)
  unsigned int nbConnex = 0;
  for(unsigned int i=0;i<nbNodes();++i) {
      if(_topologicalList[i]->graphId()>=nbConnex)
        nbConnex = _topologicalList[i]->graphId()+1;
  }

  // reorder using graph ids to be sure that we won't call not needed nodes
  for(unsigned int i=0;i<nbConnex;++i) {
      const unsigned int currentGraph = i;
      list<NodeID>::iterator it=sortedTmp.begin();

      while(it!=sortedTmp.end()) {
          const NodeID currentNode = *it;
          if(_topologicalList[currentNode]->graphId()==currentGraph) {
            sorted.push_back(currentNode);
            it = sortedTmp.erase(it);
          } else {
            it++;
          }
      }
  }

  return sorted;
}

void PbGraph::computeTopologicalListDFS(NodeID nodeId,vector<bool> &marked,list<PbGraph::NodeID> &sorted) const {
  // do nothing if this node has already been visited
  if(marked[nodeId]) return;

  // mark this node as visited
  marked[nodeId] = true;

  // recursion for each incoming connections
  vector<NodeID> inNodes = ingoingNodes(nodeId);
  for(unsigned int i=0;i<inNodes.size();++i) {
      computeTopologicalListDFS(inNodes[i],marked,sorted);
  }

  // add this node to the topological list
  sorted.push_back(nodeId);
}

bool PbGraph::createCycle(NodeID srcId, NodeID dstId) const {
  // apply a depth first search to check wether the connection from srdId to dstId create a cycle or not
  vector<bool> marked = vector<bool>(nbNodes(),false);

  // starting from dstId, we should never be able to reach srcId
  return createCycleDFS(dstId,srcId,marked);
}

bool PbGraph::createCycleDFS(NodeID startId, NodeID forbidenId, vector<bool> &marked) const {
  marked[startId] = true;

  // if we reached the forbiden id, there is a loop
  if(startId==forbidenId) return true;

  // iterate over outgoing connections
  vector<NodeID> outNodes = outgoingNodes(startId);
  for(unsigned int i=0;i<outNodes.size();++i) {
      if(marked[outNodes[i]])
        continue;
      if(createCycleDFS(outNodes[i],forbidenId,marked))
        return true;
  }

  return false;
}

void PbGraph::computePerNodeTopologicalList() {
  // for each node, compute the associated topological list
  // convenient structure for accelerating node process

  // assume that the topological list has been computed
  _perNodeTL.clear();
  _perNodeTL = vector<vector<NodeID> >(nbNodes());

  for(unsigned int i=0;i<nbNodes();++i) {

      // each node has an associated node list, including itself
     vector<bool> marked = vector<bool>(nbNodes(),false);
     computePerNodeTopologicalListDFS(i,marked);

     for(unsigned int j=0;j<nbNodes();++j) {
        if(marked[j]) {
           _perNodeTL[i].push_back(j);
        }
     }
  }
}

void PbGraph::computePerNodeTopologicalListDFS(NodeID nodeId,vector<bool> &marked) {
  marked[nodeId] = true;

  // iterate over outgoing connections
  vector<NodeID> outNodes = outgoingNodes(nodeId);
  for(unsigned int i=0;i<outNodes.size();++i) {
      if(marked[outNodes[i]])
        continue;
      computePerNodeTopologicalListDFS(outNodes[i],marked);
  }
}


NodeGrouped *PbGraph::groupNodes(const vector<NodeInterface *> &nodes) {
  // compute relative position
  Vector2f pos(0,0);
  for(unsigned int i=0;i<nodes.size();++i) {
    pos = pos+nodes[i]->pos();
  }
  pos = pos/(float)nodes.size();


  // get connections 
  vector<vector<pair<NodeInterface *, Vector2u> > > connections(nodes.size());
  for(unsigned int i=0;i<nodes.size();++i) { // for each nodes
      NodeID srcId = nodes[i]->nodeId();
      
      for(unsigned int j=0;j<nodes.size();++j) { // for each possibly connected node
	NodeID dstId = nodes[j]->nodeId();
    	
	vector<Vector2u> c = nodeConnections(srcId,dstId); // get connections... 

	for(unsigned int k=0;k<c.size();++k) { // ... and add them in an array
	  connections[i].push_back(pair<NodeInterface *,Vector2u>(nodes[j],c[k]));
	}
      }
  }
  
  // we can remove everything from the current graph and update node positions
  for(unsigned int i=0;i<nodes.size();++i) {
    nodes[i]->setPos(nodes[i]->pos()-pos);
    removeNode(nodes[i],false);
  }

  // create the nodeGrouped
  NodeGrouped *nodeGrouped = new NodeGrouped(this);
  nodeGrouped->initEmptyData(pos);
  nodeGrouped->unlock(); // the graph can be modified

  // add all the nodes
  for(unsigned int i=0;i<nodes.size();++i) {
      nodeGrouped->addNode(nodes[i]);
      nodes[i]->setParentGraph(nodeGrouped);
  }

  //create connections
  for(unsigned int i=0;i<nodes.size();++i) {
      for(unsigned int j=0;j<connections[i].size();++j) {
          NodeInterface *src = nodes[i];
          NodeInterface *dst = connections[i][j].first;
          NodeID outputId    = (connections[i][j].second)[0];
          NodeID inputId     = (connections[i][j].second)[1];

          nodeGrouped->makeConnection(src,outputId,dst,inputId);
      }
  }

  nodeGrouped->lock();   // the graph cannot be modified anymore

  // add this node to the current graph
  addNode(nodeGrouped);

  nodeGrouped->updatePositions();

  return nodeGrouped;
}

NodeGrouped *PbGraph::ungroupNode(NodeGrouped *nodeGrouped) {
  NodeID id = nodeGrouped->nodeId();

  // deploy only if the node is not connected
  if(!ingoingNodes(id).empty() || !outgoingNodes(id).empty()) return NULL;

  vector<pair<NodeInterface *,NodeInterface *> > cnodes;
  vector<Vector2u> cio;
  vector<NodeInterface *> nodes = nodeGrouped->topologicalList();
  nodeGrouped->getConnections(cnodes,cio);

  nodeGrouped->unlock();
  for(int i=(int)nodeGrouped->nbNodes()-1;i>=0;--i) {
    nodeGrouped->removeNodeNoProcess(nodeGrouped->node(i));
  }
  nodeGrouped->lock();

  removeNodeNoProcess(nodeGrouped);

  for(unsigned int i=0;i<nodes.size();++i) {
    nodes[i]->setPos(nodeGrouped->pos()+nodes[i]->pos());
    nodes[i]->setParentGraph(this);
    addNodeNoProcess(nodes[i]);
  }

  for(unsigned int i=0;i<cnodes.size();++i) {
    makeConnectionNoProcess(cnodes[i].first,cio[i][0],cnodes[i].second,cio[i][1]);
  }
  
  prepareForProcessing(true,true);

  // update positions
  for(unsigned int i=0;i<nodes.size();++i) {
    nodes[i]->updatePositions();
  }

  return nodeGrouped;
}

void PbGraph::print() const {
  cout << "*** ADJACENCY MATRIX ***" << endl;
  for(unsigned int i=0;i<nbNodes();++i) {
      cout << endl;
      for(unsigned int j=0;j<nbNodes();++j) {
          if(_adjacencyMatrix[i][j].empty())
            cout << "0 ";
          else
            cout << "1 ";
      }
      cout << "(" << _topologicalList[i]->isReady() << ")";
  }

  cout << endl << endl << "*** TOPOLOGICAL LIST ***" << endl;

  for(unsigned int i=0;i<nbNodes();++i) {
      cout << "node " << i << ":" << _topologicalList[i]->graphId() << endl;
  }
}

bool PbGraph::save(SceneSaver  *saver) {
  bool ret = true;

  if(!saveNodeParams(saver))      ret = false;
  if(!saveNodeConnections(saver)) ret = false;
  if(!saveNodeData(saver))        ret = false;

  return ret;
}

bool PbGraph::load(SceneLoader *loader) {
  bool ret = true;
    
  if(!loadNodeParams(loader))      ret = false; 
  if(!loadNodeConnections(loader)) ret = false; 
  prepareForProcessing(false,false); 
  if(!loadNodeData(loader))        ret = false; 

  // make sure that all nodes are successfully initialized 
  for(unsigned int i=0;i<nbNodes();++i) {
    cleanNode(i);
    initNode(i);
  }

  return ret;
}

bool PbGraph::saveNodeParams(SceneSaver  *saver) {
  bool ret = true;

  // save the number of nodes 
  if(!saver->saveUint("nbNodes",nbNodes())) ret = false;

  // save each node 
  for(unsigned int i=0;i<nbNodes();++i) {
    NodeManager::instance()->saveNode(saver,_topologicalList[i],this,ret,i);
  }
  
  return ret;
}

bool PbGraph::saveNodeConnections(SceneSaver  *saver) {
  bool ret = true;

  // save node connections 
  for(unsigned int i=0;i<nbNodes();++i) {
    for(unsigned int j=i+1;j<nbNodes();++j) {
      if(!saver->saveUint(Tags::attribId("nbConnections",i,j),
                   static_cast<unsigned int>(_adjacencyMatrix[i][j].size())))
	ret = false;   
      for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {
	if(!saver->saveVector2u(Tags::attribId("connection",i,j,k),
					 _adjacencyMatrix[i][j][k])) 
	  ret = false;
      }
    }
  }
  
  return ret;
}

bool PbGraph::saveNodeData(SceneSaver  *saver) {
  bool ret = true;

  for(unsigned int i=0;i<nbNodes();++i) {
    NodeManager::instance()->saveNodeData(saver,_topologicalList[i],ret,i);
  }

  return ret;
}

bool PbGraph::loadNodeParams(SceneLoader  *loader) {
  bool ret = true;

  if(nbNodes()>0) return false;
  unsigned int nb = 0;
  _topologicalList.clear();
  _adjacencyMatrix.clear();

  if(!loader->loadUint("nbNodes",nb)) ret = false;
  
  if(nb>0) {
    // init arrays
    _topologicalList = vector<NodeInterface *>(nb,NULL);
    _adjacencyMatrix = vector<vector<vector<Vector2u> > >(nb);
    for(unsigned int i=0;i<nb;++i) {
      _adjacencyMatrix[i] = vector<vector<Vector2u> >(nb);
    }
  }

  for(unsigned int i=0;i<nbNodes();++i) {
    cout << "loading node " << i << ": ";
    _topologicalList[i] = NodeManager::instance()->loadNode(loader,this,ret,i);

    if(_topologicalList[i]) {
      cout << _topologicalList[i]->name().toStdString() << endl;
      _topologicalList[i]->initEmptyData(_topologicalList[i]->pos());
    } else {
      cout << "ERROR (unkonwn node)!" << endl;
    }
  }
  
  return ret;
}

bool PbGraph::loadNodeConnections(SceneLoader  *loader) {
  bool ret = true;
  
  for(unsigned int i=0;i<nbNodes();++i) {
    for(unsigned int j=i+1;j<nbNodes();++j) {
      unsigned int nbConnections=0;
      if(!loader->loadUint(Tags::attribId("nbConnections",i,j),nbConnections)) 
	ret = false;
      if(nbConnections>0) _adjacencyMatrix[i][j]=vector<Vector2u>(nbConnections);
   
      for(unsigned int k=0;k<_adjacencyMatrix[i][j].size();++k) {

	cout << "connecting ";
	if(_topologicalList[i]) 
	  cout << _topologicalList[i]->name().toStdString() << " and "; 
	else 
	  cout << "Unknown" << " and ";
	if(_topologicalList[j]) 
	  cout << _topologicalList[j]->name().toStdString() << " (" << k << ")" << endl;
	else 
	  cout << "Unknown" << " (" << k << ")" << endl;
	
	if(!loader->loadVector2u(Tags::attribId("connection",i,j,k),_adjacencyMatrix[i][j][k])) 
	  ret = false;
      }
    }
  }
  
  return ret;
}

bool PbGraph::loadNodeData(SceneLoader  *loader) {
  bool ret = true;

  for(unsigned int i=0;i<nbNodes();++i) {
    cout << "loading user defined node data " << i << ": ";
    if(_topologicalList[i]) 
      cout << _topologicalList[i]->name().toStdString() << endl;
    else 
      cout << "(unknown node)!" << endl;

    NodeManager::instance()->loadNodeData(loader,_topologicalList[i],ret,i);
  }

  return ret;
}

NodeInterface *PbGraph::rootNode(NodeInterface *node) {
  // return the top-level node associated with this graph (in case the node is part of a group)
  if(node->parentGraph()==this) { return node; } 

  NodeGrouped *n = (NodeGrouped *)(node->parentGraph());
  while(n->parentGraph()!=this) {
    n = (NodeGrouped *)(n->parentGraph());
  }

  return n;
}
