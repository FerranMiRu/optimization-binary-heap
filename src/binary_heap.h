#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

typedef struct {
    unsigned int position;
    double value;
    char id[4];
} Vertex;

typedef struct BHQueueElement {
    Vertex *vertex;
    struct BHQueueElement *parent;
    struct BHQueueElement *left_child, *right_child;
} BHQueueElement;

typedef struct BHQueue {
    unsigned int size;
    BHQueueElement *first;
} BHQueue;

void bh_enqueue(Vertex *, BHQueue *);
Vertex *bh_dequeue(BHQueue *);
void bh_requeue_element(Vertex *, float, BHQueue *);
void print_bh(BHQueue *);
void heapify_up(BHQueueElement *);
void heapify_down(BHQueueElement *);
void find_position_in_heap(unsigned int position, BHQueue *bh_queue,
                           BHQueueElement ***element_in_position_pointer,
                           BHQueueElement **parent_of_element_in_position);
void print_bh_element(BHQueueElement *);
void free_heap_queue_memory(BHQueue *);

#endif
