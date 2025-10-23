#include "binary_heap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "utils.h"

void bh_enqueue(Vertex *vertex_to_add, BHQueue *bh_queue) {
    BHQueueElement *new_element, **last_element_pointer, *last_element_parent;

    bh_queue->size++;
    vertex_to_add->position = bh_queue->size;

    SAFE_MALLOC(new_element, 1);
    new_element->vertex = vertex_to_add;
    new_element->parent = NULL;
    new_element->left_child = NULL;
    new_element->right_child = NULL;

    if (bh_queue->first == NULL) {
        bh_queue->first = new_element;
        return;
    }

    find_position_in_heap(bh_queue->size, bh_queue, &last_element_pointer, &last_element_parent);
    *last_element_pointer = new_element;

    new_element->parent = last_element_parent;

    heapify_up(new_element);
}

Vertex *bh_dequeue(BHQueue *bh_queue) {
    Vertex *dequeued_vertex;
    BHQueueElement *element_to_free, **last_element_pointer, *_;

    if (bh_queue->first == NULL) {
        return NULL;
    }

    dequeued_vertex = bh_queue->first->vertex;

    find_position_in_heap(bh_queue->size, bh_queue, &last_element_pointer, &_);
    bh_queue->first->vertex = (*last_element_pointer)->vertex;
    bh_queue->first->vertex->position = 1;

    element_to_free = *last_element_pointer;
    *last_element_pointer = NULL;

    free(element_to_free);
    bh_queue->size--;

    heapify_down(bh_queue->first);

    return dequeued_vertex;
}

void bh_requeue_element(Vertex *vertex_to_change, float new_value, BHQueue *bh_queue) {
    BHQueueElement **bh_element_of_vertex, *_;

    find_position_in_heap(vertex_to_change->position, bh_queue, &bh_element_of_vertex, &_);

    if (new_value < vertex_to_change->value) {
        vertex_to_change->value = new_value;
        heapify_up(*bh_element_of_vertex);
    } else if (new_value > vertex_to_change->value) {
        vertex_to_change->value = new_value;
        heapify_down(*bh_element_of_vertex);
    }
}

void print_bh(BHQueue *bh_queue) {
    OQueue ordinary_queue;
    OQueueElement *aux_element;

    printf("Binary heap with %d elements and root node %s\n", bh_queue->size,
           bh_queue->first->vertex->id);

    // initialize the queue
    ordinary_queue.first = NULL;
    ordinary_queue.last = NULL;
    o_enqueue(bh_queue->first, &ordinary_queue);

    // with a for loop we ensure that we don't run into infinite iterations
    for (int i = 0; i < bh_queue->size; i++) {
        aux_element = ordinary_queue.first;
        print_bh_element(aux_element->bh_queue_element);

        // we add the children
        if (aux_element->bh_queue_element->left_child != NULL) {
            o_enqueue(aux_element->bh_queue_element->left_child, &ordinary_queue);
        }
        if (aux_element->bh_queue_element->right_child != NULL) {
            o_enqueue(aux_element->bh_queue_element->right_child, &ordinary_queue);
        }

        ordinary_queue.first = ordinary_queue.first->next;

        free(aux_element);
    }

    printf("\n");
    if (ordinary_queue.first != NULL) {
        perror("Error: Not all elements were printed!");
        exit(EXIT_FAILURE);
    }
}

void heapify_up(BHQueueElement *bh_element) {
    unsigned int aux_position;
    Vertex *aux_vertex;
    BHQueueElement *parent = bh_element->parent;

    if (parent == NULL) {
        return;
    } else if (bh_element->vertex->value < parent->vertex->value) {
        // first we swap the vertex necessary values
        aux_position = parent->vertex->position;
        parent->vertex->position = bh_element->vertex->position;
        bh_element->vertex->position = aux_position;

        // then we swap the vertexes
        aux_vertex = parent->vertex;
        parent->vertex = bh_element->vertex;
        bh_element->vertex = aux_vertex;

        heapify_up(parent);
    }
}

void heapify_down(BHQueueElement *bh_element) {
    unsigned int aux_position;
    Vertex *aux_vertex;
    BHQueueElement *smallest_child;

    // first we find the smallest child
    if (bh_element->left_child == NULL && bh_element->right_child == NULL) {
        return;
    } else if (bh_element->left_child == NULL && bh_element->right_child != NULL) {
        perror(
            "ERROR: we have found a case in which the left child is NULL and the right child is "
            "not");
        exit(EXIT_FAILURE);
    } else if (bh_element->left_child != NULL && bh_element->right_child == NULL) {
        smallest_child = bh_element->left_child;
    } else if (bh_element->left_child != NULL && bh_element->right_child != NULL) {
        smallest_child =
            bh_element->left_child->vertex->value <= bh_element->right_child->vertex->value
                ? bh_element->left_child
                : bh_element->right_child;
    } else {
        perror("ERROR: unforseen possibility when heapify_down");
        exit(EXIT_FAILURE);
    }

    if (bh_element->vertex->value <= smallest_child->vertex->value) {
        return;
    } else {
        // first we swap the vertex necessary values
        aux_position = smallest_child->vertex->position;
        smallest_child->vertex->position = bh_element->vertex->position;
        bh_element->vertex->position = aux_position;

        // then we swap the vertexes
        aux_vertex = smallest_child->vertex;
        smallest_child->vertex = bh_element->vertex;
        bh_element->vertex = aux_vertex;

        heapify_down(smallest_child);
    }
}

void find_position_in_heap(unsigned int position, BHQueue *bh_queue,
                           BHQueueElement ***element_in_position_pointer,
                           BHQueueElement **parent_of_element_in_position) {
    bool next_binary_bit_is_one, has_found_first_one = false;
    BHQueueElement *current_parent = bh_queue->first;

    for (int i = 32; i > 0; i--) {
        next_binary_bit_is_one = (position & (1 << (i - 1))) != 0;

        if (next_binary_bit_is_one) {
            if (!has_found_first_one) {
                // when we find the first 1, we set the boolean to true and skip
                has_found_first_one = true;
                continue;
            } else {
                // we found a "1" so we go right
                if (i > 1) {
                    current_parent = current_parent->right_child;
                } else if (i == 1) {
                    *parent_of_element_in_position = current_parent;
                    *element_in_position_pointer = &current_parent->right_child;
                }
            }
        } else {
            if (!has_found_first_one) {
                // while we don't find the first 1, we skip
                continue;
            } else {
                // we found a "0" so we go left
                if (i > 1) {
                    current_parent = current_parent->left_child;
                } else if (i == 1) {
                    *parent_of_element_in_position = current_parent;
                    *element_in_position_pointer = &current_parent->left_child;
                }
            }
        }
    }
}

void print_bh_element(BHQueueElement *element) {
    printf("Node %s, value = %lf", element->vertex->id, element->vertex->value);

    if (element->left_child != NULL) printf(", left child: %s", element->left_child->vertex->id);
    if (element->right_child != NULL) printf(", right child: %s", element->right_child->vertex->id);

    printf(".\n");
}

void free_heap_queue_memory(BHQueue *bh_queue) {
    free_heap_queue_element(bh_queue->first);

    bh_queue->first = NULL;
}

void free_heap_queue_element(BHQueueElement *element_to_free) {
    if (element_to_free == NULL) return;

    free_heap_queue_element(element_to_free->left_child);
    free_heap_queue_element(element_to_free->right_child);

    free(element_to_free);
}
