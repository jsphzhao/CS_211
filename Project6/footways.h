/*footways.h*/
//
// A collection of buildings in the Open Street Map.
//
// Joseph Zhao
// Northwestern University
// CS 211
//

#pragma once

#include <vector>

#include "footway.h"
#include "tinyxml2.h"
#include <iostream>

using namespace std;
using namespace tinyxml2;

class Footways {
public:

    vector<Footway> MapFootways; // Vector of Footway objects

    //
    // extractNodeIDsFromWay
    //
    vector<long long> extractNodeIDsFromWay(XMLElement* way);

    //
    // readMapFootways
    //
    void readMapFootways(XMLDocument& xmldoc);

    //
    // getNumMapFootways
    //
    int getNumMapFootways() const;

    //
    // intersects
    //
    bool intersects(const Footway& footway, const vector<long long>& buildingNodeIDs)const;

    //
    // getIntersectingFootwayIDs
    //
    vector<long long> getIntersectingFootwayIDs(const vector<long long>& buildingNodeIDs) const;

    //
    // print
    //
    void print(vector<long long> intersectingFootwayIDs) const;
};