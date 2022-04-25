
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "my_shortest.h"

// Create a weight graph, check connections, edges and weights set
int unitTest1(int status) {
    int passed = 1;

    graph_t * g = create_graph();

	graph_add_node(g, 1);
	graph_add_node(g, 2);

	if(graph_num_nodes(g) != 2) passed = 0;

	int res = graph_add_edge(g, 1, 2, 3);
	if(res != 1) passed = 0;
	res = graph_add_edge(g, 2, 1, 10);
	if(res != 1) passed = 0;

	if(contains_edge(g, 1, 2) != 1 || contains_edge(g, 2, 1) != 1) passed = 0;

	graph_node_t *outNode = find_node(g, 1);
	graph_node_t *inNode = find_node(g, 2);

	// test if the outNeighbors edge for 1 is pointing to inNeighbors edge for 2.
	graph_edge_t * one_out_edge = outNode->outNeighbors->head->data;
	graph_edge_t * two_in_edge = inNode->inNeighbors->head->data;
	if(one_out_edge != two_in_edge) passed = 0;

	if(one_out_edge->cost != 3 || one_out_edge->from != 1 || one_out_edge->to != 2) passed = 0;
	if(two_in_edge->cost != 3 || two_in_edge->from != 1 || two_in_edge->to != 2) passed = 0;

	free_graph(g);

	return passed;
}

// Check for negative weight entry failure, repeated add_node and failure to add edge to non-existing node
int unitTest2(int status) {
	int passed = 1;

	graph_t * g = create_graph();

	graph_add_node(g, 1);
	graph_add_node(g, 2);
	graph_add_node(g, 2);
	if(g->numNodes != 2) passed = 0;

	int res = graph_add_edge(g, 1, 2, -3);
	if(res != 1) passed = 0;
	res = graph_add_edge(g, 2, 1, -10);
	if(res != 1) passed = 0;
	res = graph_add_edge(g, 3, 1, 1);
	if(res != 0) passed = 0;

	if(g->numEdges != 2) passed = 0;

	free_graph(g);

	return passed;
}


// Check if remove edge works, and also if there are more than one edge in neighbor dll
int unitTest3(int status) {
    int passed = 1;

    graph_t * g = create_graph();
    graph_add_node(g, 1);
    graph_add_node(g, 2);
    graph_add_node(g, 3);
    graph_add_node(g, 4);

	// try removing an edge that does not exist
	if(graph_remove_edge(g, 1, 2) != 0) passed = 0;

	graph_add_edge(g, 1, 2, 3);
	graph_add_edge(g, 1, 3, 4);
	graph_add_edge(g, 2, 3, 10);
	graph_add_edge(g, 3, 4, 5);
    
	if(contains_edge(g, 2, 1) == 1) passed = 0;
	if(contains_edge(g, 1, 2) != 1) passed = 0;
	if(contains_edge(g, 1, 3) != 1) passed = 0;
	if(contains_edge(g, 2, 3) != 1) passed = 0;
	if(contains_edge(g, 3, 4) != 1) passed = 0;
	if(graph_num_edges(g) != 4) passed = 0;

	graph_node_t *outNode = find_node(g, 1);

	// test if the outNeighbors edge for 1 is pointing to inNeighbors edge for 2.
	node_t * node = outNode->outNeighbors->head;
	graph_edge_t *one_out_edge = node->data;
	if(one_out_edge->cost != 3 || one_out_edge->from != 1 || one_out_edge->to != 2) passed = 0;
	node = node->next;
	one_out_edge = node->data;
	if(one_out_edge->cost != 4 || one_out_edge->from != 1 || one_out_edge->to != 3) passed = 0;

	graph_remove_edge(g, 1, 2);
	graph_remove_edge(g, 1, 3);
	graph_remove_edge(g, 2, 3);
	graph_remove_edge(g, 3, 4);

	if(contains_edge(g, 1, 2) == 1) passed = 0;
	if(contains_edge(g, 1, 3) == 1) passed = 0;
	if(contains_edge(g, 2, 3) == 1) passed = 0;
	if(contains_edge(g, 3, 4) == 1) passed = 0;
	if(graph_num_edges(g) != 0) passed = 0;

	free_graph(g);

	return passed;
}


