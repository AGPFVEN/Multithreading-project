//SSOO-P3 23/24
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

struct product_info products[5];

//To create a queue
queue* queue_init(int size)
{
  queue *q = (queue *)malloc(sizeof(queue));
  q->front = q->rear = -1;
  q->length = size;
  q->elements = (struct element *) malloc(sizeof(struct element) * size);
  //q->ir = (int *) malloc(sizeof(int) * 2);
  //printf("%p\n", (q->ir));
  //(q->ir)[1] = 11;
  //printf("%i\n",  (q->ir)[1]);

  return q;
}

// get position of the next "empty" element
int next_position(queue *q){
  return (q->rear + 1) % q->length;
}

// To Enqueue an element
int queue_put(queue *q, struct element* x)
{
  // front and rear management
  if (q->rear = -1){
    q->front = 0;
    q->rear = 0;
  } else {
    q->rear++;
  }

  // put element in queue
  int aux_int = next_position(q);
  (q->elements)[aux_int] = *x;

  return 0;
}

// To Dequeue an element.
/*
struct element* queue_get(queue *q)
{
  struct element* element;
  
  return element;
}
*/

//To check queue state
int queue_empty(queue *q)
{
  
  return 0;
}

int queue_full(queue *q)
{
  
  return 0;
}

//To destroy the queue and free the resources
int queue_destroy(queue *q)
{

  return 0;
}
