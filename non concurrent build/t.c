#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 6

int main(int argc, const char *argv[])
{
      queue *myq;

      int size = 10;
      int res; 
      myq = queue_init(size);
      size++;
      
      struct element local_elements[size];
      for (int i = 0; i < size; i++){
            //printf("i: %i\n", i);
            local_elements[i].op = (i + 2) * 10;
            local_elements[i].product_id = i * 20;
            local_elements[i].units = i * 30;
            res = queue_put(myq, &local_elements[i]);
            //queue_put(myq, &local_elements[i]);
            local_elements[i].op = i * 0;
      }

      struct element *loc;
      for (int i = 0; i < size; i++){
            printf("i: %i\n", i);
            printf("rear: %i, front %i\n", myq->rear, myq->front);
            loc = queue_get(myq);
            if (loc != NULL){
                  printf("op %i\n", (myq->elements)[i].op);
            }
            free(loc);
      }

      queue_destroy(myq);
}
//gcc -o kk t.c