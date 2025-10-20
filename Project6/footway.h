/*footway.h*/

//
// A footway in the Open Street Map.
// 

#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Footway {
public:
    long long ID;
    vector<long long> NodeIDs;

    //
    // constructor 
    //
    Footway(long long id, const vector<long long>& nodeIDs);

    //
    // adds the given nodeid to the end of the vector.
    //
    void add(long long nodeid);
};