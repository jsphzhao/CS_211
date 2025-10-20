/*nodes.cpp*/

//
// A collection of nodes in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 
// References:
// 
// TinyXML: 
//   files: https://github.com/leethomason/tinyxml2
//   docs:  http://leethomason.github.io/tinyxml2/
// 
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// readMapNodes
//
// Given an XML document, reads through the document and 
// stores all the nodes into the given vector. Each node
// is a point on the map, with a unique id along with 
// (lat, lon) position. Some nodes are entrances to buildings,
// which we capture as well.
//
void Nodes::readMapNodes(XMLDocument& xmldoc)
{
  XMLElement* osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document node by node: 
  //
  XMLElement* node = osm->FirstChildElement("node");

  while (node != nullptr)
  {
    const XMLAttribute* attrId = node->FindAttribute("id");
    const XMLAttribute* attrLat = node->FindAttribute("lat");
    const XMLAttribute* attrLon = node->FindAttribute("lon");

    assert(attrId != nullptr);
    assert(attrLat != nullptr);
    assert(attrLon != nullptr);

    long long id = attrId->Int64Value();
    double latitude = attrLat->DoubleValue();
    double longitude = attrLon->DoubleValue();

    //
    // is this node an entrance? Check for a 
    // standard entrance, the main entrance, or
    // one-way entrance.
    //
    bool entrance = false;

    if (osmContainsKeyValue(node, "entrance", "yes") ||
      osmContainsKeyValue(node, "entrance", "main") ||
      osmContainsKeyValue(node, "entrance", "entrance"))
    {
      entrance = true;
    }

    //
    // Add node to vector:
    // 
    // This creates an object then pushes copy into vector:
    //
    this->MapNodes.emplace_back(id, latitude, longitude, entrance);


    //
    // next node element in the XML doc:
    //
    node = node->NextSiblingElement("node");
  }
}

//
// find
// 
// Searches the nodes for the one with the matching ID, returning
// true if found and false if not. If found, a copy of the node 
// is returned via the node parameter, which is passed by reference.
//
bool Nodes::find(long long id, double& lat, double& lon, bool& isEntrance) const
{
  // //
  // // linear search:
  // //
  // for (const Node& N : this->MapNodes)
  // {
  //   if (N.getID() == id) {
  //     lat = N.getLat();
  //     lon = N.getLon();
  //     isEntrance = N.getIsEntrance();

  //     return true;
  //   }
  // }

  int low = 0;
  int high = (int)this->MapNodes.size()- 1;

  while(low <= high) {
  int mid = low + ((high-low) / 2);
  long long nodeid = this->MapNodes[mid].getID();

  if (id == nodeid) { // found!
    lat = this->MapNodes[mid].getLat();
    lon = this->MapNodes[mid].getLon();
    isEntrance = this->MapNodes[mid].getIsEntrance();

    return true;
  }
  else if (id < nodeid) { // search left:
    high = mid - 1;
  }
  else { // search right:
    low = mid + 1;
  }
}//while

  //
  // if get here, not found:
  //
  return false;
}

//
// accessors / getters
//
int Nodes::getNumMapNodes() const {
  return (int) this->MapNodes.size();
}
