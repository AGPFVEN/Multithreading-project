//SSOO-P3 23/24
#ifndef HEADER_FILE
#define HEADER_FILE

struct element {
  int product_id; //Product identifier
  int op;         //Operation
  int units;      //Product units
};

typedef struct {
  // Define the struct yourself
  int front;
  int rear;
  int length;
  struct element *elements;
} queue;

//threads aclarar que debería ir en archivo a parte pero en el statemente pone "must"
struct th_args{
  int operations_per_th;
  int ith_from_file_array;
  int *profit;
  int *stock;
  queue *th_queue;
  struct element *th_elements_array;
};



queue* queue_init (int size);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif
