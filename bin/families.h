/*
 *  connectedComponent.h
 *  MosaicFinder
 *
 *  Created by romain pogorelcnik on 17/08/11.
 *  Copyright 2011 ISIMA. All rights reserved.
 *
 */

#ifndef Families_h
#define Families_h

#include <iostream>
#include <set>

class Families
{
private:
  std::set < int > familyNodes;
  
public:
  std::set< int > & getFamilyNodes() {return familyNodes;};
};

#endif
