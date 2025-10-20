/*main.cpp*/
//
// Joseph Zhao
// Northwestern University
// CS 211
//

#include <iostream>
#include <string>

#include "building.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "buildings.h"

using namespace std;
using namespace tinyxml2;

// searchAndDisplayBuildings
// Helper function to search through all the buildings and output each building whose name contains the user’s input
void searchAndDisplayBuildings(const string& command, const Buildings& buildings, const Nodes& nodes) {
    for (const auto& building : buildings.MapBuildings) {
        if (building.Name.find(command) != string::npos) { // If part of the name matches the user input
            cout << building.Name << endl
                 << "Address: " << building.StreetAddress << endl
                 << "Building ID: " << building.ID << endl
                 << "Nodes: " << endl;
            // Output each node's information
            for (const auto& nodeId : building.NodeIDs) {
                double lat, lon;
                bool isEntrance;
                if (nodes.find(nodeId, lat, lon, isEntrance)) {
                    cout << "   " << nodeId << ": (" << lat << ", " << lon << ")";
                    if (isEntrance) cout << ", Entrance";
                    cout << endl;
                } else {
                    cout << "Node ID: " << nodeId << " ** NOT FOUND **" << endl;
                }
            }
        }
    }
}

int main() {
    // Prompt for and load OSM file
    string filename;
    cout << "Enter the name of an OSM file: " << endl;
    getline(cin, filename);

    tinyxml2::XMLDocument doc;
    if (osmLoadMapFile(filename, doc) != true) {
        cout << "Error loading OSM file.\n";
        return 0;
    }

    // Read and display nodes
    Nodes nodes;
    nodes.readMapNodes(doc);
    cout << "Number of nodes: " << nodes.getNumMapNodes() << endl;

    // Read and display buildings
    Buildings buildings;
    buildings.readMapBuildings(doc);
    cout << "Number of buildings: " << buildings.getNumMapBuildings() << endl;
    
    // Interactive command loop
    string command;
    while (true) {
        cout << "Enter building name (partial or complete), or * to list, or $ to end> " << endl;
        getline(cin, command);
        if (command == "$") { // Exit command
            break;
        }
        else if (command == "*") { // Display buildings command
            cout << "*" << endl;
            for (const auto& building : buildings.MapBuildings) {
                cout << building.ID << ": " << building.Name << ", " << building.StreetAddress << endl;
            }
        }
        else {
            searchAndDisplayBuildings(command, buildings, nodes);
        }
    }
    cout << "** Done **" << endl;
    cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
    cout << "# of nodes created: " << Node::getCreated() << endl;
    cout << "# of nodes copied: " << Node::getCopied() << endl;
    return 0;
}