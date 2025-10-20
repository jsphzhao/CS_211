/*busstop.h*/

//
// A busstop in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#pragma once
#include <string>
#include <utility>
#include "curl_util.h"

using namespace std;

class BusStop {
private:
    int stopID;
    int route;
    string stopName;
    string direction;
    string location;
    pair<double, double> position;

public:
    //
    // default constructor
    //
    BusStop();

    //
    // constructor
    //
    BusStop(int id, int route, string name, string dir, string loc, pair<double, double> pos);

    //
    // print
    //
    void print() const;

    //
    // getStopID
    //
    int getStopID() const;

    //
    // getRoute
    //
    int getRoute() const;

    //
    // getStopName
    //
    string getStopName() const;

    //
    // getDirection
    //
    string getDirection() const;

    //
    // getLocation
    //
    string getLocation() const;

    //
    // getPosition
    //
    pair<double, double> getPosition() const;

    //
    //
    //
    void printBusPredictions(CURL* curl, int routeNumber, int stopId) const;
};