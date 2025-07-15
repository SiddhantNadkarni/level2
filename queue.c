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


#include "queue.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL;

// Implement your queue functions here.
//

// Creates a new queue.
// PRECONDITION: Register malloc() and free() functions via the
//               queue_register_malloc() and
//               queue_register_free() functions.
// Returns a new linked_list on success, NULL on failure.
//
struct queue * queue_create(void){
    //check if malloc_fptr is NULL
    if(malloc_fptr == NULL){
        return NULL;
    }
    //allocate queue
    struct queue *queue = (struct queue *)malloc_fptr(sizeof(struct queue));

    //exit if allocation wasn't successful
    if(queue == NULL){
        return NULL;
    }

    //allocate underlying linkedlist
    queue->ll = linked_list_create();

    //exit if linkedlist allocation wasn't successful
    if(queue->ll == NULL){
        //free allocated queue before exit
        free_fptr(queue);
        return NULL;
    }

    //return queue if everything was successful
    return queue;

}

// Deletes a linked_list.
// \param queue : Pointer to queue to delete
// Returns TRUE on success, FALSE otherwise.
//
bool queue_delete(struct queue * queue){
    //check if input is NULL and free_fptr is NULL
    if(queue == NULL || free_fptr == NULL){
        return false;
    }

    //free all nodes of the underlying linkedlist
    //store result of operation
    bool successful = linked_list_delete(queue->ll);

    //free the allocated queue
    free_fptr(queue);

    //return result of linkedlist deletion
    return successful;
}

// Pushes an unsigned int onto the queue.
// \param queue : Pointer to queue.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool queue_push(struct queue * queue, unsigned int data){
    //check if queue is allocated and not NULL
    if(queue == NULL){
        return false;
    }

    bool success = linked_list_insert_end(queue->ll, data);
    if(success){
        queue->head_data = queue->ll->head->data;
    }

    //push at the tail (back) of linkedlist and return operation result
    return success;
}

// Pops an unsigned int from the queue, if one exists.
// \param queue       : Pointer to queue.
// \param popped_data : Pointer to popped data (provided by caller), if pop occurs.
// Returns TRUE on success, FALSE otherwise.
//
bool queue_pop(struct queue * queue, unsigned int * popped_data){
    //check if queue, underlying linkedlist and linkedlist head is allocated and not NULL
    if(queue == NULL || queue->ll == NULL || queue->ll->head == NULL || popped_data == NULL){
        return false;
    }

    //get data at head (front) of queue
    *popped_data = queue->head_data;

    bool success = linked_list_remove(queue->ll, 0);
    if(success && queue->ll->size > 0){
        queue->head_data = queue->ll->head->data;
    }

    //pop the head at index 0 and return operation result
    return success;
}

// Returns the size of the queue.
// \param queue : Pointer to queue.
// Returns size on success, SIZE_MAX otherwise.
//
size_t queue_size(struct queue * queue){
    //check if queue is allocated and not NULL
    if(queue == NULL){
        return SIZE_MAX;
    }

    //return underlying linkedlist size
    return linked_list_size(queue->ll);
}

// Returns whether an entry exists to be popped.
// \param queue: Pointer to queue.
// Returns TRUE if an entry can be popped, FALSE otherwise.
//
bool queue_has_next(struct queue * queue){
    //check if queue and underlying linkedlist is allocated and not NULL
    if(queue == NULL || queue->ll == NULL){
        return false;
    }

    //true if elements exist
    return queue->ll->size > 0;
}

// Returns the value at the head of the queue, but does
// not pop it.
// \param queue       : Pointer to queue.
// \param popped_data : Pointer to popped data (provided by caller), if pop occurs.
// Returns TRUE on success, FALSE otherwise.
//
bool queue_next(struct queue * queue, unsigned int * popped_data){
    //check if queue, underlying linkedlist and if linkedlist atleast 1 element and not NULL
    if(queue == NULL || queue->ll == NULL || queue->ll->size == 0 || popped_data == NULL){
        return false;
    }

    //get data at head of queue
    *popped_data = queue->head_data;

    return true;
}

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// POSTCONDITION: Initializes malloc() function pointer in linked_list.
// Returns TRUE on success, FALSE otherwise.
//
bool queue_register_malloc(void * (*malloc)(size_t)){
    //exit if input is NULL
    if(malloc == NULL){
        return false;
    }

    //set malloc_fptr to input function ptr and return
    malloc_fptr = malloc;

    //register malloc for underlying linkedlist
    linked_list_register_malloc(malloc);
    return true;
}

// Registers free() function.
// \param free : Function pointer to free()-like function.
// POSTCONDITION: Initializes free() functional pointer in linked_list.
// Returns TRUE on success, FALSE otherwise.
//
bool queue_register_free(void (*free)(void*)){
    //exit if input is NULL
    if(free == NULL){
        return false;
    }
    //set free_fptr to input function ptr and return
    free_fptr = free;

    //register free for underlying linkedlist
    linked_list_register_free(free);
    return true;
}
