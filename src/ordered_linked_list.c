// Reads a file with two fields organized with two columns separated by comma.
// Organizes the data as a sorted linked list.
// Prints the linked list.
// Removes first element of the linked list and prints again the linked list.
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

// The data we will deal with are vertices with just:
// an id, which is up to 4 characters.
// a value (we will use it to order the list).
typedef struct {
    char id[5];
    double value;
} Vertex;

// Defines a node in the ordered queue (a linked list node) which contains:
// a pointer to a Vertex and
// a pointer to the next element.
typedef struct OQueueElement {
    Vertex *vertex;
    struct OQueueElement *next;
} OQueueElement;

// Defines the structure of an ordered queue:
// only keeps a pointer to the first element
typedef struct {
    OQueueElement *first;
} OQueue;

int Oenqueue(Vertex *, OQueue *);
Vertex Odequeue(OQueue *);
void printOqueue(OQueue);

int main(int argc, char *argv[]) {
    FILE *datafile;
    char ll;
    Vertex *vertexlist;
    unsigned vertexnumber = 0;

    if (argc < 2) {
        printf("Error: need the name of the file.\n");
        return 1;
    }
    datafile = fopen(argv[1], "r");
    if (datafile == NULL) {
        printf("Error: cannot open the file.\n");
        return 1;
    }
    while ((ll = fgetc(datafile)) != EOF) {
        if (ll == '\n') {
            vertexnumber++;
        }
    }
    rewind(datafile);
    // After rewinding the file, it allocates memory for the vertices and reads them into the
    // 'vertexlist' array.
    if ((vertexlist = (Vertex *)malloc(vertexnumber * sizeof(Vertex))) == NULL) {
        printf("Error: cannot allocate memory for vertexs.\n");
        return 1;
    }
    for (int i = 0; i < vertexnumber; i++) {
        fscanf(datafile, "%[^,],%lf\n", vertexlist[i].id, &vertexlist[i].value);
    }
    fclose(datafile);
    OQueue vertexoqueue;
    vertexoqueue.first = NULL;
    for (int i = 0; i < vertexnumber; i++) {
        if (Oenqueue(&vertexlist[i], &vertexoqueue) != 0) {
            printf("Error: problem adding elements to the queue.\n");
            return 666;
        }
    }
    printOqueue(vertexoqueue);
    Vertex v = Odequeue(&vertexoqueue);
    printf("We have extracted vertex %s from the queue.\n", v.id);
    printOqueue(vertexoqueue);
    return 0;
}

// The function Oenqueue inserts vertices into an ordered queue, keeping the elements sorted by the
// value field of the Vertex.
int Oenqueue(Vertex *vert, OQueue *Q) {
    OQueueElement *element_aux, *element_iter;
    element_aux = (OQueueElement *)malloc(sizeof(OQueueElement));
    if (element_aux == NULL) {
        printf("Error: cannot allocate memory for queue element.");
        return 1;
    }
    element_aux->vertex = vert;
    element_aux->next = NULL;
    if (Q->first == NULL) {
        Q->first = element_aux;
        return 0;
    }
    if (Q->first->vertex->value > element_aux->vertex->value) {
        element_aux->next = Q->first;
        Q->first = element_aux;
        return 0;
    }
    element_iter = Q->first;
    while (element_iter->next != NULL && element_iter->next->vertex->value < vert->value)
        element_iter = element_iter->next;
    element_aux->next = element_iter->next;
    element_iter->next = element_aux;
    return 0;
}

// The Odequeue function removes and returns the first vertex from the queue.
Vertex Odequeue(OQueue *Q) {
    Vertex v;
    if (Q->first == NULL) {  // there is no queue!
        v.id[0] = 0;
    }
    OQueueElement *element_aux = Q->first;
    v = *(element_aux->vertex);
    Q->first = Q->first->next;
    free(element_aux);
    return v;
}

// The printOqueue function prints the elements of an ordered queue.
void printOqueue(OQueue Q) {
    OQueueElement *element_iter = Q.first;
    while (element_iter != NULL) {
        printf("Vertex id = %s, value = %lf.\n", element_iter->vertex->id,
               element_iter->vertex->value);
        element_iter = element_iter->next;
    }
}
