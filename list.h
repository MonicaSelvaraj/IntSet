// Linked list interface ... COMP9024 20T2
#include <stdbool.h>

typedef struct Node *List;

List insertLL(List, int,int, int);
List deleteLL(List, int,int);
bool inLL(List, int,int);
void freeLL(List);
void showLL(List);