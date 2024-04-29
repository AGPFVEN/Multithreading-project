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
      // open file
      int fd;
      if ((fd = open("file.txt", O_RDONLY)) == -1){
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
      char bf[2];
      bf[1] = '\0';
      char *endptr;

      // loop to read and store data
      while((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0)
      {
            // look for errors
            if (buffer_file_result == -1){
                  printf("Error at reading file\n");
                  return -1;
            }

            if (number_operations == 0){
                  if(*buffer_file != '\n'){
                        bf[0] = *buffer_file;
                        strcat(number_buffer, buffer_file);
                  } else {
                        number_operations = strtol(number_buffer, &endptr, 10);
                  }
            }
      }


      printf("%i\n", number_operations);
}
//gcc -o kk t.c