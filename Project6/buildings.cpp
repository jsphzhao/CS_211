/*buildings.cpp*/
//
// A collection of buildings in the Open Street Map.
//
// Joseph Zhao
// Northwestern University
// CS 211
//

#include "buildings.h"
#include "tinyxml2.h"
#include "osm.h"
#include "footways.h"

#include <cassert>

//
// readMapBuildings
//
// Parses an XML document to extract and store all buildings identified
// as "university" buildings within the OpenStreetMap data
//
void Buildings::readMapBuildings(XMLDocument& xmldoc) {
    XMLElement* osm = xmldoc.FirstChildElement("osm"); // Ensure we start from the root element
    XMLElement* way = osm->FirstChildElement("way");
    while (way != nullptr) {
        // Check if the current way element is a university building
        if (osmContainsKeyValue(way, "building", "university")) {
            long long id = stoll(way->Attribute("id"));
            string name = osmGetKeyValue(way, "name");
            string streetAddr = osmGetKeyValue(way, "addr:housenumber") + " " + osmGetKeyValue(way, "addr:street");

            // Initialize the Building object with the collected data
            Building B(id, name, streetAddr);

            // Iterate over nd elements to collect node references
            XMLElement* nd = way->FirstChildElement("nd");
            while (nd != nullptr) {
                const XMLAttribute* ndref = nd->FindAttribute("ref");
                assert(ndref != nullptr); // Ensures the reference attribute exists

                long long id = ndref->Int64Value();

                B.add(id); // Add node ID to the Building object

                nd = nd->NextSiblingElement("nd"); // Move to the next node reference
            }

            // Add the fully initialized Building object to the MapBuildings vector
            MapBuildings.push_back(B);
        }
        way = way->NextSiblingElement("way"); // Move to the next way element
    }
}

int Buildings::getNumMapBuildings() const {
    return (int) this->MapBuildings.size();
}

//
// print
// 
void Buildings::print() const {
    for (const auto& building : MapBuildings) {
        cout << building.ID << ": " << building.Name << ", " << building.StreetAddress << endl;
    }
}

//
// findAndPrint
//
void Buildings::findAndPrint(const string& command, const Nodes& nodes, const Footways& footways) const {
    bool found = false;

    for (const auto& building : MapBuildings) {
        if (building.Name.find(command) != string::npos) {
            found = true;
            building.print(nodes);

            vector<long long> buildingNodeIDs = building.NodeIDs;
            vector<long long> intersectingFootwayIDs = footways.getIntersectingFootwayIDs(buildingNodeIDs);

            cout << "Footways that intersect: " << intersectingFootwayIDs.size() << endl;
            if (intersectingFootwayIDs.size() == 0)
            {
                cout << " None" << endl;
            }
            footways.print(intersectingFootwayIDs);
        }
    }

    if (!found) {
        cout << "No such building" << endl; // If no building matched the command
    }
}