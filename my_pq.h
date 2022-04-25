// =================== Support Code =================
// Priority Queue
//
// - Implementation of the functions to create a working circular queue.
// - This similar to the queue that we built in HW 2 except that elements
//   are dequeue in order of "priority". We will be implementing it so that
//   elements with the lowest cost are the ones that are dequeued first.
// - Do not change any of the function declarations
//   - (i.e. queue_t* create_queue(unsigned int _capacity) should not have additional arguments)
// - You should not have any 'printf' statements in your queue functions. 
//   - (You may consider using these printf statements to debug, 
//     but they should be removed from your final version)
// ==================================================
#ifndef MYPQ_H
#define MYPQ_H

#include "my_graph.h"

// The main data structure for the queue
struct queue {
	unsigned int back;	    // The next free position in the queue
				    				// (i.e. the end or tail of the line)
	unsigned int front;	    // Current 'head' of the queue
				    				// (i.e. the front or head of the line)
	unsigned int size;	    // How many total elements we currently have enqueued.
	unsigned int capacity;  // Maximum number of items the queue can hold
	graph_node_t** data; 	// The data our queue holds	
};
// Creates a global definition of 'queue_t' so we 
// do not have to retype 'struct queue' everywhere.
typedef struct queue queue_t;

// Create a queue
// Returns a pointer to a newly created queue.
// The queue should be initialized with data on
// the heap.
queue_t* create_queue(unsigned int _capacity){
	queue_t* myQueue = NULL;
	// TODO: Implement me!
	myQueue = (queue_t *)malloc(sizeof(queue_t));
	if(myQueue == NULL) return NULL;
	myQueue->size = 0;
	myQueue->capacity = _capacity;
	myQueue->front = 0;
	myQueue->back = 0;
	myQueue->data = (graph_node_t**)malloc(sizeof(graph_node_t*) * _capacity);
	if(myQueue->data == NULL) return NULL;
	return myQueue;
}

// Queue Empty
// Check if the queue is empty
// Returns 1 if true (The queue is completely empty)
// Returns 0 if false (the queue has at least one element enqueued)
int queue_empty(queue_t* q){
	// TODO: Implement me!
	if(q == NULL) return 0;
	if(q->size == 0) return 1;
	return 0;
}

// Queue Full
// Check if the queue is Full
// Returns 1 if true (The queue is completely full)
// Returns 0 if false (the queue has more space available to enqueue items)
int queue_full(queue_t* q){
	// TODO: Implement me!
	if(q == NULL) return 0;
	if(q->size == q->capacity) return 1;
	return 0;
}

// Enqueue a new node
// i.e. push a new item into our data structure
// Returns a -1 if the operation fails (otherwise returns 0 on success).
// (i.e. if the queue is full that is an error).
int queue_enqueue(queue_t* q, graph_node_t* item){
	// TODO: Implement me!
	if(q == NULL) return 01;
	if(queue_empty(q) == 1){
		// queue is empty, set first element to 0 signalling start of queue
		q->front = 0;
		// Likewise for back which overlaps the front equals 0
		q->back = 0;
		q->data[q->back] = item;
		q->size++;
		// printf("\nEnqueuing...\n");
		// printf("q->size: %d\n", q->size);
		// printf("q->front: %d\n", q->front);
		// printf("q->back: %d\n", q->back);
		return 0;
	}else if(queue_full(q) == 0){
		// queue is not empty but also not full, move the pointer of back by one
		// in case the queue still has space but has to wrap around
		// wrapping done by mod
		q->back = (q->back + 1) % q->capacity;
		q->data[q->back] = item;
		q->size++;
		// printf("\nEnqueuing...\n");
		// printf("q->size: %d\n", q->size);
		// printf("q->front: %d\n", q->front);
		// printf("q->back: %d\n", q->back);
		return 0;
	}
	// when queue is full
	return -1; // Note: you should have two return statements in this function.
}

// Dequeue the node that has the minimum cost
// Returns the item at the front of the queue and
// removes an item from the queue.
// Removing from an empty queue should crash the program, call exit(1)
graph_node_t* queue_dequeue(queue_t *q){
	// TODO: Implement me!
	if(q == NULL) return NULL;
	if(queue_empty(q) == 1){
		// queue is empty, no dequeue
		// queue_empty checked for q == NULL already
		exit(1);
	}else{

		int min_pos = 0;
		int min_cost = 99999999;
		int count = 0;
		// Linear search and get the node with min cost
		int front, back;
		if(q->front > q->back){
			front = q->front;
			back = q->back + q->capacity;
		}else{
			front = q->front;
			back = q->back;
		}
		for (int i = front; i <= back; i++){
			int count = i % q->capacity;
			if(q->data[count]->cost < min_cost){
				min_cost = q->data[count]->cost;
				min_pos = count;
			}
		}

		// swap the min value with the value at front pointer
		graph_node_t* temp = q->data[min_pos];
		q->data[min_pos] = q->data[q->front];
		q->data[q->front] = temp;

		graph_node_t* item;
		item = q->data[q->front];

		// *** Do we actually destroy the data or leave it as it is until it is replaced? ***
		// *** The pointer front back will signal skipping it but still it costs memory ***
		// to calculate how much the front pointer would need to move to the element to be deleted
		// Same thing with the back pointer
		// wrapping also occurs at front pointers
		q->front = (q->front + 1) % q->capacity;
		q->size--;
		return item;
	}
	// *** What is this? ***
	return NULL; // Note: This line is a filler so the code compiles.
}


// Queue Size
// Queries the current size of a queue
// A queue that has not been previously created will crash the program.
// (i.e. A NULL queue cannot return the size, call exit(1))
unsigned int queue_size(queue_t* q){
	// TODO: Implement me!
	if(q == NULL){
		exit(1);
	}
	return q->size;
}


// Free queue
// Removes a queue and all of its elements from memory.
// This should be called before the proram terminates.
void free_queue(queue_t* q){
	// TODO: Implement me!
	free(q->data);
	free(q);
}


#endif
