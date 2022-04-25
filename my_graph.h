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
#ifndef MYGRAPH_H
#define MYGRAPH_H

#include "my_dll.h"
#include <stdlib.h>
#include <assert.h>
// Create a graph data structure
typedef struct graph {
    int numNodes;
    int numEdges;
    dll_t* nodes;   // a list of nodes storing all of our nodes

    // bookkeeping for Dikjstra's Algorithm
    // -1 indicates that there is no shortest-path data in the graph
    // it should be reset whenever a node or edge is added or removed 
    // from the graph
    int source;
} graph_t;

typedef struct graph_node {
    // per-node bookkeping for Dikjstra's Algorithm
    int data;
    int cost;
    int from;
    // per-node neighbor information which now stores edges instead of nodes
    dll_t* inNeighbors;
    dll_t* outNeighbors;
} graph_node_t;


typedef struct graph_edge {
    int from;
    int to;
    int cost;
} graph_edge_t;

// -----------------------------helper function---------------------------------

// Returns the node pointer if the node exists.
// Returns NULL if the node doesn't exist or the graph is -1
int find_node_in_graph_dll( graph_t * g, int value){
    if ( g == NULL ) return -1;

    int pos = 0;
    node_t *curr_node = g->nodes->head;
    while(curr_node != NULL){
        // data of the head of g->node contains a graph node
        graph_node_t* curr_g_node = (graph_node_t*)curr_node->data;
        if((int)curr_g_node->data == value){
            return pos;
        }
        pos++;
        curr_node = curr_node->next;
    }
    return -1;
}

// Check the src is present on the inNeighbor_dll
// -1 means l is NULL
// -9999 means not found
int inNeighbor_dll_contains_src(dll_t* l, int src){
    int pos = 0;

    if(l == NULL) return -1;
    node_t *curr_node = l->head;
    while(curr_node != NULL){
        graph_edge_t *temp_g_edge = curr_node->data;
        if(temp_g_edge->from == src){
            return pos;
        }
        curr_node = curr_node->next;
        pos++;
    }
    return -9999;
}

// Check the dest is present on the outNeighbor_dll
// -1 means l is NULL
// -9999 means not found
int outNeighbor_dll_contains_dest(dll_t* l, int dest){
    int pos = 0;

    if(l == NULL) return -1;
    node_t *curr_node = l->head;
    while(curr_node != NULL){
        graph_edge_t *temp_g_edge = curr_node->data;
        if(temp_g_edge->to == dest){
            return pos;
        }
        curr_node = curr_node->next;
        pos++;
    }
    return -9999;
}


void print_all_node(graph_t* g){
    node_t *node = g->nodes->head;
    while(node){
        graph_node_t *curr_g_node = node->data;
        printf("Value:\t\t%d\n", curr_g_node->data);
        printf("Cost:\t\t%d\n", curr_g_node->cost);
        printf("From node:\t%d\n\n", curr_g_node->from);
        node = node->next;
    }
}


// -----------------------------------------------------------------------------


// Creates a graph
// Returns a pointer to a newly created graph.
// The graph should be initialized with data on the heap.
// The graph fields should also be initialized to default values.
// Returns NULL if we cannot allocate memory.
graph_t* create_graph(){
    // Modify the body of this function as needed.
    graph_t* myGraph= (graph_t*)malloc(sizeof(graph_t));
    if ( myGraph == NULL ) return NULL;

    myGraph->nodes = create_dll();
    myGraph->numEdges = 0;
    myGraph->numNodes = 0;
    myGraph->source = -1;
    return myGraph;
}

// Creates a graph node
// Note: This relies on your dll implementation.
graph_node_t * create_graph_node(int value){
    graph_node_t* graph_node = (graph_node_t*)malloc(sizeof(graph_node_t));
    
    if ( graph_node == NULL ) return NULL;
    
    graph_node->data = value;
    graph_node->inNeighbors = create_dll();
    graph_node->outNeighbors = create_dll();
    
    return graph_node;
}

// Returns the node pointer if the node exists.
// Returns NULL if the node doesn't exist or the graph is NULL
graph_node_t* find_node( graph_t * g, int value){
    if ( g == NULL ) return NULL;

    int pos = 0;
    node_t *curr_node = g->nodes->head;
    while(curr_node != NULL){
        // data of the head of g->node contains a graph node
        graph_node_t* curr_g_node = (graph_node_t*) curr_node->data;
        if((int)curr_g_node->data == value){
            return curr_g_node;
        }
        pos++;
        curr_node = curr_node->next;
    }
    return NULL;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node already exists )
