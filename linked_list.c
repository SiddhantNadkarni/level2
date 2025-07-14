/*
*MIT License
*
*Copyright (c) 2025 Siddhant Nadkarni
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/


#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

// (void)(malloc_fptr);
// (void)(free_fptr);



// Creates a new linked_list.
// PRECONDITION: Register malloc() and free() functions via the
//               linked_list_register_malloc() and
//               linked_list_register_free() functions.
// POSTCONDITION: An empty linked_list has its head point to NULL.
// Returns a new linked_list on success, NULL on failure.
//
struct linked_list * linked_list_create(void){

    //check if malloc_fptr is NULL
    if(malloc_fptr == NULL){
        return NULL;
    }

    //allocate linked_list
    struct linked_list *ll = (struct linked_list *)malloc_fptr(sizeof(struct linked_list));

    //exit if allocation wasn't successful
    if(ll == NULL){
        return NULL;
    }

    //if allocation successful, init head, tail to NULL and size to 0
    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;

    return ll;
}

// Deletes a linked_list and frees all memory assoicated with it.
// \param ll : Pointer to linked_list to delete
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete(struct linked_list * ll){

    //check if input is NULL and free_fptr is NULL
    if(ll == NULL || free_fptr == NULL){
        return false;
    }

    //create pointers to head and next nodes
    //and free each node by iterating
    //through the entire list
    struct node* curr = ll->head;
    struct node* next = NULL;
    while(curr != NULL){
        //store next node
        next = curr->next;
        //free curr node
        free_fptr(curr);
        //traverse to next
        curr = next;
    }

    //finally, free the linked_list
    free_fptr(ll);

    return true;
}

// Returns the size of a linked_list.
// \param ll : Pointer to linked_list.
// Returns size on success, SIZE_MAX on failure.
//
size_t linked_list_size(struct linked_list * ll){

    //check if input is NULL
    if(ll == NULL){
        return SIZE_MAX;
    }

    //return counter
    return ll->size;

}

// Inserts an element at the end of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_end(struct linked_list * ll,
                            unsigned int data){

    //check if input is NULL and malloc_fptr is NULL
    if(ll == NULL || malloc_fptr == NULL){
        return false;
    }

    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }

    //init the new_node next and data
    new_node->data = data;
    new_node->next = NULL;

    //handle the edge case where its the first node in the linked_list
    if(ll->tail == NULL){
        //if its the first node, set both head and tail to new_node
        ll->tail = new_node;
        ll->head = new_node;
    }
    else{
        //set old node's next to new_node
        ll->tail->next = new_node;
        //update tail
        ll->tail = new_node;
    }

    //increase linked_list size
    ll->size++;

    return true;
}

// Inserts an element at the front of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data){

    //check if input is NULL and malloc_fptr is NULL
    if(ll == NULL || malloc_fptr == NULL){
        return false;
    }

    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }

    //init the new_node next and data
    new_node->data = data;
    new_node->next = NULL;

    //handle the edge case where its the first node in the linked_list
    if(ll->head == NULL){
        //if its the first node, set both head and tail to new_node
        ll->tail = new_node;
        ll->head = new_node;
    }
    else{
        //set new_node->next to current linked_list->head
        new_node->next = ll->head;
        //update head
        ll->head = new_node;
    }

    //increment linked_list size
    ll->size++;

    return true;
}

// Inserts an element at a specified index in the linked_list.
// \param ll    : Pointer to linked_list.
// \param index : Index to insert data at.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data){

    //check if input is NULLx malloc_fptr/free_fptr are NULL or if index is > linked_list size
    if(ll == NULL || malloc_fptr == NULL || free_fptr == NULL || index > ll->size){
        return false;
    }
    //if index to be inserted is at front, use linked_list_insert_front
    if(index == 0){
        return linked_list_insert_front(ll, data);
    }
    //if index to be inserted is at the end, use linked_list_insert_end
    else if(index == ll->size){
         return linked_list_insert_end(ll, data);
    }

    //create an iterator and populate it
    struct iterator iter;
    if(__linked_list_populate_iterator(ll, 0, &iter) == false){
        return false;
    }

    //iterate to the previous node of the index element i.e. to index - 1
    while(linked_list_iterate(&iter) && iter.current_index < index - 1){

    }

    //curr points to the previous node of the node to be deleted
    struct node* curr = iter.current_node;
    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }
    //set the node data
    new_node->data = data;
    //set new_node->next to curr
    new_node->next = curr->next;
    //set curr->next to new_node
    curr->next = new_node; //missed this
    //increase linked_list size
    ll->size++;

    return true;
}

// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data){

    //if input is NULL then return SIZE_MAX
    if(ll == NULL){
        return SIZE_MAX;
    }

    //create an iterator and populate it
    struct iterator iter;
    if(__linked_list_populate_iterator(ll, 0, &iter) == false){
        return SIZE_MAX;
    }

    //if first element is the data, then return
    if(iter.data == data){
        size_t curr_idx = iter.current_index;
        return curr_idx;
    }

    //iterate till we find the first occurence if its not the first element
    while(linked_list_iterate(&iter)){
        if(iter.data == data){
            size_t curr_idx = iter.current_index;
            return curr_idx;
        }
    }

    //no match found so return SIZE_MAX
    return SIZE_MAX;
}

// Removes a node from the linked_list at a specific index.
// \param ll    : Pointer to linked_list.
// \param index : Index to remove node.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_remove(struct linked_list * ll,
                        size_t index){

    //check if input is NULL or if free_fptr is NULL or if index to be removed
    //is greater than or equal to current linked_list size
    if(ll == NULL || free_fptr == NULL || index >= ll->size){
        return false;
    }

    //if index to be removed is the first
    if(index == 0){
        struct node* curr = ll->head;
        //edge case: only one element
        if(ll->size == 1){
            free_fptr(curr);
            ll->head = NULL;
            ll->tail = NULL;
        }
        else{
            //point head to the next node
            ll->head = ll->head->next;
            free_fptr(curr);
        }

        //decrement counter
        ll->size--;

        return true;
    }

    //create an iterator and populate it
    struct iterator iter;
    if(__linked_list_populate_iterator(ll, 0, &iter) == false){
        return false;
    }

    //iterate to the previous node of the index element i.e. to index - 1
    while(linked_list_iterate(&iter) && iter.current_index < index - 1){

    }

    //point curr to the previous node of index i.e. index - 1
    struct node* curr = iter.current_node;
    //point toBeDeteled to the node to be deleted at index
    struct node* toBeDeteled = curr->next;
    //handle edge case where node to be deleted is tail
    if(toBeDeteled == ll->tail){
        ll->tail = curr;
        ll->tail->next = NULL;
    }
    else{
        //else simply bypass the node toBeDeteled
        curr->next = toBeDeteled->next;
    }


    //free the node to be deleted
    free_fptr(toBeDeteled);
    //decrement size
    ll->size--;

    return true;
}

// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index){

    //check if input is NULL and malloc_fptr/free_fptr are NULL
    if(ll == NULL || malloc_fptr == NULL || free_fptr == NULL || index >= ll->size){
        return NULL;
    }

    //allocate iterator
    struct iterator* iterator = malloc_fptr(sizeof(struct iterator));
    //exit if allocation wasn't successful
    if(iterator == NULL){
        return NULL;
    }

    //counter for index tracker
    size_t curr_idx = 0;

    //traverse to node to set the iterator
    struct node* curr = ll->head;
    while(curr != NULL && curr_idx < index){
        curr = curr->next;
        curr_idx++;
    }

    //set the iterator fields
    iterator->ll = ll;
    iterator->current_node = curr;
    iterator->current_index = index;
    iterator->data = curr->data;

    //return the iterator
    return iterator;
}

// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter){

    //check if input iter is NULL and free_fptr is is NULL
    if(iter == NULL || free_fptr == NULL){
        return false;
    }

    //free the input iterator
    free_fptr(iter);

    return true;
}

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter){

    //check if input iterator, iter->current_node and iter->current_node->next are NULL
    if(iter == NULL || iter->current_node == NULL || iter->current_node->next == NULL){
        return false;
    }

    //update iterator to the next node
    iter->current_node = iter->current_node->next;
    iter->current_index++;
    iter->data = iter->current_node->data;

    return true;
}

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_malloc(void * (*malloc)(size_t)){

    //exit if input is NULL
    if(malloc == NULL){
        return false;
    }

    //set malloc_fptr to input function ptr and return
    malloc_fptr = malloc;
    return true;
}

// Registers free() function.
// \param free : Function pointer to free()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_free(void (*free)(void*)){

    //exit if input is NULL
    if(free == NULL){
        return false;
    }
    //set free_fptr to input function ptr and return
    free_fptr = free;
    return true;
}


// Populates iterators that are locally allocated
// on stack. Avoid malloc/free calls per iterator creation.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// \param iterator    : Pointer to Iterator allocated locally on stack.
// Returns TRUE on success, FALSE otherwise.
//
bool __linked_list_populate_iterator(struct linked_list* ll,
                                            size_t index,
                                            struct iterator* iterator){
    if(iterator == NULL || ll == NULL || index >= ll->size){
        return false;
    }

    //counter for index tracker
    size_t curr_idx = 0;

    //traverse to node to set the iterator
    struct node* curr = ll->head;
    while(curr != NULL && curr_idx < index){
        curr = curr->next;
        curr_idx++;
    }

    //set the iterator fields
    iterator->ll = ll;
    iterator->current_node = curr;
    iterator->current_index = index;
    iterator->data = curr->data;

    return true;
}