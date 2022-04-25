// =================== Support Code =================
// Graph.
//
// - Implement each of the functions to create a working graph.
// - Do not change any of the function declarations
//   - (i.e. graph_t* create_graph() should not have additional arguments)
// - You should not have any 'printf' statements in your graph functions.
//   - (You may consider using these printf statements to debug, but they 
//      should be removed from your final version)
// ==================================================
#ifndef MYSHORTEST_H
#define MYSHORTEST_H

#include <stdlib.h>
#include <assert.h>

// Our library that we have written.
// Also, by a really smart engineer!
#include "my_graph.h"
#include "my_pq.h"
// Note that we are locating this file
// within the same directory, so we use quotations
// and provide the path to this file which is within
// our current directory.


// returns 1 if Dijkstra's algorithm completes successfully
// returns 0 if Dikjstra's algorithm does not complete successfully
// NOTE: Recall from our discussion, Dikjstra's algorithm does not
//       handle negative weight edges
// returns -1 if the graph is NULL
int calculate_shortest_path(graph_t * g, int source) {
	// TODO: Implement me!
    if ( g == NULL ) return -1;

	// g->source not equal to -1 means calculate dikjstra does not need to run
    if ( g->source != -1 ) return 0;
	
	queue_t* q = create_queue(g->numNodes);

	// initialize all the cost from
	node_t * node = g->nodes->head;
	while(node){
		graph_node_t *currNode = (graph_node_t *)node->data;
		currNode->cost = 99999999;
		currNode->from = -99999999;
		queue_enqueue(q, currNode);
		node = node->next;
	}

	// set the cost for source to zero, to start the path finding
	graph_node_t *src = find_node(g, source);
	if(src == NULL) return 0;
	src->cost = 0;

	while(q->size > 0){
		graph_node_t* srcNode = queue_dequeue(q);
		node_t *outNeighborEdge = getOutNeighbors(g, srcNode->data)->head;
		while(outNeighborEdge){
			graph_edge_t *currEdge = (graph_edge_t *)outNeighborEdge->data;
			// negative cost algo will not run, return 0 for failure
			if(currEdge->cost < 0){
				free_queue(q);
                printf("Negative weight exists in the graph, Dikjstra incomplete\n");
				return 0;
			}
			graph_node_t *toNode = find_node(g, currEdge->to);
			int altCost = currEdge->cost + srcNode->cost;
			if(toNode->cost > altCost){
				toNode->cost = altCost;
				toNode->from = srcNode->data;
			}
			outNeighborEdge = outNeighborEdge->next;
		}
	}

	free_queue(q);

	g->source = source;  

	return 1;
}

// prints the shortest path from source to destination if there
// exists a path from the source to the destination. Consider
// how you can solve this recursively if you want to avoid using
// another data structure.
//
// Returns 1 if there is a path from source to destination
// Returns 0 if there is not a path from source to destination
// Returns -1 if the graph is NULL
// Returns -2 if called before `calculate_shortest_path` is called
// Note: consider the bookkeeping fields in `graph_t` struct to
//       determine whether this is true or not
int print_shortest_path(graph_t * g, int source, int dest) {
	// TODO: Implement me!
    if ( g == NULL ) return -1;

	// calculate_shortest_path should be called first
    if ( g->source == -1 ) {
		printf("Please calculate shortest path before calling print shortest graph\n");
		return -2;
	}

	// In case source or dest is not found, no path possible
	if(find_node_in_graph_dll(g, source) == -1) return 0;
	if(find_node_in_graph_dll(g, dest) == -1) return 0;
    // In case the source is not the calculated source, need re-calculation
    if(source != g->source){
		printf("Please calculate shortest path from %d before calling print shortest graph\n", source);
		return -2;
	}

    // Only push front is used
	dll_t *stack = create_dll();
	// try to back track from destination to source
	// check if the destination -> x ... -> y -> source has the link
	graph_node_t* pre_node = find_node(g, dest);
	int cost = pre_node->cost;
	while(pre_node != NULL && pre_node->from != -99999999){
		dll_push_front(stack, pre_node->data);
		pre_node = find_node(g, pre_node->from);
	}
	// Source is not reachable from destination from back tracking process
	if(pre_node->data != source) {
        free_dll(stack);
        return 0;
    }

	dll_push_front(stack, source);
	printf("Shortest path from %d to %d : ", source, dest);
	print_dll(stack);
	printf("Shortest path cost : %d\n", cost);

	free_dll(stack);
	return 1;
}

#endif