// Returns -1 if the graph is NULL.
int graph_add_node(graph_t* g, int value){
    if ( g == NULL ) return -1;
    
    // if the value exists in any graph nodes
    if (find_node_in_graph_dll(g, value) != -1) return 0;
    
    graph_node_t * newNode = create_graph_node(value);
    // *** TODO: Should return 0 or -1? ***
    if ( newNode == NULL ) return -1;
    
    assert(g->nodes);
    dll_push_back(g->nodes, newNode);

    // Reset for running dikjstra
    g->source = -1;
    
    g->numNodes++;
    
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge doesn't exists )
// Returns -1 if the graph is NULL.
int graph_remove_edge(graph_t * g, int source, int destination){
    // TODO: Implement me!
    // TODO: Can use get_edge?
    //The function removes an edge from source to destination but not the other way.
    //Make sure you remove destination from the out neighbors of source.
    //Make sure you remove source from the in neighbors of destination.
    if ( g == NULL ) return -1;

    // Working on out neighor (source -> destination)
    graph_node_t *src = find_node(g, source);
    if(src == NULL) return 0;

    // Working on in neighor (source -> destination)
    graph_node_t *dest = find_node(g, destination);
    if(dest == NULL) return 0;

    int src_neighbor_pos = outNeighbor_dll_contains_dest(src->outNeighbors, destination);
    int dest_neighbor_pos = inNeighbor_dll_contains_src(dest->inNeighbors, source);

    if (src_neighbor_pos < 0 || dest_neighbor_pos < 0) return 0;

    // get the actual edge and free it, then delete the node_t via dll_remove 
    graph_edge_t *common_edge = (graph_edge_t *)dll_get(src->outNeighbors, src_neighbor_pos);
    free(common_edge);

    dll_remove(src->outNeighbors, src_neighbor_pos);
    dll_remove(dest->inNeighbors, dest_neighbor_pos);

    // Reset for running dikjstra
    g->source = -1;

    g->numEdges--;
    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the node doesn't exist )
// Returns -1 if the graph is NULL.
int graph_remove_node(graph_t* g, int value){
    // TODO: Implement me!
    // The function removes the node from the graph along with any edges associated with it.
    // That is, this node would have to be removed from all the in and out neighbor's lists that countain it.
    if ( g == NULL ) return -1;

    int pos = find_node_in_graph_dll(g, value);
    // the node does not exist
    if(pos == -1) return 0;

    graph_node_t *node = dll_get(g->nodes, pos);
    node_t *inNode = node->inNeighbors->head;
    node_t *outNode = node->outNeighbors->head;

    // Remove edges related to all inNeighbors of value node, 
    // i.e. (1 -> 5), (2 -> 5), (1, 2) would be the inNeighbors of 5
    while(inNode != NULL){
        node_t *nextNode = inNode->next;
        graph_edge_t *curr_edge = inNode->data;
        int res = graph_remove_edge(g, curr_edge->from, curr_edge->to);
        if(res != 1) return 0;
        inNode = nextNode;
    }

    // Remove edges related to all inNeighbors of value node, 
    // i.e. (5 -> 1), (5 -> 2), (1, 2) would be the outNeighbors of 5
    while(outNode != NULL){
        node_t *nextNode = outNode->next;
        graph_edge_t *curr_edge = outNode->data;
        int res = graph_remove_edge(g, curr_edge->from, curr_edge->to);
        if(res != 1) return 0;
        outNode = nextNode;
    }

    // freeing the graph_node_t's 2 dlls and the graph_node_t itself
    free_dll(node->inNeighbors);
    free_dll(node->outNeighbors);
    free(node);
    
    // freeing the node_t in g->nodes
    dll_remove(g->nodes, pos);

    // Reset for running dikjstra, in case no edges are reset
    if(g->source != -1) g->source = -1;
    
    g->numNodes--;

    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist, or the edge already exists )
// Returns -1 if the graph is NULL.
int graph_add_edge(graph_t * g, int source, int destination, int cost){
    // TODO: Implement me!
    // The function adds an edge from source to destination but not the other way.
    // Make sure you are not adding the same edge multiple times.
    // Make sure you modify the in and out neighbors appropriatelly. destination will be an out neighbor of source.
    // Source will be an in neighbor of destination.
    if ( g == NULL ) return -1;
    // // cost cannot be negative
    // if ( cost < 0 ) return 0;

    // Working on out neighor (source -> destination)
    graph_node_t *src = find_node(g, source);
    if(src == NULL) return 0;

    // Working on in neighor (source -> destination)
    graph_node_t *dest = find_node(g, destination);
    if(dest == NULL) return 0;

    // if destination contains edge from source, negative means cannot be found
    if(inNeighbor_dll_contains_src(dest->inNeighbors, source) >= 0) return 0;
    // if source contains edge to destination, negative means cannot be found
    if(outNeighbor_dll_contains_dest(src->outNeighbors, destination) >= 0) return 0;

    graph_edge_t *new_edge = (graph_edge_t *)malloc(sizeof(graph_edge_t));
    if(new_edge == NULL) return -1;
    new_edge->from = source;
    new_edge->to = destination;
    new_edge->cost = cost;

    int res = dll_push_back(src->outNeighbors, new_edge);
    if(res != 1) return 0;

    res = dll_push_back(dest->inNeighbors, new_edge);
    if(res != 1) return 0;

    // Reset for running dikjstra
    g->source = -1;

    g->numEdges++;

    return 1;
}

// Returns 1 on success
// Returns 0 on failure ( or if the source or destination nodes don't exist )
// Returns -1 if the graph is NULL.
int contains_edge( graph_t * g, int source, int destination){
    // TODO: Implement me!
    if ( g == NULL ) return -1;

    int src_pos = find_node_in_graph_dll(g, source);
    int dest_pos = find_node_in_graph_dll(g, destination);
    if(src_pos == -1 || dest_pos == -1) return 0;

    graph_node_t *src = (graph_node_t*) dll_get(g->nodes, src_pos);
    graph_node_t *dest = (graph_node_t*) dll_get(g->nodes, dest_pos);

    // Check if the source and destination both contains the edge
    if(outNeighbor_dll_contains_dest(src->outNeighbors, destination) < 0) return 0;
    if(inNeighbor_dll_contains_src(dest->inNeighbors, source) < 0) return 0;

    return 1;
}

// Returns a pointer to the edge
// Returns NULL on failure (or if the source or destination nodes don't exist)
// Exits if the graph is NULL (only use this function if the graph `contains_edge` is true)
graph_edge_t* graph_get_edge(graph_t * g, int source, int destination) {
    // TODO: Implement me!
    if ( g == NULL ) exit(1);

    // Working on out neighor (source -> destination)
    graph_node_t *src = find_node(g, source);
    if(src == NULL) return NULL;

    // Working on in neighor (source -> destination)
    graph_node_t *dest = find_node(g, destination);
    if(dest == NULL) return NULL;

    int src_neighbor_pos = outNeighbor_dll_contains_dest(src->outNeighbors, destination);
    int dest_neighbor_pos = inNeighbor_dll_contains_src(dest->inNeighbors, source);

    if (src_neighbor_pos < 0 || dest_neighbor_pos < 0) return NULL;

    return (graph_edge_t *)dll_get(src->outNeighbors, src_neighbor_pos);
}

// Returns dll_t* of all the in neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getInNeighbors( graph_t * g, int value ){
    // TODO: Implement me!
    if ( g == NULL ) return NULL;

    // Working on out neighor (source -> destination)
    int node_pos = find_node_in_graph_dll(g, value);
    // if graph node dest cannot be found or 
    // src's outNeighors contains source (0 means src->outNeighors exists but without destination value)
    if(node_pos == -1) return NULL;
    graph_node_t *node = dll_get(g->nodes, node_pos);
    return node->inNeighbors;
}

// Returns the number of in neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumInNeighbors( graph_t * g, int value){
    // TODO: Implement me!
    if ( g == NULL ) return -1;

    dll_t * l = getInNeighbors(g, value);
    if (l == NULL) return -1;
    int length = dll_size(l);
    if (length == -1) return -1;

    return length;
}

