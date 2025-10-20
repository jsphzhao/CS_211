/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include "building.h"
#include "nodes.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//
// print
//
void Building::print(const Nodes& nodes) const {
    cout << Name << endl
    << "Address: " << StreetAddress << endl
    << "Building ID: " << ID << endl
    << "Nodes: " << NodeIDs.size() << endl;
    
    for (const auto& nodeId : NodeIDs) {
        double lat, lon;
        bool isEntrance;
        if (nodes.find(nodeId, lat, lon, isEntrance)) {
            cout << " " << nodeId << ": (" << lat << ", " << lon << ")";
            if (isEntrance) cout << ", is entrance";
            cout << endl;
        } else {
            cout << "Node ID: " << nodeId << " ** NOT FOUND **" << endl;
        }
    }
}