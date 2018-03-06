/*
 *  separator.cpp
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */

#include "separators.h"
#include "families.h"
#include <map>
#include <iostream>
#include <stack>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <cfloat>
using namespace std;

Separators::Separators( SubGraph * subGraphIn)
{
  subGraph = subGraphIn;
}

void Separators::separatorClosedNeighborhoodThreshold()
{
  set < int >::iterator it;
  vector< edges>::iterator it2;
  closedNeighborhood.clear();
  set < int > interSet;
  vector< int > inter(subGraph->getNumOfNodes()*2);
  vector< int >::iterator itInter;
  
  it = separatorNodes.begin();
  for ( it2 = listInput()[numInput()[*it]].neighbors.begin(); it2 != listInput()[numInput()[*it]].neighbors.end(); ++it2)
  { 
    if (it == separatorNodes.begin())
    {
      if ( it2->evalue < (float)subGraph->getEvalue() && it2->similarity > (float)subGraph->getSimilarity())
        closedNeighborhood.insert(numSub()[it2->tail]);
    }   
  }
  ++it;
  for (; it != separatorNodes.end(); ++it)
  {  
    interSet.clear();
    for ( it2 = listInput()[numInput()[*it]].neighbors.begin(); it2 != listInput()[numInput()[*it]].neighbors.end(); ++it2)    
    { 
      if ( it2->evalue < (float)subGraph->getEvalue() && it2->similarity > (float)subGraph->getSimilarity())
        interSet.insert(numSub()[it2->tail]);
    }      
    itInter = set_intersection(closedNeighborhood.begin(),closedNeighborhood.end(),interSet.begin(), interSet.end(), inter.begin());
    closedNeighborhood.clear();
    vector<int>::iterator itInterEl;
    for (itInterEl = inter.begin(); itInterEl != itInter; ++itInterEl )    
      closedNeighborhood.insert(*itInterEl);
  }  
}

void Separators::separatorClosedNeighborhood()
{
  set < int >::iterator it;  vector< edges>::iterator it2;
  closedNeighborhood.clear();
  set < int > interSet;
  vector< int > inter(subGraph->getNumOfNodes()*2);
  vector< int >::iterator itInter;
  
  it = separatorNodes.begin();
  for ( it2 = listInput()[numInput()[*it]].neighbors.begin(); it2 != listInput()[numInput()[*it]].neighbors.end(); ++it2)    
    closedNeighborhood.insert(numSub()[it2->tail]);
  
  ++it;
  
  for (; it != separatorNodes.end(); ++it)
  {
    interSet.clear();
    for ( it2 = listInput()[numInput()[*it]].neighbors.begin(); it2 != listInput()[numInput()[*it]].neighbors.end(); ++it2)    
      interSet.insert(numSub()[it2->tail]);
    
    itInter = set_intersection(closedNeighborhood.begin(),closedNeighborhood.end(),interSet.begin(),interSet.end(), inter.begin());
    closedNeighborhood.clear();
    vector<int>::iterator itInterEl;
    for (itInterEl = inter.begin(); itInterEl != itInter; ++itInterEl )
      closedNeighborhood.insert(*itInterEl);
  }
} 

void Separators::createFamilies()
{
  if ( (float)subGraph->getSimilarity() > FLT_EPSILON || (10 - (float)subGraph->getEvalue() > FLT_EPSILON) )
    this->separatorClosedNeighborhoodThreshold();
  else
    this->separatorClosedNeighborhood();
  
  // open neighborhood creation
  vector< int > diff(subGraph->getNumOfNodes());
  set< int >::iterator neigh, neigh2;
  vector< edges >::iterator it;
  
  int * visited = new int [subGraph->getNumOfNodes()];
  
  int u;
  stack< int > S;
  memset(visited, 0, sizeof(int) * subGraph->getNumOfNodes());
  for ( neigh = closedNeighborhood.begin(); neigh != closedNeighborhood.end(); ++neigh)
  { 
    if (visited[*neigh] != 0)
      continue;
    
    Families family;
    S.push(*neigh);
    while(!S.empty())
    {
      u = S.top();
      S.pop();
      if (visited[u] == 0)
      {        
        visited[u] = 1;
        family.getFamilyNodes().insert(u);
        
        for (it = listInput()[numInput()[u]].neighbors.begin();
             it != listInput()[numInput()[u]].neighbors.end(); ++it)
        {           
          for( neigh2 = closedNeighborhood.begin(); neigh2 != closedNeighborhood.end(); ++neigh2 )
          {
            if ( numSub()[it->tail] == *neigh2 && visited[numSub()[it->tail]] == 0)
              S.push(numSub()[it->tail]);
            
          }
        } 
      }  
    }
    separatorFamilies.push_back(family);
  }  
  delete [] visited;
}

void Separators::displaySeparator()
{
  if (separatorFamilies.size() > 1)
  {    
    static int static_cmp = 0;
    set<int>::iterator it3;
    vector<Families>::iterator it, it2;
    for ( it = separatorFamilies.begin() ; it != separatorFamilies.end(); ++it)
    {
      it2 = it; ++it2;
      for ( ; it2 != separatorFamilies.end(); ++it2)
      {
        for (it3 =separatorNodes.begin();it3 != separatorNodes.end();++it3) 
          cout << listInput()[numInput()[*it3]].name << "\t" << static_cmp<<"\t"<< '0' << "\t" << endl;	
        for (it3 = it->getFamilyNodes().begin() ; it3 != it->getFamilyNodes().end(); ++it3)  
          cout<< listInput()[numInput()[*it3]].name << "\t"<< static_cmp<<"\t"<< '1' << endl;
        for (it3 = it2->getFamilyNodes().begin() ; it3 != it2->getFamilyNodes().end(); ++it3)  
          cout<< listInput()[numInput()[*it3]].name << "\t" << static_cmp << "\t" << '2' << endl;        
      }
    }
    static_cmp++;
  }
}

