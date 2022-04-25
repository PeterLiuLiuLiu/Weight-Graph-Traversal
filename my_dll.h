// =================== Support Code =================
// Doubly Linked List ( DLL ).
//
// - Do NOT just replace this file with your DLL file - some of the 
//     signatures have changed data types! Our data is now a void*
// - Implement each of the functions to create a working DLL.
// - Do not change any of the function declarations
//   - (i.e. dll_t* create_dll() should not have additional arguments)
//   - (You may write helper functions to help you debug your code such as print_list etc)
// ==================================================
#ifndef MYDLL_H
#define MYDLL_H

// Create a node data structure. 
// NOTE THE CHANGES from your original node - we are using a 
// void pointer for data so that we can use the same dll 
// but we're not storing integers anymore.
typedef struct node {
    void* data;
    struct node* next;
    struct node* previous;
} node_t;

// create a node_t with pointers of next and previous initialized to null, 
// data assigned to item
node_t* create_node(void* item){
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if(new_node == NULL){
		return NULL;
	}
	new_node->data = item;
	new_node->previous = NULL;
	new_node->next = NULL;
	return new_node;
}

// Create a DLL data structure
// Our DLL holds a pointer to the first node in our DLL called head,
// and a pointer to the last node in our DLL called tail.
typedef struct DLL {
    int count;              // count keeps track of how many items are in the DLL.
    node_t* head;           // head points to the first node in our DLL.
    node_t* tail;           //tail points to the last node in our DLL.
} dll_t;

// Creates a DLL
// Returns a pointer to a newly created DLL.
// The DLL should be initialized with data on the heap.
// (Think about what the means in terms of memory allocation)
// The DLLs fields should also be initialized to default values.
// Returns NULL if we could not allocate memory.
dll_t* create_dll(){
    // Modify the body of this function as needed.
   dll_t* myDLL = NULL;     

    // TODO: Implement me!!
	myDLL = (dll_t *)malloc(sizeof(dll_t));
	if(myDLL == NULL){
		return NULL;
	}
	myDLL->count = 0;
	myDLL->head = NULL;
	myDLL->tail = NULL;
		
	return myDLL;
}

// DLL Empty
// Check if the DLL is empty
// Exits if the DLL is NULL.
// Returns 1 if true (The DLL is completely empty)
// Returns 0 if false (the DLL has at least one element enqueued)
int dll_empty(dll_t* l){
    // TODO: Implement me!!
	if(l == NULL){
		return -1;
	}
	if(l->count == 0){
		return 1;
	}
	return 0;
}

// push a new item to the front of the DLL ( before the first node in the list).
// Exits if DLL is NULL.
// Returns 1 on success
// Returns 0 on failure ( i.e. we couldn't allocate memory for the new node)
// (i.e. the memory allocation for a new node failed).
int dll_push_front(dll_t* l, void* item){
	// TODO: Implement me!!
	if(l == NULL){
		return -1;
	}
	node_t *new_node = NULL;
    new_node = create_node(item);
	if(new_node == NULL){
		return 0;
	}

	// take into account the initial push
	if(l->count == 0){
		l->head = new_node;
		l->tail = new_node;
	}else{
		new_node->next = l->head;
		l->head->previous = new_node;
		l->head = new_node;
	}
	l->count++;
	return 1;
}

// push a new item to the end of the DLL (after the last node in the list).
// Exits if DLL is NULL.
// Returns 1 on success
// Returns 0 on failure ( i.e. we couldn't allocate memory for the new node)
// (i.e. the memory allocation for a new node failed).
int dll_push_back(dll_t* l, void* item){
	// TODO: Implement me!!
	if(l == NULL){
		return -1;
	}
	node_t *new_node = NULL;
    new_node = create_node(item);
	if(new_node == NULL){
		return 0;
	}

	// take into account the initial push
	if(l->count == 0){
		l->head = new_node;
		l->tail = new_node;
	}else{
		new_node->previous = l->tail;
		l->tail->next = new_node;
		l->tail = new_node;
	}
	l->count++;
	return 1;
}

// Returns the first item in the DLL and also removes it from the list.
// Exits if the DLL is NULL. 
// Returns NULL on failure, i.e. there is noting to pop from the list.
// Assume no negative numbers in the list or the number zero.
void* dll_pop_front(dll_t* t){
    // TODO: Implement me!!
	void* data;
	if(t == NULL){
		return (void*)-1;
	}
	if(t->count == 0){
		return (void*)0;
	}
	if(t->count == 1){
		data = t->head->data;   
		free(t->head);
		t->head = NULL;
		t->tail = NULL;
	}else{
		node_t* dump_node = NULL;
		dump_node = t->head;
		data = dump_node->data;
		t->head = dump_node->next;
		t->head->previous = NULL;

		dump_node->previous = NULL;
		dump_node->next = NULL;

		free(dump_node);
	}
	t->count--;
	return data;
}