// Check against the Dikjstra after running
int unitTest4(int status) {
    int passed = 1;

    graph_t * g = create_graph();
	graph_add_node(g, 1);
	graph_add_node(g, 2);
	graph_add_node(g, 3);
	graph_add_node(g, 4);

	graph_add_edge(g, 1, 2, 10);
	graph_add_edge(g, 1, 3, 10);
	graph_add_edge(g, 2, 3, 10);
	graph_add_edge(g, 3, 4, 10);

	// 1 should have outNeighbors 2, 3 and no inNeighbors
	if(getNumInNeighbors(g, 1) != 0) passed = 0;

	dll_t *out_1 = getOutNeighbors(g, 1);
	if(getNumOutNeighbors(g, 1) != 2) passed = 0;
	// dll_contains_value returns the actual position, 
	// therefore should be non-negative
	if(outNeighbor_dll_contains_dest(out_1, 2) < 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_1, 3) < 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_1, 4) >= 0) passed = 0;

	// 2 should have outNeighbors 3 and inNeighbors 1
	dll_t *in_2 = getInNeighbors(g, 2);
	if(getNumInNeighbors(g, 2) != 1) passed = 0;
	if(inNeighbor_dll_contains_src(in_2, 1) < 0) passed = 0;

	dll_t *out_2 = getOutNeighbors(g, 2);
	if(getNumOutNeighbors(g, 2) != 1) passed = 0;
	if(outNeighbor_dll_contains_dest(out_2, 3) < 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_2, 4) >= 0) passed = 0;
	
	// 3 should have outNeighbors 4 and inNeighbors 1, 2
	dll_t *in_3 = getInNeighbors(g, 3);
	if(getNumInNeighbors(g, 3) != 2) passed = 0;
	if(inNeighbor_dll_contains_src(in_3, 1) < 0) passed = 0;
	if(inNeighbor_dll_contains_src(in_3, 2) < 0) passed = 0;

	dll_t *out_3 = getOutNeighbors(g, 3);
	if(getNumOutNeighbors(g, 3) != 1) passed = 0;
	if(outNeighbor_dll_contains_dest(out_3, 4) < 0) passed = 0;
	
	// 4 should have no outNeighbors and inNeighbors 3
	dll_t *in_4 = getInNeighbors(g, 4);
	if(getNumInNeighbors(g, 4) != 1) passed = 0;
	if(inNeighbor_dll_contains_src(in_4, 3) < 0) passed = 0;

	dll_t *out_4 = getOutNeighbors(g, 4);
	if(getNumOutNeighbors(g, 4) != 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_4, 1) >= 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_4, 2) >= 0) passed = 0;
	if(outNeighbor_dll_contains_dest(out_4, 3) >= 0) passed = 0;
	
	free_graph(g);

	return passed;
}

