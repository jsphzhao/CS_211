/*busstop.cpp*/

//
// A busstop in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include <string>
#include <utility>
#include <iostream>

#include "busstop.h"
#include "curl_util.h"
#include "json.hpp"


using json = nlohmann::json;
using namespace std;

//
// default constructor
//
BusStop::BusStop(){}

//
// constructor
//
BusStop::BusStop(int id, int route, string name, string dir, string loc, pair<double, double> pos)
    : stopID(id), route(route), stopName(name), direction(dir), location(loc), position(pos) {
}

//
// print
//
void BusStop::print() const {
    cout << stopID << ": "
         << "bus " << route << ", "
         << stopName << ", "
         << direction << ", "
         << location << ", "
         << "location (" << position.first << ", " << position.second << ")" 
         << endl;
}

//
// getStopID
//
int BusStop::getStopID() const {
    return stopID; 
}

//
// getRoute
//
int BusStop::getRoute() const {
    return route;
}

//
// getStopName
//
string BusStop::getStopName() const {
    return stopName;
}

//
// getDirection
//
string BusStop::getDirection() const {
    return direction; 
}

//
// getLocation
//
string BusStop::getLocation() const {
    return location;
}

//
// getPosition
//
pair<double, double> BusStop::getPosition() const {
    return position; 
}

//
// printBusPredictions
//
void BusStop::printBusPredictions(CURL* curl, int routeNumber, int stopId) const {
    string baseUrl = "http://ctabustracker.com/bustime/api/v2/getpredictions";
    string apiKey = "92LZMGx5TU8LbAHmMSdsKceeY";
    string url = baseUrl + "?key=" + apiKey + "&rt=" + to_string(routeNumber) + "&stpid=" + to_string(stopId) + "&format=json";

    string response;
    bool success = callWebServer(curl, url, response);

    if (!success) {
        cout << "  <<bus predictions unavailable, call failed>>" << endl;
        return;
    }

    auto jsonData = json::parse(response);
    auto busResponse = jsonData["bustime-response"];
    auto predictions = busResponse["prd"];

    if (predictions.empty()) {
        cout << "  <<no predictions available>>" << endl;
        return;
    }

    for (auto& M : predictions) {
        try {
            cout << "  vehicle #" << stoi(M["vid"].get_ref<std::string&>()) 
                 << " on route " << M["rt"].get_ref<std::string&>()
                 << " travelling " << M["rtdir"].get_ref<std::string&>()
                 << " to arrive in " << stoi(M["prdctdn"].get_ref<std::string&>()) << " mins" << endl;
        }
        catch (const std::exception& e) {
            cout << "  error" << endl;
            cout << "  malformed CTA response, prediction unavailable"
                 << " (error: " << e.what() << ")" << endl;
        }
    }
}