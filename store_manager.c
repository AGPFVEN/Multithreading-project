// SSOO-P3 23/24

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

int main(int argc, const char *argv[])
{
  // check number of arguments
  if (argc < 5){
    printf("Too few arguments\n");
    return -1;
  }

  // open file
  int fd;
  if ((fd = open(argv[1], O_RDONLY)) == -1){
    printf("Error opening initial file\n");
    return -1;
  }

  // declare variables in order to read file
  const int buffer_file_size = 1;
  ssize_t buffer_file_result;
  char *buffer_file = (char *) malloc(buffer_file_size * sizeof(char));

  // declare variables to store data
  int number_operations = 0;
  const int max_number_length = 6;
  char number_buffer[max_number_length] = "";


  // loop to read and store data
  while((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0)
  {
    // look for errors
    if (buffer_file_result == -1){
      printf("Error at reading file\n");
      return -1;
    }

    if (number_operations == 0){
      if(buffer_file != '\n'){
        
      }
    }
  }



  int profits = 0;
  int product_stock[5] = {0};

  // Output
  printf("Total: %d euros\n", profits);
  printf("Stock:\n");
  printf("  Product 1: %d\n", product_stock[0]);
  printf("  Product 2: %d\n", product_stock[1]);
  printf("  Product 3: %d\n", product_stock[2]);
  printf("  Product 4: %d\n", product_stock[3]);
  printf("  Product 5: %d\n", product_stock[4]);

  return 0;
}
