/*
Bhavana Chemuturi
3345.502

        Program Description:
        --------------------
        //Semester Project - Plan a Flight
        This program determines feasible flight plans and calculates the total cost and time
        for airline routes between cities using input flight data and requested flight plans.
        It employs an array of linked lists to represent the flight network as a directed graph and
        utilizes an iterative backtracking algorithm with a stack to explore all possible paths.
        The output includes up to three optimal flight plans sorted by cost or time, or an error
        message if no viable plan exists.
        ---------------------

        Changelog:
        ---------
        11/24/24: Created file and wrote program description + started changelog
            Read and extract data from both files --> Finished readFiles() & processData()
            Created Linked List structure
        11/25/24: Figure out how to process each line for input
            Created Arrays of Linkedlists from Flight Data and implemented it bidirectionally
            To do: Process Requested Flight Data and calculate routes
            - Change struct LinkedList into a different class.
        11/26/24: Figured out how to calculate route. Tweak output to match assignment
        11/27/24: Finished program and outputting onto file.
        ----------
*/

#include <iostream> // cout and cin
#include <fstream>  // for file handling
#include <string>   // for strings
#include <sstream>
#include <vector>   // for vectors
#include <algorithm> // For std::find
#include "LinkedList.cpp"
#include "Stack.cpp"

using namespace std;

//--------------------Program Starts--------------------//

// Global variables
string inputFile;
string requestFile;
ifstream flightDataInput;
ifstream requestedFlightsInput;
int flightData;
int pathCount = 0;
int requestedPaths;
LinkedList* flightLists = nullptr;
int size;
string outputFileName;


// Structure to store path information
struct PathInfo {
    vector<string> path;  // The actual path
    float cost;           // Total cost of the path
    int duration;         // Total duration of the path

    // Constructor
    PathInfo(vector<string> p, float c, int d) : path(p), cost(c), duration(d) {}
};

vector<PathInfo> allPaths; // Global vector to store all paths

// Use stack for pathfinding
void findPathWithStack(string startCity, string endCity) {
    // Define a custom stack-based state
    struct State {
        LinkedList::Node* currentNode;
        vector<string> currentPath;
        float currentCost;
        int currentDuration;

        State(LinkedList::Node* node, vector<string> path, float cost, int duration)
            : currentNode(node), currentPath(path), currentCost(cost), currentDuration(duration) {}
    };

    Stack<State*> stateStack; // Use the modified Stack class

    // Find the start node
    LinkedList::Node* startNode = nullptr;
    for (int i = 0; i < size; ++i) {
        if (flightLists[i].head && flightLists[i].head->city == startCity) {
            startNode = flightLists[i].head;
            break;
        }
    }

    // If the start city is not found, return
    if (!startNode) return;

    // Push the initial state onto the stack
    stateStack.push(new State(startNode, {startCity}, 0, 0));

    while (!stateStack.isEmpty()) {
        // Pop the current state from the stack
        State* currentState = stateStack.pop();

        LinkedList::Node* currentNode = currentState->currentNode;
        vector<string> currentPath = currentState->currentPath;
        float currentCost = currentState->currentCost;
        int currentDuration = currentState->currentDuration;

        // Check if the destination is reached
        if (currentNode->city == endCity) {
            allPaths.emplace_back(currentPath, currentCost, currentDuration);
            delete currentState; // Clean up
            continue;
        }

        // Find neighbors of the current node
        int index = -1;
        for (int i = 0; i < size; ++i) {
            if (flightLists[i].head && flightLists[i].head->city == currentNode->city) {
                index = i;
                break;
            }
        }

        // If no neighbors, clean up and continue
        if (index == -1) {
            delete currentState;
            continue;
        }

        // Traverse neighbors and push them onto the stack
        LinkedList::Node* neighbor = flightLists[index].head->nextNode;
        while (neighbor) {
            if (find(currentPath.begin(), currentPath.end(), neighbor->city) == currentPath.end()) {
                vector<string> newPath = currentPath;
                newPath.push_back(neighbor->city);

                stateStack.push(new State(
                    neighbor,
                    newPath,
                    currentCost + neighbor->cost,
                    currentDuration + neighbor->duration
                ));
            }
            neighbor = neighbor->nextNode;
        }

        // Clean up the current state
        delete currentState;
    }
}



void sortPaths(string filter) {
    if (filter == "T") {
        // Sort by duration (shorter time first)
        sort(allPaths.begin(), allPaths.end(), [](const PathInfo& a, const PathInfo& b) {
            return a.duration < b.duration;
        });
    } else if (filter == "C") {
        // Sort by cost (lower cost first)
        sort(allPaths.begin(), allPaths.end(), [](const PathInfo& a, const PathInfo& b) {
            return a.cost < b.cost;
        });
    }
}

void printLists() {
    cout << "\n";
    for (int i = 0; i < size; ++i) {
        if (flightLists[i].head) {
            cout << "Linked List " << i + 1 << ":" << endl;
            flightLists[i].printList();
        }
    }
}

