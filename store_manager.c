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

#include <errno.h>

#define BUFFER_SIZE 6

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
  ssize_t buffer_file_result;
  int const buffer_file_size = 1;
  char *buffer_file = (char *) malloc(buffer_file_size * sizeof(char));

  // declare variables to store data
  int number_operations = 0;
  char number_buffer[BUFFER_SIZE] = "";
  char *endptr;


  // loop to read and store data
  while((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0)
  {
    // look for errors
    if (buffer_file_result == -1){
      printf("Error at reading file\n");
      return -1;
    }

    // get number of operations
    if (number_operations == 0){
      if(*buffer_file != '\n'){
        strcat(number_buffer, buffer_file);
      } else {
        number_operations = strtol(number_buffer, &endptr, 10);
        if (errno != 0){
          perror("Error getting number of operations:");
        }
      }
    }
  }

  printf("%i\n", number_operations);


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

/*
./store_manager file.txt e e e
./store manager <file name><num producers><num consumers><buff size>
*/