/*footways.cpp*/
//
// A collection of buildings in the Open Street Map.
//
// Joseph Zhao
// Northwestern University
// CS 211
//

#include "footways.h"
#include "tinyxml2.h"
#include "building.h"
#include "osm.h"

#include <cassert>
#include <algorithm>

using namespace std;

//
// extractNodeIDsFromWay
//
vector<long long> Footways::extractNodeIDsFromWay(XMLElement* way) {
    vector<long long> nodeIDs;
    for (XMLElement* nd = way->FirstChildElement("nd"); nd != nullptr; nd = nd->NextSiblingElement("nd")) {
        long long nodeID = nd->FindAttribute("ref")->Int64Value();
        nodeIDs.push_back(nodeID);
    }
    return nodeIDs;
}

//
// readMapFootways
// Input footways from a file
//
void Footways::readMapFootways(XMLDocument& xmldoc) {
    // Access the root element 'osm'
    XMLElement* root = xmldoc.FirstChildElement("osm");
    if (root == nullptr) return;

    // Iterate through each 'way' element within the 'osm' root
    for (XMLElement* way = root->FirstChildElement("way"); way != nullptr; way = way->NextSiblingElement("way")) {
        // Check if this way is a footway by looking for specific key-value pairs
        if (osmContainsKeyValue(way, "highway", "footway") || osmContainsKeyValue(way, "area:highway", "footway")) {
            // Extract the ID of the footway
            long long id = way->FindAttribute("id")->Int64Value();
            vector<long long> nodeIDs;

            nodeIDs = extractNodeIDsFromWay(way);

            // Create a Footway object with the extracted ID and node IDs, and add it to MapFootways
            MapFootways.emplace_back(id, nodeIDs);
        }
    }
}

//
// getNumMapFootways
// 
int Footways::getNumMapFootways() const {
    return (int) this->MapFootways.size();
}

//
// intersects
//
bool Footways::intersects(const Footway& footway, const vector<long long>& buildingNodeIDs) const {
    for (const auto& nodeID : footway.NodeIDs) {
        if (find(buildingNodeIDs.begin(), buildingNodeIDs.end(), nodeID) != buildingNodeIDs.end()) {
            return true; // Intersection found
        }
    }
    return false; // No intersection found
}

//
// getIntersectingFootwayIDs
//
vector<long long> Footways::getIntersectingFootwayIDs(const vector<long long>& buildingNodeIDs) const {
    vector<long long> intersectingIDs;
    for (const auto& footway : this->MapFootways) {
        if (intersects(footway, buildingNodeIDs)) {
            intersectingIDs.push_back(footway.ID);
        }
    }
    sort(intersectingIDs.begin(), intersectingIDs.end());
    return intersectingIDs;
}

//
// print
//
void Footways::print(vector<long long> intersectingFootwayIDs) const {
    for (auto id : intersectingFootwayIDs) {
                cout << " Footway " << id << endl;
            }
}