// Returns dll_t* of all the out neighbors of this node.
// Returns NULL if thte node doesn't exist or if the graph is NULL.
dll_t* getOutNeighbors( graph_t * g, int value ){
    // TODO: Implement me!
    if ( g == NULL ) return NULL;

    // Working on out neighor (source -> destination)
    int node_pos = find_node_in_graph_dll(g, value);
    // if graph node dest cannot be found or 
    // src's outNeighors contains source (0 means src->outNeighors exists but without destination value)
    if(node_pos == -1) return NULL;
    graph_node_t *node = dll_get(g->nodes, node_pos);
    return node->outNeighbors;
}

// Returns the number of out neighbors of this node.
// Returns -1 if the graph is NULL or the node doesn't exist.
int getNumOutNeighbors( graph_t * g, int value){
    // TODO: Implement me!
    if ( g == NULL ) return -1;

    dll_t * l = getOutNeighbors(g, value);
    if (l == NULL) return -1;
    int length = dll_size(l);
    if (length == -1) return -1;

    return length;
}

// Returns the number of nodes in the graph
// Returns -1 if the graph is NULL.
int graph_num_nodes(graph_t* g){
    // TODO: Implement me!
    if ( g == NULL ) return -1;
    return g->numNodes;
}

// Returns the number of edges in the graph,
// Returns -1 on if the graph is NULL
int graph_num_edges(graph_t* g){
    // TODO: Implement me!
    if ( g == NULL ) return -1;
    return g->numEdges;
}

// Free graph
// Removes a graph and ALL of its elements from memory.
// This should be called before the program terminates.
// Make sure you free all the dll's too.
void free_graph(graph_t* g){
    // TODO: Implement me!
    if(g == NULL) return;
    node_t* curr_node = g->nodes->head;
    while(curr_node){
        // Freeing graph_node (it frees the graph edges as well)  inside g->node dll
        node_t* next_node = curr_node->next;
        graph_node_t* curr_g_node = curr_node->data;
        graph_remove_node(g, curr_g_node->data);
        curr_node = next_node;
    }
    free_dll(g->nodes);
    free(g);
}

#endif

// graph_edge_t instead of edge_t in graph_get_edge, and null -> NULL
// return an int for pop front and back instead of void*?
// dll_get should not return an int but a void*?
// graph_remove_edge can be reuse graph_get_edge?
