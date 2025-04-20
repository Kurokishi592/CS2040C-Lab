#include <utility>
#include "graph.h"
#include "shortest_path.h"

using namespace std;

// i gave up parsing automatically, just do it manually

// Helper function to convert characters ('A' to 'J') to vertex indices (0 to 9)
int charToIndex(char c) {
    return c - 'A';
}

// Helper function to convert vertex indices (0 to 9) back to characters ('A' to 'J')
char indexToChar(int i) {
    return 'A' + i;
}

int main() {
    Graph g(10); // 10 vertices (A to J)
    g.addEdge(charToIndex('A'), charToIndex('B'), 77);
    g.addEdge(charToIndex('A'), charToIndex('D'), 148);
    g.addEdge(charToIndex('A'), charToIndex('G'), 157);
    g.addEdge(charToIndex('B'), charToIndex('A'), 144);
    g.addEdge(charToIndex('B'), charToIndex('C'), 105);
    g.addEdge(charToIndex('B'), charToIndex('G'), 79);
    g.addEdge(charToIndex('C'), charToIndex('E'), 144);
    g.addEdge(charToIndex('C'), charToIndex('F'), 106);
    g.addEdge(charToIndex('C'), charToIndex('H'), 31);
    g.addEdge(charToIndex('C'), charToIndex('I'), 111);
    g.addEdge(charToIndex('D'), charToIndex('A'), 117);
    g.addEdge(charToIndex('D'), charToIndex('C'), 166);
    g.addEdge(charToIndex('D'), charToIndex('J'), 158);
    g.addEdge(charToIndex('E'), charToIndex('A'), 82);
    g.addEdge(charToIndex('E'), charToIndex('F'), 27);
    g.addEdge(charToIndex('F'), charToIndex('A'), 29);
    g.addEdge(charToIndex('F'), charToIndex('B'), 97);
    g.addEdge(charToIndex('F'), charToIndex('G'), 99);
    g.addEdge(charToIndex('F'), charToIndex('I'), 17);
    g.addEdge(charToIndex('G'), charToIndex('F'), 45);
    g.addEdge(charToIndex('G'), charToIndex('J'), 122);
    g.addEdge(charToIndex('H'), charToIndex('B'), 134);
    g.addEdge(charToIndex('H'), charToIndex('F'), 102);
    g.addEdge(charToIndex('I'), charToIndex('F'), 150);
    g.addEdge(charToIndex('J'), charToIndex('C'), 18);

    // Set the source and destination
    int source = charToIndex('B'); // Source = B
    int dest = charToIndex('F'); // Destination = F

    Path p = shortestPath(g, source, dest);

    std::cout << "The shortest distance from " << indexToChar(source) << " to " << indexToChar(dest) << " is " 
        << p.total_distance() << " and the path is: { ";
    for (int i = 0; i < p.path().size(); i++) {
        std::cout << indexToChar(p.path()[i]);
        if (i != p.path().size() - 1) std::cout << " , ";
    }
    std::cout << " }" << endl;
    
    return 0;
}

