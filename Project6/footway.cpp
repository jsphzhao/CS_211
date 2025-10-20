/*footway.cpp*/

//
// A footway in the Open Street Map.
// 

#include "footway.h"

using namespace std;

//
// constructor
//
Footway::Footway(long long id, const std::vector<long long>& nodeIDs)
    : ID(id), NodeIDs(nodeIDs) {
}

//
// adds the given nodeid to the end of the vector.
//
void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}