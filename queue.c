//SSOO-P3 23/24
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

//To create a queue
queue* queue_init(int size)
{
  queue *q = (queue *)malloc(sizeof(queue));
  q->front = q->rear = -1;
  q->length = size;
  q->elements = (struct element *) malloc(sizeof(struct element) * size);

  return q;
}

// get position of the next "empty" element
int next_position(queue *q){
  return (q->rear + 1) % q->length;
}

// To Enqueue an element
int queue_put(queue *q, struct element* x)
{
  if (queue_full(q) == 1){
    return -1;
  }
  
  // data management
  int aux_int = next_position(q);
  (q->elements)[aux_int] = *x;

  // data structure management
  if (queue_empty(q) == 1)
  {
    q->front = 0;
    q->rear = 0;
  }
  else 
  {
    q->rear++;
  }

  // good result
  return 0;
}

// To Dequeue an element.
struct element* queue_get(queue *q)
{
  if(queue_empty(q) == 1){
    return NULL;
  }

  // data management
  struct element* element;
  element = (struct element*) malloc(sizeof(struct element));
  *element = (q->elements)[q->front];

  // data structure management
  if (q->rear == q->front){
    q->front = -1;
    q->rear = -1;
  }
  else
  {
    q->front++;
  } 

  // good result
  return element;
}

//To check queue state
int queue_empty(queue *q)
{  
  return (q->front == -1);
}

int queue_full(queue *q)
{ 
  return (((q->rear + 1) % q->length) == q->front);
}

//To destroy the queue and free the resources
int queue_destroy(queue *q)
{
  free(q->elements);
  free(q);
  return 0;
}


//printf("rear: %i, front %i\n", q->rear, q->front);
//printf("qop -> %i\n", (q->elements)[q->front].op);
//printf("qop -> %i\n", (q->elements)[q->front].op);
//(q->elements)[q->front].op = 9;
//printf("wop  9 ==%i\n", (q->elements)[q->front].op);
//printf("res -> %i\n", element->op);
  
//printf("rear: %i, front %i\n", myq->rear, myq->front);
//printf("op %i\n", (myq->elements)[i].op);
//printf("result: %i\n", res);

//element->op = (q->elements)[q->rear].op;
//element->product_id = (q->elements)[q->rear].product_id;
//element->units = (q->elements)[q->rear].units;