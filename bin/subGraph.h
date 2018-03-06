/*
 *  subGraph.h
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */
#ifndef SubGraph_h
#define SubGraph_h

#include <vector>
#include <map>
#include <set>
#include "inputGraph.h"

class SubGraph {
  
private:
  int ** matrix;
  InputGraph * inGraph;
  std::vector< int > * numSubToInput;
  int numOfNodes;
  std::map< int , int >numInputToSub;
  
public:
  
  SubGraph(std::vector< int > *, InputGraph * );
  SubGraph(std::vector< int > *, InputGraph *, float,float);
  ~SubGraph();
  void separators();
  InputGraph * getInputGraph() {return inGraph;};
  std::map< int , int > & getNumInputToSub(){return numInputToSub;};
  std::vector< int > & getNumSubToInput(){return *numSubToInput;};
  int getNumOfNodes(){return numOfNodes;};
  float getEvalue(){return inGraph->getEvalue();};
  float getSimilarity(){return inGraph->getSimilarity();};
  
private:
  SubGraph();
  void addFill(int,int);
  bool isFill(int,int);
  bool isEdge(int,int);
  bool isComplete(std::set< int > &);
};

#endif