// Check if graph_remove_node method works correctly
int unitTest5(int status) {
    int passed = 1;

    graph_t * g = create_graph();
    graph_add_node(g, 1);
    graph_add_node(g, 2);
    graph_add_node(g, 3);
    graph_add_node(g, 4);

	graph_add_edge(g, 1, 2, 5);
	graph_add_edge(g, 1, 3, 5);
	graph_add_edge(g, 2, 3, 5);
	graph_add_edge(g, 3, 4, 5);
	if(contains_edge(g, 1, 2) == 0) passed = 0;
	if(contains_edge(g, 1, 3) == 0) passed = 0;
	if(graph_num_edges(g) != 4) passed = 0;

	graph_remove_node(g, 1);
	// edges related to the deleted nodes should have been deleted
	if(contains_edge(g, 1, 2) == 1) passed = 0;
	if(contains_edge(g, 1, 3) == 1) passed = 0;
	if(graph_num_edges(g) != 2) passed = 0;

	graph_remove_node(g, 3);
	// edges related to the deleted nodes should have been deleted
	if(contains_edge(g, 2, 3) == 1) passed = 0;
	if(contains_edge(g, 3, 4) == 1) passed = 0;
	if(graph_num_edges(g) != 0) passed = 0;

	if(graph_num_nodes(g) != 2) passed = 0;

	// No free_graph should be needed
	graph_remove_node(g, 2);
	graph_remove_node(g, 4);
	free_dll(g->nodes);
	free(g);
    return passed;
}

// Check against the Dikjstra after running
int unitTest6(int status) {
    int passed = 1;

    graph_t * g = create_graph();
	graph_add_node(g, 1);
	graph_add_node(g, 2);
	graph_add_node(g, 3);
	graph_add_node(g, 4);
	graph_add_node(g, 5);

	graph_add_node(g, 6);
	graph_add_edge(g, 1, 6, 10);
	graph_add_edge(g, 6, 1, 1);

	graph_add_edge(g, 2, 3, 5);
	graph_add_edge(g, 4, 2, 2);
	graph_add_edge(g, 1, 2, 6);
	graph_add_edge(g, 1, 4, 1);
	graph_add_edge(g, 4, 5, 1);
	graph_add_edge(g, 5, 2, 2);
	graph_add_edge(g, 5, 3, 5);

	graph_add_edge(g, 3, 2, 5);
	graph_add_edge(g, 2, 4, 2);
	graph_add_edge(g, 2, 1, 6);
	graph_add_edge(g, 4, 1, 1);
	graph_add_edge(g, 5, 4, 1);
	graph_add_edge(g, 2, 5, 2);
	graph_add_edge(g, 3, 5, 5);

	int res = calculate_shortest_path(g, 1);
	if(res != 1) passed = 0;
	graph_remove_node(g, 6);
	
    res = print_shortest_path(g, 1, 3);
	if(res != -2) passed = 0;
	res = calculate_shortest_path(g, 1);
	if(res != 1) passed = 0;
	// Finding the shortest path from 2, which is not calculated
	res = print_shortest_path(g, 2, 3);
	if(res != -2) passed = 0;
	// No node 10
	res = print_shortest_path(g, 10, 3);
	if(res != 0) passed = 0;
	// No node 9
	res = print_shortest_path(g, 2, 9);
	if(res != 0) passed = 0;
	res = print_shortest_path(g, 1, 3);
	if(res != 1) passed = 0;
	print_all_node(g);
	free_graph(g);

	return passed;
}

