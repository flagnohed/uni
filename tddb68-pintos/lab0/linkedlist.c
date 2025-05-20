//#include "linkedlist.h"

#include <stdlib.h>
#include <stdio.h>

struct list_item {
  int value;
  struct list_item *next;
};

typedef struct list_item item;

void append(item *first, int x){
  while (first->next != NULL){
    first = first->next;
  }
  item *added = malloc(sizeof(item));
  added->next = NULL;
  added->value = x;
  first->next = added;

}

void prepend(item *first, int x){
  item *fresh = malloc(sizeof(item));
  fresh->value = x;
  fresh->next = first->next;
  first->next = fresh;
}

void print(item *first){
  while (first != NULL){
    printf("Value: %d \n", first->value);
    first = first->next;

  }
}

void input_sorted(item *first, int x){
  while (first->next != NULL && first->next->value <= x){
    first = first->next;
  }
  prepend(first, x);
}

void clear(item *first){
  item *temp = first->next;
  first->next = NULL;
  while (temp != NULL){
    first = temp;
    temp = temp->next;
    free(first);
  }
}

int main(int argc, char ** argv){
  item root;
  root.value = -1;
  root.next = NULL;
  append(&root, 2);
  append(&root, 5);
  append(&root, 13);
  append(&root, 37);

  prepend(&root, 4);
  prepend(&root, 0);

  input_sorted(&root, 6);
  input_sorted(&root, 9);
  input_sorted(&root, 420);

  print(&root);
  clear(&root);
  print(&root); // ska -1 också bort?
  input_sorted(&root,2);
  print(&root);
  clear(&root);
  return 0;
}
