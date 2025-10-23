// Ferran Mirabent Rubinat - 1528268
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_heap.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    char current_char;
    unsigned amount_vertexes = 0;
    FILE *data_file;
    Vertex *vertex_list, niu_vertex, *f_vertex = NULL;
    BHQueue main_queue;

    if (argc < 2) {
        printf("Error: need the name of the file.\n");
        return 1;
    }

    if ((data_file = fopen(argv[1], "r")) == NULL) {
        printf("Error: cannot open the file.\n");
        return 1;
    }
    while ((current_char = fgetc(data_file)) != EOF) {
        if (current_char == '\n') {
            amount_vertexes++;
        }
    }
    rewind(data_file);

    SAFE_MALLOC(vertex_list, amount_vertexes);

    for (int i = 0; i < amount_vertexes; i++) {
        fscanf(data_file, "%[^,],%lf\n", vertex_list[i].id, &vertex_list[i].value);
    }
    fclose(data_file);

    // initialize the binary heap
    main_queue.size = 0;
    main_queue.first = NULL;

    for (int i = 0; i < amount_vertexes; i++) {
        bh_enqueue(&vertex_list[i], &main_queue);
    }

    print_bh(&main_queue);

    strcpy(niu_vertex.id, "j");
    niu_vertex.value = 68;
    amount_vertexes++;

    bh_enqueue(&niu_vertex, &main_queue);
    print_bh(&main_queue);

    printf("Dequeued vertex: %s\n", bh_dequeue(&main_queue)->id);
    print_bh(&main_queue);

    for (int i = 0; i < amount_vertexes; i++) {
        if (strcmp(vertex_list[i].id, "f") == 0) {
            f_vertex = &vertex_list[i];
            break;
        }
    }

    if (f_vertex == NULL) {
        perror("ERROR: Couldn't find the vertex with id 'f'");
        exit(EXIT_FAILURE);
    }

    bh_requeue_element(f_vertex, f_vertex->value - 8, &main_queue);
    print_bh(&main_queue);

    free_heap_queue_memory(&main_queue);
    free(vertex_list);

    return 0;
}
