/*
 *  separator.h
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */

#ifndef Separators_h
#define Separators_h

#include "inputGraph.h"
#include "families.h"
#include "subGraph.h"
#include <set>
#include <vector>

class Separators
{
  
private:
  SubGraph * subGraph;
  std::set< int > separatorNodes;
  std::set< int > closedNeighborhood;
  std::vector<Families> separatorFamilies;
  
public:
  Separators(SubGraph *);
  std::set< int > & getSeparatorNodes() { return separatorNodes; } ;
  void separatorClosedNeighborhood();
  void separatorClosedNeighborhoodThreshold();
  void createFamilies();
  void displayFamilies();
  void displaySeparator();
  void displaySeparatorAlign();
  
private:
  Separators();
  std::vector< nodes > & listInput() {return subGraph->getInputGraph()->getList();};
  std::map< int, int > & numSub() {return  subGraph->getNumInputToSub();};
  std::vector< int > &numInput() {return subGraph->getNumSubToInput();}
  
};

#endif
