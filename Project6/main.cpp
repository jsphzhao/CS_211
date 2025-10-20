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
#include "footways.h"

using namespace std;
using namespace tinyxml2;

int main() {
    cout << "** NU open street map **" << endl;
    cout << endl;

    // Prompt for and load OSM file
    string filename;
    cout << "Enter map filename> " << endl;
    getline(cin, filename);

    XMLDocument doc;
    if (osmLoadMapFile(filename, doc) != true) {
        cout << "Error loading OSM file.\n";
        return 0;
    }

    // Read and display nodes
    Nodes nodes;
    nodes.readMapNodes(doc);
    cout << "# of nodes: " << nodes.getNumMapNodes() << endl;

    // Read and display buildings
    Buildings buildings;
    buildings.readMapBuildings(doc);
    cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;

    // Read and display footways
    Footways footways;
    footways.readMapFootways(doc);
    cout << "# of footways: " << footways.getNumMapFootways() << endl;

    cout << endl;
    
    // Interactive command loop
    string command;
    while (true) {
        cout << "Enter building name (partial or complete), or * to list, or $ to end> " << endl;
        getline(cin, command);
        if (command == "$") { // Exit command
            break;
        }
        else if (command == "*") { // Display buildings command
            buildings.print();
        }
        else {
            buildings.findAndPrint(command, nodes, footways);
        }
    }
    cout << "** Done **" << endl;
    // cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
    // cout << "# of nodes created: " << Node::getCreated() << endl;
    // cout << "# of nodes copied: " << Node::getCopied() << endl;
    return 0;
}