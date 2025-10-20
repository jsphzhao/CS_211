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