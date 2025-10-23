#ifndef QUEUE_H
#define QUEUE_H

typedef struct BHQueueElement BHQueueElement;

typedef struct OQueueElement {
    BHQueueElement *bh_queue_element;
    struct OQueueElement *next;
} OQueueElement;

typedef struct OQueue {
    OQueueElement *first;
    OQueueElement *last;  // included for faster enqueue operations
} OQueue;

void o_enqueue(BHQueueElement *, OQueue *);

#endif
