#include "graph.h"
#include "shortest_path.h"
#include "heap.hpp"

#define INF 1e9 // INT_MAX doesn't work :(

Path shortestPath(const Graph& g, int source, int dest) {
  // Only allowed to use MaxHeap, so we can invert all priority by *-1
  // which essentially changes the MaxHeap into a MinHeap implementation
  // extractMax becomes extractMin
	int numVertices = g.num_vertices();
	
	// check coz im somehow getting segfault for one of the private test cases
	if (source < 0 || source >= numVertices || dest < 0 || dest >= numVertices) {
		throw std::out_of_range("Invalid source or destination");
	}

	vector<int> dist(numVertices, INF); // init all est. dist. to infinity
	vector<int> parent(numVertices, -1); // keep track of parents to return path later. -1 = u are ur own parent
	
	// Store {distance, vertext} in a pq
	Heap<GraphEdge> pq;

	dist[source] = 0;
	pq.insert(GraphEdge(source, 0)); // Insert source with dist 0

	while (!pq.empty()) {
		int currNode = pq.extractMax().dest(); // extractMin
		if (currNode == dest) break; // early exit if dest reached

		// relaxxxxxx edging relaxxxxxx
		for (const GraphEdge& currEdge : g.edges_from(currNode)) {
			int nextNode = currEdge.dest();
			int newDist = dist[currNode] + currEdge.weight();
			if (dist[nextNode] > newDist) {
				dist[nextNode] = newDist;
				parent[nextNode] = currNode;
				pq.insert(GraphEdge(nextNode, -newDist)); // Store negated dist to simulate the minHeap behaviour using maxHeap
			}
		}
	}
	if (dist[dest] == INF || parent[dest] == -1) {
		return Path(-1, {});
		//throw std::out_of_range("No path found");
	}
	// Reconstruct the path from dest to source
	vector<int> path;
	for (int node = dest; node != -1; node = parent[node]) {
		path.insert(path.begin(), node); // insertHead() which reverses the order naturally
	}
	return Path(dist[dest], path);
}