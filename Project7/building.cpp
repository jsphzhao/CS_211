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
#include <utility>

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
// getLocation
// gets the center (lat, lon) of the building based
// on the nodes that form the perimeter
//
pair<double, double> Building::getLocation(const Nodes& nodes) const {
    double avgLat = 0.0, avgLon = 0.0;
    bool isEntrance; 
    double lat, lon;

    //Loop through NodeIDs
    for (long long id : NodeIDs) {
        // If node is found
        if (nodes.find(id, lat, lon, isEntrance)) { 
            avgLat += lat;
            avgLon += lon;
        }
    }

    avgLat /= NodeIDs.size();
    avgLon /= NodeIDs.size();
    
    return make_pair(avgLat, avgLon);
}

//
// print
//
void Building::print(const Nodes& nodes) const {
    auto location = getLocation(nodes);
    cout << Name << endl
    << "Address: " << StreetAddress << endl
    << "Building ID: " << ID << endl
    << "# perimeter nodes: " << NodeIDs.size() << endl
    << "Location: (" << location.first << ", " << location.second << ")" << endl;


    // << "Nodes: " << NodeIDs.size() << endl;
    
    // for (const auto& nodeId : NodeIDs) {
    //     double lat, lon;
    //     bool isEntrance;
    //     if (nodes.find(nodeId, lat, lon, isEntrance)) {
    //         cout << " " << nodeId << ": (" << lat << ", " << lon << ")";
    //         if (isEntrance) cout << ", is entrance";
    //         cout << endl;
    //     } else {
    //         cout << "Node ID: " << nodeId << " ** NOT FOUND **" << endl;
    //     }
    // }
}