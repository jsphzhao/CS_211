/*buildings.h*/
//
// A collection of buildings in the Open Street Map.
//
// Joseph Zhao
// Northwestern University
// CS 211
//
// Original author: Prof. Joe Hummel
//

#pragma once

#include <vector>

#include "building.h"
#include "tinyxml2.h"
#include "footways.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

//
// Keeps track of all the buildings in the map.
//
class Buildings
{
public:
    vector <Building> MapBuildings;
    //
    // readMapBuildings
    //
    // Given an XML document, reads through the document and
    // stores all the buildings into the given vector.
    //
    void readMapBuildings(XMLDocument& xmldoc);

    //
    // accessors / getters
    //
    int getNumMapBuildings() const;

    //
    // print
    // 
    void print() const;

    //
    // findAndPrint
    //
    void findAndPrint(const string& input, const Nodes& nodes, const Footways& footways) const;

    //
    // getBuildingNodeIdsByName
    //
    vector<int> getBuildingNodeIdsByName(const string& buildingName) const;
};