void addNode(const string& origin, const string& destination, float cost, int duration) {
    bool originFound = false;

    // Search for the origin in the list array
    for (int i = 0; i < size; ++i) {
        if (flightLists[i].head && flightLists[i].head->city == origin) {
            flightLists[i].addNode(destination, cost, duration);
            originFound = true;
            break;
        }
    }

    // If the origin is not found, create a new linked list for it
    if (!originFound) {
        for (int i = 0; i < size; ++i) {
            if (!flightLists[i].head) {
                flightLists[i].head = new LinkedList::Node(origin, 0, 0);
                flightLists[i].addNode(destination, cost, duration);
                break;
            }
        }
    }
}


void readFiles() {
    cout << "Enter Flight Input File: " << endl;
    getline(cin, inputFile); // Read user input
    cout << "Enter Requested Flight File: " << endl;
    getline(cin, requestFile); // Read user input
    cout << "Enter Output File Name: " << endl;
    getline(cin, outputFileName); // Read user input for the output file name

    flightDataInput.open(inputFile);

    if (!flightDataInput.is_open()) {
        cerr << "Error: Could not open flight data file: " << inputFile << endl;
        return;
    }
    requestedFlightsInput.open(requestFile);

    if (!requestedFlightsInput.is_open()) {
        cerr << "Error: Could not open requested flights file: " << requestFile << endl;
        return;
    }

    string line;
    if (getline(flightDataInput, line)) {
        flightData = stoi(line); // Convert the first line to an integer
    }

    if (getline(requestedFlightsInput, line)) {
        requestedPaths = stoi(line); // Convert the first line to an integer
    }
}

void processFlightData(string line) {
    stringstream ss(line);
    string origin, destination, costStr, durationStr;
    int duration;
    float cost;
    // Extract substrings separated by '|'
    getline(ss, origin, '|');         // Get the origin
    getline(ss, destination, '|');    // Get the destination
    getline(ss, costStr, '|');        // Get the cost as a string
    getline(ss, durationStr, '|');    // Get the duration as a string
    cost = stoi(costStr);
    duration = stoi(durationStr);

    addNode(origin, destination, cost, duration);
    addNode(destination, origin, cost, duration); //bidirectional
}

void processRequestedData(string line, int lineCount, ofstream& outputFile) {
    stringstream ss(line);
    string startCity, endCity, filter;

    getline(ss, startCity, '|');
    getline(ss, endCity, '|');
    getline(ss, filter, '|');

    allPaths.clear();

    findPathWithStack(startCity, endCity); // Use stack-based pathfinding

    sortPaths(filter); // Sort paths based on the filter

    if (allPaths.empty()) {
        cout << "No paths found from " << startCity << " to " << endCity << "." << endl;
        outputFile << "No paths found from " << startCity << " to " << endCity << "." << endl;
    } else {
        cout << "\nFlight " << lineCount << ": " << startCity << " to " << endCity << " (" << (filter == "T" ? "Time" : "Cost") << ")\n";
        outputFile << "\nFlight " << lineCount << ": " << startCity << " to " << endCity << " (" << (filter == "T" ? "Time" : "Cost") << ")\n";

        int pathNum = 1;
        for (const auto& pathInfo : allPaths) {
            cout << "Path " << pathNum << ": ";
            outputFile << "Path " << pathNum++ << ": ";
            for (size_t i = 0; i < pathInfo.path.size(); ++i) {
                cout << pathInfo.path[i];
                outputFile << pathInfo.path[i];
                if (i < pathInfo.path.size() - 1) {
                    cout << " -> ";
                    outputFile << " -> ";
                }
            }
            cout << " | Time: " << pathInfo.duration << ", Cost: " << pathInfo.cost << endl;
            outputFile << " | Time: " << pathInfo.duration << ", Cost: " << pathInfo.cost << endl;
        }
    }
}

void processData() {
    cout << "Processing Flight Data File:" << endl;
    string line;
    int lineCount = 0;

    // Read flight data
    while (getline(flightDataInput, line)) {
        processFlightData(line);
    }

    // Open the output file using the name provided by the user
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to create output file: " << outputFileName << endl;
        return;
    }

    cout << "Processing Requested Flights File:" << endl;
    while (getline(requestedFlightsInput, line)) {
        lineCount++;
        processRequestedData(line, lineCount, outputFile); // Pass file reference
        pathCount = 0;
    }

    outputFile.close(); // Close the output file after all requests are processed
    printLists();
}


//--------------------main function--------------------//

int main() {
    readFiles(); // Ask user for both input files
    size = flightData * 2;
    flightLists = new LinkedList[size]; // Dynamically allocate an array of LinkedLists
    cout << "\n\nDisplaying results on created file: flightoutput.txt\n\n"<<endl;
    processData(); // Process the data


    return 0;
}
