/*
 *  inputGraph.h
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */
#ifndef InputGraph_h
#define InputGraph_h

#include <map>
#include <vector>
#include <string>


typedef struct  {
  int tail;
  float evalue;
  float similarity;
  unsigned int sstart;
  unsigned int send;  
  unsigned int qstart;
  unsigned int qend; 
}edges;

typedef struct  {
  int degree;
  std::string name;
  std::vector < edges > neighbors;
}nodes;

class InputGraph
{  
private:
  int num_of_nodes;
  float pidentT;
  float evalT;
  bool alignOption;
  std::vector < nodes > list;
  
public:
  // Constructors
  InputGraph(std::string filename, float evalue, float similarity, float evalueStrict, float similarityStrict);
  
  // Getters
  std::vector< nodes > & getList() { return list; };
  float getEvalue() {return evalT;};
  float getSimilarity() {return pidentT;};
  bool getAlignOption(){return alignOption;};
  
  // Methods
  void displayList();
  void execute();
  void execute(float evalueT, float similarityT);
  
private:
  InputGraph();
};

#endif
