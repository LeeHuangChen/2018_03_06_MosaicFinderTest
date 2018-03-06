/*
 *  subGraph.cpp
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */


#include "subGraph.h"
#include "inputGraph.h"
#include "separators.h"
#include <iostream>
#include <cstring>
using namespace std;
SubGraph::SubGraph()
{
}

SubGraph::SubGraph(vector< int >  * num_inputGraph, InputGraph * inGraphIn, float evalueT, float similarityT )
{
  
  inGraph = inGraphIn;
  numSubToInput = num_inputGraph;
  numOfNodes = num_inputGraph->size();
  matrix = new int * [numOfNodes];
  
  for (int i = 0; i < numOfNodes; ++i) 
  {
    numInputToSub[(*numSubToInput)[i]] = i;
    matrix[i] = new int[numOfNodes];
    memset(matrix[i], 0, numOfNodes*sizeof(int));
  }
  vector< edges >::iterator itedges;
  for (int i = 0; i < numOfNodes; ++i) 
  {
    for (itedges = inGraph->getList()[(*numSubToInput)[i]].neighbors.begin();
         itedges != inGraph->getList()[(*numSubToInput)[i]].neighbors.end(); ++itedges) 
    {
      if ( itedges->evalue < evalueT && itedges->similarity > similarityT)
        matrix[i][numInputToSub[itedges->tail]] = 1;
    }
  }
}

SubGraph::~SubGraph()
{
  if( matrix )
  {
    for (int i = 0; i < numOfNodes; ++i)
    {
     delete [] matrix[i];
    }
  delete [] matrix;
  }
}

void SubGraph::separators()
{

  int previousNodeLabel; 
  previousNodeLabel = 0;

  int * reached = new int [numOfNodes];
  int * label  = new int [numOfNodes];
  int * alpha = new int [numOfNodes];

  vector < int > newNeighbors, oldNeighbors;  
  
  set <set <int> > setSep;
  pair<set<set<int> >::iterator,bool> ret;

  memset(label, 0, sizeof(int)*numOfNodes);
  memset(alpha, 0, sizeof(int)*numOfNodes);

  vector < vector <int> > reach;
  reach.resize(numOfNodes);

  for( int i = numOfNodes; i > 0; --i )
  {
    int max = -1;
    int maxCardinalityNode = -1;

    for( int j = 0; j < numOfNodes; ++j )
    {
      if ( alpha[j] == 0 && label[j] > max )
      {
        max = label[j];
        maxCardinalityNode = j;
      }
    }

    newNeighbors.clear();
    oldNeighbors.clear();

    alpha[maxCardinalityNode] = i ;

    for( int j = 0; j < numOfNodes; ++j )
    {
      reached[j] = 0;      
    }
    for (int j = 0; j < numOfNodes; ++j) 
    {
      reach[j].clear();
    }
    reached[maxCardinalityNode] = 1;

    for ( int j = 0; j < numOfNodes; ++j )
    {
      if (alpha[j] != 0)
        continue;

      if (isEdge(maxCardinalityNode, j) )
      {
        reached[j] = 1;
        reach[label[j]].push_back(j);
        oldNeighbors.push_back(j);
      }
    }
    
    int y = 0; 
    for(int j = 0; j < numOfNodes; ++j)
    {
      while( !reach[j].empty() )
      {
        y = reach[j].back();
        reach[j].pop_back();
        for (int z = 0; z < numOfNodes; ++z)
        {
          if (alpha[z] != 0)
            continue;
          
          if ( isEdge(y, z) )
          {
            if ( reached[z] == 0 )
            {
              reached[z] = 1;
              if ( label[z] > j )
              {
                newNeighbors.push_back(z);
                reach[label[z]].push_back(z);
              }
              else
              {
                reach[j].push_back(z);
              }
            }
          }
        }
      }
    }
    for( int j = 0; j < (int)newNeighbors.size(); ++j )
    {
      addFill(newNeighbors[j],maxCardinalityNode);
      label[newNeighbors[j]] += 1;
    }
    
    for (int j = 0; j < (int)oldNeighbors.size(); ++j) 
    {
      label[oldNeighbors[j]] += 1;
    }
    
    if ( alpha[maxCardinalityNode] != numOfNodes ) 
    {
      if (label[maxCardinalityNode] <= previousNodeLabel)
      {
        Separators separator(this);
        for (int k = 0; k < numOfNodes; ++k) 
        {
          if (alpha[k] != 0 && isEdge(maxCardinalityNode, k)) 
          {
            separator.getSeparatorNodes().insert(k);
          }
        }
        ret = setSep.insert(separator.getSeparatorNodes());

        if (ret.second == true &&  isComplete( separator.getSeparatorNodes()))
        {
          separator.createFamilies();
          if ( inGraph->getAlignOption() )
            separator.displaySeparatorAlign(); 
          else
            separator.displaySeparator();
        }
        else
          setSep.insert(ret.first,separator.getSeparatorNodes());
      } 
    }  
    previousNodeLabel = label[maxCardinalityNode];
  }  
  delete[] reached;
  delete[] label;
  delete[] alpha;  
}

bool SubGraph::isComplete (set< int > & sep)
{  
  set< int >::iterator it,it2;
  it = sep.begin();
  
  while (it != sep.end())
  {
    it2 = it;
    ++it2;
    
    while (it2 != sep.end() ) {
      
      if ( isFill(*it, *it2))
        return false;
      
      it2++;
    }
    it++;
  }  
  return true;
}

bool SubGraph::isFill (int i, int j)
{
  if (matrix[i][j] == 2)
    return true;
  else
    return false;
}

bool SubGraph::isEdge (int i, int j)
{
  if (matrix[i][j] != 0)
    return true;
  else
    return false;  
}

void SubGraph::addFill(int a, int b)
{
	matrix [a][b] = 2;
	matrix [b][a] = 2;
}
