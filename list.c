// Linked list implementation ... COMP9024 20T2
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
   int v;
   int time;
   int cost;
   struct Node *next; 
} Node;

Node *makeNode(int v,int time,int cost) {
   Node *new = malloc(sizeof(Node));
   assert(new != NULL);
   new->time = time;
   new->cost = cost;
   new->next = NULL;
   new->v = v;
   return new;
}

List insertLL(List L,int v, int time,int cost) {

   Node *new = makeNode(v,time,cost);
   // add new node at the beginning
   if(L==NULL || time<=L->time){
      new->next = L;
   }
   //INSERT IN MIDDLE OR END
   else if(L!=NULL)
   {
      List temp = L;
      while(temp->next!=NULL && time>=temp->time){
         temp = temp->next;
      }
      if(temp->next==NULL){
         temp = new;
         new->next = NULL;
         return L;
      }
      new->next = temp->next;
      temp->next = new;
   }
   return L;
}

// List deleteLL(List L, int time,int cost) {
//    if (L == NULL) {
//       return L;
//    } else if (L->time == time) {
//       Node *p = L->next;
//       free(L);
//       return p;
//    } else {
//       L->next = deleteLL(L->next, time,cost);
//       return L;
//    }
// }

bool inLL(List L, int time,int cost) {
   if (L == NULL)
      return false;
   if (L->time == time && L->cost==cost)
     return true;
   if (L->time == time && L->cost!=cost)
     return false;
   
   return inLL(L->next, time,cost);
}

void showLL(List L) {
   if (L == NULL)
      putchar('\n');
   else {
      printf("(%d,", L->time);
      printf("%d)-> ", L->cost);
      showLL(L->next);
   }
}

// void freeLL(List L) {
//    if (L != NULL) {
//       freeLL(L->next);
//       free(L);
//    }
// }