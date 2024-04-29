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

      myq = queue_init(4);
      
      struct element o;
      o.op = 11;
      o.product_id = 12;
      o.units = 13;
      printf("%i\n", myq->rear);
      int res = queue_put(myq, &o);
      printf("%i\n", myq->rear);
      printf("%i\n", res);
      //printf("%i %i\n", myq->length, (myq->ir)[1]);
}
//gcc -o kk t.c