// Returns the last item in the DLL, and also removes it from the list.
// Exits if the DLL is NULL. 
// Returns NULL on failure, i.e. there is noting to pop from the list.
// Assume no negative numbers in the list or the number zero.
void* dll_pop_back(dll_t* t){
    // TODO: Implement me!!
	void* data;
	if(t == NULL){
		return (void*)-1;
	}
	if(t->count == 0){
		return (void*)0;
	}
	if(t->count == 1){
   	data = t->head->data;
			free(t->tail);
      t->head = NULL;
      t->tail = NULL;
	}else{
		node_t* dump_node = NULL;
      dump_node = t->tail;
		data = dump_node->data;
		t->tail = dump_node->previous;
		t->tail->next = NULL;

		dump_node->previous = NULL;
		dump_node->next = NULL;

		free(dump_node);
   }
   t->count--;
   return data;
}

// Inserts a new node before the node at the specified position.
// Exits if the DLL is NULL
// Returns 1 on success
// Retruns 0 on failure:
//  * we couldn't allocate memory for the new node
//  * we tried to insert at a negative location.
//  * we tried to insert past the size of the list
//   (inserting at the size should be equivalent as calling push_back).
int dll_insert(dll_t* l, int pos, void* item){
	// TODO: Implement me!!
	if(l == NULL){
		return -1;
	}
	// pos starts from 0, acceptable range 0 to l->count, 
    // inserting at l->count means push back
	if(pos < 0 || pos > l->count){
		return 0;
	}
	if(pos == 0){
		return dll_push_front(l, item);
	}else if(pos == l->count){
		return dll_push_back(l, item);
	}else{
		// find that particular node address
		// counter starts from 1 since we need to offset the node by 1 to insert
		// i.e. insert at pos = 2 means insert betwteen node 1 and node 2
		// Only need to arrive node 1
		int counter = 1;
		node_t *current_node = l->head;
		while(counter < pos){
			current_node = current_node->next;
			counter++;
		}
		// arrived one node before
		node_t *new_node = NULL;
		new_node = create_node(item);
		if(new_node == NULL){
			return 0;
		}

		new_node->previous = current_node;
		new_node->next = current_node->next;
		current_node->next->previous = new_node;
		current_node->next = new_node;
	}
	l->count++;
	return 1;
}

// Returns the item at position pos starting at 0 ( 0 being the first item )
// Exits if the DLL is NULL
// Returns NULL on failure:
//  * we tried to get at a negative location.
//  * we tried to get past the size of the list
// Assume no negative numbers in the list or the number zero.
void* dll_get(dll_t* l, int pos){
	// TODO: Implement me!!
	if(l == NULL){
		return (void*)-1;
	}
	if(pos < 0 || pos >= l->count){
		// unsuccessful pos should be >= l->count since the last index is l->count
		return (void*)0;
	}
	int counter = 0;
	node_t *current_node = l->head;
	while(counter < pos){
		current_node = current_node->next;
		counter++;
	}
	return current_node->data;
}

// Removes the item at position pos starting at 0 ( 0 being the first item )
// Exits if the DLL is NULL
// Returns NULL on failure:
//  * we tried to remove at a negative location.
//  * we tried to remove get past the size of the list
// Assume no negative numbers in the list or the number zero.
void* dll_remove(dll_t* l, int pos){
	// TODO: Implement me!!
	// This function will return the item being removed
	void* data;
	if(l == NULL){
		return (void*)-1;
	}
	if(pos < 0 || pos >= l->count){
		return (void*)0;
	}

	if(pos == 0){
		return dll_pop_front(l);
	}else if(pos == (l->count - 1)){
		return dll_pop_back(l);
	}else{
		// find that particular node address
		// counter start from 1 since pos = 0 is checked and pop_front is used
		int counter = 1;
		node_t *dump_node = NULL;
      dump_node = l->head->next;
		while(counter < pos){
			dump_node = dump_node->next;
			counter++;
		}
		data = dump_node->data;

		dump_node->next->previous = dump_node->previous;
		dump_node->previous->next = dump_node->next;

		dump_node->previous = NULL;
		dump_node->next = NULL;

		free(dump_node);
		l->count--;
	}
	return data;
}

// DLL Size
// Exits if the DLL is NULL.
// Queries the current size of a DLL
int dll_size(dll_t* t){
	// TODO: Implement me!!
	if(t == NULL){
		return -1;
	}
	return t->count;
}

// Free DLL
// Exits if the DLL is NULL.
// Removes a DLL and all of its elements from memory.
// This should be called before the proram terminates.
void free_dll(dll_t* t){
	// TODO: Implement me!!
	if(t == NULL){
		return;
	}
    node_t* current_node = t->head;
	while(current_node != NULL){
        node_t* next = current_node->next; 
		free(current_node);
        current_node = next;	
    }
	free(t);
}

// To print the value of dll from start
void print_dll(dll_t* t){
	if(t == NULL){
		return;
	}
	int i;
	for (i = 0; i < t->count; i++){
		printf("%d  ", (int)dll_get(t, i));
	}
	printf("\n");
}

#endif
