#include <iostream>
#include <string>
#include <vector>   // for vectors

using namespace std;

// Node structure for the LinkedList
class LinkedList {
public:
    struct Node {
        string city;
        int cost;
        int duration;
        Node* nextNode = nullptr;

        Node(const string& city, int cost, int duration)
            : city(city), cost(cost), duration(duration), nextNode(nullptr) {}
    };

    Node* head;

    LinkedList() : head(nullptr) {}

    void addNode(const string& destination, int cost, int duration) {
        Node* newNode = new Node(destination, cost, duration);
        if (!head) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->nextNode) {
                current = current->nextNode;
            }
            current->nextNode = newNode;
        }
    }

    void printList() const {
        Node* current = head;
        while (current) {
            cout << "City: " << current->city
                 << ", Cost: " << current->cost
                 << ", Duration: " << current->duration << endl;
            current = current->nextNode;
        }
    }

    ~LinkedList() {
        while (head) {
            Node* temp = head;
            head = head->nextNode;
            delete temp;
        }
    }
};
