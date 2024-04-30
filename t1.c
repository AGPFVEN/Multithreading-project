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

int main(int argc, char *argv){
      int *c;

      c = (int *) malloc(10 * sizeof(int));

      for (int i=0;i<10;i++){
            c[i]=0;
      }

      free(c);
}