void Separators::displaySeparatorAlign()
{
  bool match = false;
  vector<Families>::iterator it, it2;
  set<int>::iterator it3,it5,isneighb;
  vector<edges>::iterator it4;
  vector<int> startVec;
  vector<int> endVec;
  vector<bool> sepNodeCompatible;
  vector<int *> sepNodeMedian;
  int startMedianF1, startMedianF2, endMedianF1, endMedianF2, breakPoint;
  startMedianF1 = startMedianF2 = endMedianF1 = endMedianF2 = breakPoint =0;
  static int static_cmp = 0;
  if (separatorFamilies.size() > 1)
  {
    for ( it = separatorFamilies.begin() ; it != separatorFamilies.end(); ++it)
    {
      it2 = it; ++it2;
      for ( ; it2 != separatorFamilies.end(); ++it2)
      {
        match = false;
        it3 =separatorNodes.begin();
        sepNodeCompatible.clear();
        sepNodeMedian.clear();
        while (it3 != separatorNodes.end()) 
        {
          startVec.clear();endVec.clear();
          for (it4 = listInput()[numInput()[*it3]].neighbors.begin(); it4 != listInput()[numInput()[*it3]].neighbors.end(); ++it4)
          {
            isneighb = it->getFamilyNodes().find(numSub()[it4->tail]);
            if ( isneighb ==  it->getFamilyNodes().end() )
              continue;
            
            if (it4->qstart < it4->qend)
            {
              startVec.push_back(it4->qstart);
              endVec.push_back(it4->qend);
            }
            else
            {
              startVec.push_back(it4->qend);
              endVec.push_back(it4->qstart);
            }
          }
          sort (startVec.begin(), startVec.end());
          sort  (endVec.begin(), endVec.end());   
          int pos = (int) startVec.size();
          
          if ( pos % 2 == 0 )
          {
            pos = pos/2;
            startMedianF1 = ( startVec[pos-1] + startVec[pos]) / 2;
            endMedianF1 = ( endVec[pos-1] + endVec[pos]) / 2;
          }
          else
          {
            startMedianF1 = startVec[floor((float)pos/2)];
            endMedianF1 = endVec[floor((float)pos/2)];
          }
          
          
          startVec.clear();endVec.clear();
          for (it4 = listInput()[numInput()[*it3]].neighbors.begin(); it4 != listInput()[numInput()[*it3]].neighbors.end(); ++it4)
          {
            isneighb = it2->getFamilyNodes().find(numSub()[it4->tail]);
            if ( isneighb ==  it2->getFamilyNodes().end() )
              continue;
            
            if (it4->qstart < it4->qend)
            {
              startVec.push_back(it4->qstart);
              endVec.push_back(it4->qend);
            }
            else
            {
              startVec.push_back(it4->qend);
              endVec.push_back(it4->qstart);
            }
          } 
          
          sort (startVec.begin(), startVec.end());
          sort (endVec.begin(), endVec.end());
          pos = (int) startVec.size();
          if ( pos % 2 == 0)
          {
            pos = pos/2;	
            startMedianF2 = ( startVec[pos-1] + startVec[pos]) / 2;
            endMedianF2 = ( endVec[pos-1] + endVec[pos]) / 2;
          }
          else 
          {
            startMedianF2 = startVec[floor((float)pos/2)];
            endMedianF2 = endVec[floor((float)pos/2)];
          }
          
          if (endMedianF1 <  (startMedianF2 + 20)  ||  endMedianF2 <  (startMedianF1 + 20) )
          {
            match= true;
            sepNodeCompatible.push_back(true);
            int * medians;
            medians = new int[4];
            medians[0] = startMedianF1; medians[1] = endMedianF1; medians[2] = startMedianF2; medians[3] = endMedianF2;
            sepNodeMedian.push_back(medians);
          }
          else
            sepNodeCompatible.push_back(false);
          
          ++it3;
        }
        if (match)
        {
          int i = 0;
          int j = 0;
          for (it5 = separatorNodes.begin(); it5 != separatorNodes.end() ; ++it5) 
          {
            if (sepNodeCompatible[i])
            { 
              if (sepNodeMedian[j][0] < sepNodeMedian[j][2])
                breakPoint = (sepNodeMedian[j][1] + sepNodeMedian[j][2])/2;
              else 
                breakPoint = (sepNodeMedian[j][3] + sepNodeMedian[j][0])/2;
              
              cout << listInput()[numInput()[*it5]].name << "\t" << static_cmp<<"\t"<< '0' << "\t" << sepNodeMedian[j][0] << "\t" << sepNodeMedian[j][1] << "\t" << sepNodeMedian[j][2] << "\t" << sepNodeMedian[j][3] << "\t" << breakPoint << "\n";
              delete[] sepNodeMedian[j];
              j++;
            }
            i++;
          }
          for (it5 = it->getFamilyNodes().begin() ; it5 != it->getFamilyNodes().end(); ++it5)  
            cout<< listInput()[numInput()[*it5]].name << "\t"<< static_cmp<<"\t"<< '1' << endl;
          for (it5 = it2->getFamilyNodes().begin() ; it5 != it2->getFamilyNodes().end(); ++it5)  
            cout<< listInput()[numInput()[*it5]].name << "\t" << static_cmp << "\t" << '2' << endl;
        }
      }
    }
    if (match)
      static_cmp++;
  }
}
