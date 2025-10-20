/*busstops.h*/

//
// A collection of busstops in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#pragma once
#include "busstop.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class BusStops {
private:
    vector<BusStop> busstops;

    //
    // readStopsHelper
    //
    BusStop readStopsHelper(const string& line) const;

public:
    //
    // constructor
    //
    BusStops(const string& filename);

    //
    // readStopsFromFile
    //
    void readStopsFromFile(const string& filename);

    //
    // print
    //
    void print() const;

    //
    // getNumBusStops
    //
    int getNumBusStops() const;

    //
    // findClosestStops
    //
    pair<BusStop, BusStop> findClosestStops(double lat, double lon) const;

    //
    // printClosestStops
    //
    void printClosestStops(pair<double, double> location, pair<BusStop, BusStop> closestStops, CURL* curl) const;
};