// Check against the Dikjstra after running
int unitTest7(int status) {
    int passed = 1;

    graph_t * g = create_graph();
    graph_add_node(g, 1);
    graph_add_node(g, 2);
    graph_add_node(g, 3);
    graph_add_node(g, 4);
    graph_add_node(g, 5);
    graph_add_node(g, 6);

	graph_add_edge(g, 1, 2, 2);
	graph_add_edge(g, 1, 4, 8);
	graph_add_edge(g, 2, 5, 6);
	graph_add_edge(g, 4, 6, 2);
	graph_add_edge(g, 4, 5, 3);
	graph_add_edge(g, 5, 3, 9);
	graph_add_edge(g, 6, 3, 3);
	graph_add_edge(g, 2, 4, 5);
	graph_add_edge(g, 5, 6, 1);

	graph_add_edge(g, 2, 1, 2);
	graph_add_edge(g, 4, 1, 8);
	graph_add_edge(g, 5, 2, 6);
	graph_add_edge(g, 6, 4, 2);
	graph_add_edge(g, 5, 4, 3);
	graph_add_edge(g, 3, 5, 9);
	graph_add_edge(g, 3, 6, 3);
	graph_add_edge(g, 4, 2, 5);
	graph_add_edge(g, 6, 5, 1);

	int res = calculate_shortest_path(g, 1);
	if(res != 1) passed = 0;

	graph_node_t *node = find_node(g, 1);
	if(node->cost != 0) passed = 0;
	if(node->from != -99999999) passed = 0;

	node = find_node(g, 2);
	if(node->cost != 2) passed = 0;
	if(node->from != 1) passed = 0;

	node = find_node(g, 3);
	if(node->cost != 12) passed = 0;
	if(node->from != 6) passed = 0;

	node = find_node(g, 4);
	if(node->cost != 7) passed = 0;
	if(node->from != 2) passed = 0;

	node = find_node(g, 5);
	if(node->cost != 8) passed = 0;
	if(node->from != 2) passed = 0;

	node = find_node(g, 6);
	if(node->cost != 9) passed = 0;
	if(node->from != 4) passed = 0;

	print_all_node(g);

	res = print_shortest_path(g, 1, 3);
	if(res != 1) passed = 0;

    free_graph(g);

	return passed;
}

// Check against the Dikjstra after running
int unitTest8(int status) {
    int passed = 1;

    graph_t * g = create_graph();
    graph_add_node(g, 0);
    graph_add_node(g, 1);
    graph_add_node(g, 2);
    graph_add_node(g, 3);
    graph_add_node(g, 4);
    graph_add_node(g, 5);
    graph_add_node(g, 6);
    graph_add_node(g, 7);
    graph_add_node(g, 8);

	graph_add_edge(g, 0, 1, 4);
	graph_add_edge(g, 1, 2, 8);
	graph_add_edge(g, 2, 3, 7);
	graph_add_edge(g, 3, 4, 9);
	graph_add_edge(g, 4, 5, 10);
	graph_add_edge(g, 5, 6, 2);
	graph_add_edge(g, 6, 7, 1);
	graph_add_edge(g, 7, 8, 7);
	graph_add_edge(g, 0, 7, 8);
	graph_add_edge(g, 1, 7, 11);
	graph_add_edge(g, 2, 8, 2);
	graph_add_edge(g, 8, 6, 6);
	graph_add_edge(g, 2, 5, 4);
	graph_add_edge(g, 3, 5, 14);

	graph_add_edge(g, 1, 0, 4);
	graph_add_edge(g, 2, 1, 8);
	graph_add_edge(g, 3, 2, 7);
	graph_add_edge(g, 4, 3, 9);
	graph_add_edge(g, 5, 4, 10);
	graph_add_edge(g, 6, 5, 2);
	graph_add_edge(g, 7, 6, 1);
	graph_add_edge(g, 8, 7, 7);
	graph_add_edge(g, 7, 0, 8);
	graph_add_edge(g, 7, 1, 11);
	graph_add_edge(g, 8, 2, 2);
	graph_add_edge(g, 6, 8, 6);
	graph_add_edge(g, 5, 2, 4);
	int res = graph_add_edge(g, 5, 3, -14);
	if(res != 1) passed = 0;
	graph_remove_edge(g, 5, 3);
	res = graph_add_edge(g, 5, 3, 14);
	if(res != 1) passed = 0;

	res = calculate_shortest_path(g, 0);
	if(res != 1) passed = 0;
	print_all_node(g);

	res = print_shortest_path(g, 0, 4);
	if(res != 1) passed = 0;

    free_graph(g);

	return passed;
}

