#include "queue.h"

#include "utils.h"

void o_enqueue(BHQueueElement *bh_element, OQueue *o_queue) {
    OQueueElement *new_element;

    SAFE_MALLOC(new_element, 1);
    new_element->bh_queue_element = bh_element;
    new_element->next = NULL;

    if (o_queue->first == NULL) {
        o_queue->first = new_element;
        o_queue->last = new_element;
    } else {
        o_queue->last->next = new_element;
        o_queue->last = new_element;
    }
}
