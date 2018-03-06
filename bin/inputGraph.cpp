/*
 *  inputGraph.cpp
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */

#include "inputGraph.h"
#include "subGraph.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cfloat>


using namespace std;

InputGraph::InputGraph()
{
}

InputGraph::InputGraph(string filename, float evalueT, float similarityT, float evalueStrictT, float similarityStrictT )
{  
  evalT =  evalueT;
  pidentT = similarityT;
  alignOption =false;
  map<string,int>::iterator itmap, itmap2, itmap3, itmap4;
  map<string,int> valueMap;
  vector<edges>::iterator itvecedge;
  map<string,int> stockage_nom;
  pair<map<string,int>::iterator,bool> ret, ret2;
  vector<string> valuevec;
  unsigned int sstart,send, qstart, qend;
  bool header = true;
  int compt, wn1, wn2;
  compt = wn1 = wn2 = sstart = send = qstart = qend =0;
  
  ifstream fileIn(filename.c_str(), ios::in);
  if(fileIn.is_open())
	{
    string line,val,chaine,chaine2;
    float sim = 0;
    float eval = 0;
    istringstream iss;
    
    getline (fileIn, line);
    if ( line.empty() )
      throw string ("invalid file format!"); 
    
    iss.str(line);
    iss.clear();
    int valueNumber = 0;
    
    while ( getline(iss,val,'\t') )
	  {
      valueMap[val] = valueNumber++;
      valuevec.push_back(val);
	  }
    
    int linenumber = 1;
    
    itmap = valueMap.find("sseqid");	itmap2 = valueMap.find("qseqid");
    if ( (itmap == valueMap.end() || itmap2 == valueMap.end()) && valuevec.size() == 12) 
	  {
      header = false;
      alignOption = true;
      chaine = valuevec[0]; chaine2 = valuevec[1]; 
      iss.str(valuevec[10]); iss.clear(); iss >> eval;
      iss.str(valuevec[2]); iss.clear(); iss >> sim;  
      iss.str(valuevec[8]); iss.clear(); iss >> sstart;
      iss.str(valuevec[9]); iss.clear(); iss >> send;
      iss.str(valuevec[6]); iss.clear(); iss >> qstart; 
      iss.str(valuevec[7]); iss.clear(); iss >> qend; 
      cout << "gene_id\tfusion_event\tfamily_number\tstart_med_align1\tend_med_align1\tstart_med_align2\tend_med_align2\tbreak_point\n";
	  }
    
    else if (itmap == valueMap.end() || itmap2 == valueMap.end())
      throw string ("invalid file format!"); 
    
    else
	  {
      itmap = valueMap.find("evalue");
      if ( itmap == valueMap.end() && ( (10 - evalueT) > FLT_EPSILON ) )
        throw string ("evalue not found!");
      
      itmap = valueMap.find("pident");
      if ( itmap == valueMap.end() && ( (similarityT) > FLT_EPSILON ) )
        throw string ("pident not found!");
      
      itmap = valueMap.find("sstart"); itmap2 = valueMap.find("qstart"); itmap3 = valueMap.find("send"); itmap4 = valueMap.find("qend");
      if ( itmap == valueMap.end() || itmap2 == valueMap.end() || itmap3 == valueMap.end() || itmap4 == valueMap.end() )
      {
        cerr << "No alignment found !\n";
        cout << "gene_id\tfusion_event\tfamily_number\n";
      }
      else
      { 
        alignOption = true;
        cout << "gene_id\tfusion_event\tfamily_number\tstart_med_align1\tend_med_align1\tstart_med_align2\tend_med_align2\tbreak_point\n";
      } 
	  }
    
    valuevec.clear();
    
    while (getline (fileIn, line)) 
	  { 
      if ( line.empty() )
        break;
      
      linenumber++;
      ostringstream oss;
      oss << linenumber;
      string numString = ("error at line ");
      numString += oss.str();
      nodes n1,n2;
      iss.str(line);
      iss.clear();
      
      
      while (getline(iss,val,'\t') )
        valuevec.push_back(val);
      
      if ( header )
		  {
        
        if ( valuevec.size() != valueMap.size() )
          throw string (numString);
        
        chaine = valuevec[valueMap["qseqid"]];
        chaine2 = valuevec[valueMap["sseqid"]]; 
        
        itmap = valueMap.find("evalue");
        if ( itmap != valueMap.end() )
        {
          iss.str(valuevec[valueMap["evalue"]]);
          iss.clear();
          iss >> eval;
        }
        itmap = valueMap.find("pident");
        if ( itmap != valueMap.end() )
        {
          iss.str(valuevec[valueMap["pident"]]);
          iss.clear();
          iss >> sim;  
        }
        itmap = valueMap.find("sstart");      
        if ( itmap != valueMap.end() ) 
        {
          iss.str(valuevec[valueMap["sstart"]]);
          iss.clear();
          iss >> sstart;
        }
        itmap = valueMap.find("send");      
        if ( itmap != valueMap.end() ) 
        {
          iss.str(valuevec[valueMap["send"]]);
          iss.clear();
          iss >> send; 
        }
        itmap = valueMap.find("qstart");      
        if ( itmap != valueMap.end() ) 
        {
          iss.str(valuevec[valueMap["qstart"]]);
          iss.clear();
          iss >> qstart; 
        }
        itmap = valueMap.find("qend");      
        if ( itmap != valueMap.end() ) 
        {
          iss.str(valuevec[valueMap["qend"]]);
          iss.clear();
          iss >> qend; 
        }
        valuevec.clear();
		  }
      
      else
		  {
        chaine = valuevec[0]; chaine2 = valuevec[1]; 
        iss.str(valuevec[10]); iss.clear(); iss >> eval;
        iss.str(valuevec[2]); iss.clear(); iss >> sim;  
        iss.str(valuevec[8]); iss.clear(); iss >> sstart;
        iss.str(valuevec[9]); iss.clear(); iss >> send;
        iss.str(valuevec[6]); iss.clear(); iss >> qstart; 
        iss.str(valuevec[7]); iss.clear(); iss >> qend; 
        valuevec.clear();
		  }
      
      if ( sim < similarityT || eval > evalueT )
        continue;
      
      ret = stockage_nom.insert( pair<string,int>(chaine,compt) );
      if (ret.second==true)
		  {
        n1.name = chaine;
        n1.degree = 1;
        list.push_back(n1);
        wn1 = compt;
        compt++;
		  }
      else 
		  {
        wn1 = ret.first->second;
        list[wn1].degree++;
		  }
      ret2 = stockage_nom.insert( pair<string,int>(chaine2,compt) );
      if (ret2.second==true)
		  {
        n2.name = chaine2;
        n2.degree = 1;
        list.push_back(n2);
        wn2 = compt;
        compt++;
		  }
      else 
		  {
        wn2 = ret2.first->second;
        list[wn2].degree++;
		  }
      
      if (ret.second == false && ret2.second == false)
		  {
        for (itvecedge = list[ret.first->second].neighbors.begin(); 
             itvecedge != list[ret.first->second].neighbors.end(); ++itvecedge)
        {
          if (itvecedge->tail == ret2.first->second && itvecedge->evalue > eval)
          {
            itvecedge->evalue = eval;
            itvecedge->similarity = sim;
            itvecedge->sstart = sstart;
            itvecedge->send =send;  
            itvecedge->qstart = qstart;
            itvecedge->qend = qend;             
          }
        }
        for (itvecedge = list[ret2.first->second].neighbors.begin(); 
             itvecedge != list[ret2.first->second].neighbors.end(); ++itvecedge)
        {
          if (itvecedge->tail == ret.first->second && itvecedge->evalue > eval)
          {
            itvecedge->evalue = eval;
            itvecedge->similarity = sim;
            itvecedge->sstart = sstart;
            itvecedge->send =send;  
            itvecedge->qstart = qstart;
            itvecedge->qend = qend;             
          }
        }
		  }  
      
      edges e1,e2;
      e1.tail = stockage_nom[chaine2];e2.tail = stockage_nom[chaine];
      e1.similarity = e2.similarity = sim;
      e1.evalue = e2.evalue = eval;
      e1.sstart = sstart; e2.sstart =qstart;
      e1.send = send; e2.send =qend;
      e1.qstart = qstart; e2.qstart =sstart;
      e1.qend = qend; e2.qend =send;
      list[wn1].neighbors.push_back(e1);
      list[wn2].neighbors.push_back(e2);
	  }
    num_of_nodes = stockage_nom.size();
    fileIn.close();
	}
  else
    throw string ("Cannot open file !"); 
}

void InputGraph::execute(float evalueT, float similarityT)
{
  vector< edges >::iterator it;
  int * visited = new int [num_of_nodes];
  int topNode;
  vector< int > ConnectedComponentNodes;
  stack< int > S;
  
  memset(visited, 0, num_of_nodes*sizeof(int));
  
  for (int i = 0; i < num_of_nodes ; ++i)
	{
    if (visited[i] != 0)
      continue;
    
    S.push(i);
    while(!S.empty())
	  {
      topNode = S.top();
      S.pop();
      if (visited[topNode] == 0)
      {
        visited[topNode] = 1;
        ConnectedComponentNodes.push_back(topNode);
        for (it = list[topNode].neighbors.begin(); it != list[topNode].neighbors.end(); ++it)
        {
          if (visited[it->tail] == 0 && it->evalue < evalueT && it->similarity > similarityT)
            S.push(it->tail);
        }
      }
	  }
    SubGraph sub(& ConnectedComponentNodes,this,evalueT,similarityT);
    sub.separators();
    ConnectedComponentNodes.clear();
	}
  delete [] visited;
}