// removing and adding back nodes and edges
int unitTest9(int status) {
    int passed = 1;

    graph_t * g = create_graph();
	graph_add_node(g, 1);
	graph_add_node(g, 2);
	graph_add_node(g, 3);
	graph_add_node(g, 4);
	graph_add_node(g, 5);

	graph_add_node(g, 6);
	graph_add_edge(g, 1, 6, 10);
	graph_add_edge(g, 6, 1, 1);
	graph_add_edge(g, 2, 6, 10);
	graph_add_edge(g, 6, 2, 1);

	graph_add_edge(g, 2, 3, 5);
	graph_add_edge(g, 4, 2, 2);
	graph_add_edge(g, 1, 2, 6);
	graph_add_edge(g, 1, 4, 1);
	graph_add_edge(g, 4, 5, 1);
	graph_add_edge(g, 5, 2, 2);
	graph_add_edge(g, 5, 3, 5);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);
	graph_add_edge(g, 5, 3, 15);

	graph_add_edge(g, 3, 2, 5);
	graph_add_edge(g, 2, 4, 2);
	graph_add_edge(g, 2, 1, 6);
	graph_add_edge(g, 4, 1, 1);
	graph_add_edge(g, 5, 4, 1);
	graph_add_edge(g, 2, 5, 2);
	graph_add_edge(g, 3, 5, 5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);
	graph_add_edge(g, 3, 5, -5);

	int res = calculate_shortest_path(g, 1);
	if(res != 1) passed = 0;

	// Remove and rebuild
	graph_remove_node(g, 6);
	graph_remove_node(g, 3);
	graph_add_node(g, 3);
	graph_add_edge(g, 2, 3, 5);
	graph_add_edge(g, 3, 2, 5);
	graph_add_edge(g, 3, 5, 5);
	graph_add_edge(g, 5, 3, 5);
	res = calculate_shortest_path(g, 1);
	if(res != 1) passed = 0;

	res = print_shortest_path(g, 1, 3);
	if(res != 1) passed = 0;

	print_all_node(g);
	free_graph(g);

	return passed;
}

// Checking for negative weight entries
int unitTest10(int status) {
    int passed = 1;

    graph_t * g = create_graph();
	graph_add_node(g, 1);
	graph_add_node(g, 2);
	graph_add_node(g, 3);
	graph_add_node(g, 4);
	graph_add_node(g, 5);

	graph_add_edge(g, 2, 3, 5);
	graph_add_edge(g, 4, 2, 2);
	graph_add_edge(g, 1, 2, 6);
	graph_add_edge(g, 1, 4, 1);
	graph_add_edge(g, 4, 5, 1);
	graph_add_edge(g, 5, 2, 2);
	graph_add_edge(g, 5, 3, -5);

	graph_add_edge(g, 3, 2, 5);
	graph_add_edge(g, 2, 4, 2);
	graph_add_edge(g, 2, 1, 6);
	graph_add_edge(g, 4, 1, 1);
	graph_add_edge(g, 5, 4, 1);
	graph_add_edge(g, 2, 5, 2);
	graph_add_edge(g, 3, 5, 5);

	int res = calculate_shortest_path(g, 1);
	if(res != 0) passed = 0;

	res = print_shortest_path(g, 1, 3);
	if(res != -2) passed = 0;
	free_graph(g);

	return passed;
}

	
// TODO: Add more tests here at your discretion
int (*unitTests[])(int)={
	unitTest1,
	unitTest2,
	unitTest3,
	unitTest4,
	unitTest5,
	unitTest6,
	unitTest7,
	unitTest8,
	unitTest9,
	unitTest10,
	NULL
};


// ====================================================
// ================== Program Entry ===================
// ====================================================
int main() {
    unsigned int testsPassed = 0;
    // List of Unit Tests to test your data structure
    int counter = 0;
    while(unitTests[counter]!=NULL){
	printf("========unitTest %d========\n", counter);
        if(1==unitTests[counter](1)){
		printf("passed test\n");
		testsPassed++;	
	}else{
		printf("failed test, missing functionality, or incorrect test\n");
	}
        counter++;
    }

    printf("%d of %d tests passed\n", testsPassed, counter);

    return 0;
}
