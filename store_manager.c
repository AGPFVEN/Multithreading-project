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

//---------BC of purchase
#define BUFFER_SIZE 9

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
  while(((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0) && (number_operations == 0))
  {
    // look for errors
    if (buffer_file_result == -1){
      printf("Error at reading file\n");
      return -1;
    }

    // get number of operations
    if(*buffer_file != '\n'){
      strcat(number_buffer, buffer_file);
    } else {
      number_operations = strtol(number_buffer, &endptr, 10);
      strcpy(number_buffer, "");
      if (errno != 0){
        perror("Error getting number of operations:");
      }
    }    
  }

  struct element all_elements[number_operations];
  int read_counter = 0, current_element = 0; 

  while((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0){
    /*El diseño del loop lo estoy haciendo así por flexibilidad,
    si en algún momento se quieren añadir más operaciones se puede hacer más fácil que
    con el aproach de según la primera letra sacar la operación y hacer lseek*/
    // get elements
    if((*buffer_file != ' ') && (*buffer_file != '\n')){
      strcat(number_buffer, buffer_file);
    } else {
      // get product_id
      if (read_counter == 0) {
        all_elements[current_element].product_id = strtol(number_buffer, &endptr, 10);
        if (errno != 0){
          perror("Error getting number of operations:");
        } 

      // get operation number (0 -> purchase, 1 -> sale)
      } else if (read_counter == 1) {
        if (strcmp(number_buffer, "PURCHASE") == 0)
          all_elements[current_element].op = 0;
          
        else if (strcmp(number_buffer, "SALE") == 0)
          all_elements[current_element].op = 1;

        else {
          printf("Error reading operation number %i: %s\n", current_element, number_buffer);
          return -1;
        }

      // get number of units
      } else if (read_counter == 2){
        all_elements[current_element].units = strtol(number_buffer, &endptr, 10);
        if (errno != 0){
          perror("Error getting number of operations:");
        } 
      }

      // new lines and spaces handling
      if (*buffer_file == '\n')
      {
        current_element++;
        printf("n: %i, id: %i, op: %i, units: %i\n", current_element, all_elements[current_element].product_id, all_elements[current_element].op, all_elements[current_element].units);
      }
      else 
        read_counter++;

      //reset word
      strcpy(number_buffer, ""); 
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