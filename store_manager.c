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
#include <time.h>

//---------BC of purchase
#define BUFFER_SIZE 9

int product_cost[5];
int product_price[5];

pthread_mutex_t access_mutex;
pthread_cond_t non_full;
pthread_cond_t non_empty;


/*Lo hice así para que cada thread acceda a memoria haciendo que sea más eficiente
en vez de que el main se encargara de elements[i] y pasarle el element directamente al producer*/
void *t_producer(void *v_args){
  // enqueue element
  struct th_args args = *(struct th_args*) v_args;
  //printf("thread: %i\n", args.th_elements_array[args.ith_from_file_array].units);

  for(int i = args.ith_from_file_array; i < args.ith_from_file_array + args.operations_per_th; i++)
  {
    pthread_mutex_lock(&access_mutex);
    while (queue_full(args.th_queue)){
      pthread_cond_wait(&non_full, &access_mutex);
    }
    int result = queue_put(args.th_queue, &(args.th_elements_array[i]));
    //printf("p %i of %i id: %i\n", i, args.ith_from_file_array + args.operations_per_th, args.ith_from_file_array);
    //printf("%i\n", queue_full(args.th_queue));
    if (result != 0){
      printf("Error putting in queue the element number n%i\n", args.ith_from_file_array);
    }
    pthread_cond_signal(&non_empty);
    pthread_mutex_unlock(&access_mutex);
  }

  return NULL;
}

void *t_consumer(void *v_args){
  struct th_args args = *(struct th_args*) v_args;

  for(int i = args.ith_from_file_array; i < args.ith_from_file_array + args.operations_per_th; i++)
  {
    pthread_mutex_lock(&access_mutex);
    while (queue_empty(args.th_queue)){
      pthread_cond_wait(&non_empty, &access_mutex);
    }
    struct element *consumer_element = queue_get(args.th_queue);
    //printf("c %i of %i id: %i\n", i, args.ith_from_file_array + args.operations_per_th, args.ith_from_file_array);
    //printf("%i\n", queue_empty(args.th_queue));
    // calculate results
    switch (consumer_element->op)
    {
    case 0:
      *(args.profit) -= consumer_element->units * product_cost[(consumer_element->product_id) - 1]; 
      (args.stock)[consumer_element->product_id - 1] += consumer_element->units;
      break;
  
    case 1:
      *(args.profit) += consumer_element->units * product_price[(consumer_element->product_id) - 1]; 
      (args.stock)[consumer_element->product_id - 1] -= consumer_element->units;
      break;
  
    default:
      break;
    }
    //if (consumer_element == NULL){
      //printf("Error putting in queue the element number n%i\n", args.ith_from_file_array);
    //}
    pthread_cond_signal(&non_full);
    pthread_mutex_unlock(&access_mutex);
  }

  return NULL;
}

int producer(int i, struct element elements[], queue *producer_queue){
  // enqueue element
  int result = queue_put(producer_queue, &elements[i]);
  if (result != 0){
    printf("Error putting in queue the element number n%i\n", i);
  }

  return 0;
}

int consumer(int *consumer_profit, int consumer_stock[], queue *consumer_queue){
  // dequeue element
  struct element *consumer_element;
  consumer_element = queue_get(consumer_queue);

  // calculate results
  switch (consumer_element->op)
  {
  case 0:
    *consumer_profit -= consumer_element->units * product_cost[(consumer_element->product_id) - 1]; 
    consumer_stock[consumer_element->product_id - 1] += consumer_element->units;
    break;
  
  case 1:
    *consumer_profit += consumer_element->units * product_price[(consumer_element->product_id) - 1]; 
    consumer_stock[consumer_element->product_id - 1] -= consumer_element->units;
    break;
  
  default:
    break;
  }
  return 0;
}

int main(int argc, const char *argv[])
{

  // check number of arguments
  if (argc != 5){
    printf("Incorrect number of arguments arguments\n");
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
  while((number_operations == 0) && ((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0))
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

  //fix this imperfection
  (buffer_file_result = read(fd, buffer_file, buffer_file_size));
  struct element all_elements[number_operations];
  int read_counter = 0, current_element = 0; 

  do{
    /*El diseño del loop lo estoy haciendo así por flexibilidad,
    si en algún momento se quieren añadir más operaciones se puede hacer más fácil que
    con el aproach de según la primera letra sacar la operación y hacer lseek*/
    
    /*Decidí guardar las operaciones en ints en vez de en strings para ahorrar memoría sin
    comprometer la flexibilidad*/

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
        //printf("n: %i, id: %i, op: %i, units: %i\n", current_element, all_elements[current_element].product_id, all_elements[current_element].op, all_elements[current_element].units);
        current_element++;
        read_counter = 0;
      }
      else{
        read_counter++;
      }

      //reset word
      strcpy(number_buffer, "");
    }
  } while(((buffer_file_result = read(fd, buffer_file, buffer_file_size)) != 0) && (current_element < number_operations));

  // set costs and prices (id -= 1)
  product_cost[0] = 2;
  product_cost[1] = 5;
  product_cost[2] = 15;
  product_cost[3] = 25;
  product_cost[4] = 100;
  product_price[0] = 3;
  product_price[1] = 10;
  product_price[2] = 20;
  product_price[3] = 40;
  product_price[4] = 125;

  // measure time for fun
  //clock_t t = clock();

  //not final -------------------------------------
  int thread_num_p = 1;
  //int buffer_queue = 10;
  int thread_num_c = 1; 

  //declare useful vars
  pthread_t producers[atoi(argv[2])];
  pthread_t consumers[atoi(argv[3])];
  queue *store_queue;
  //store_queue = queue_init(buffer_queue);
  store_queue = queue_init(atoi(argv[4]));
  //int res;

  pthread_mutex_init(&access_mutex, NULL);
  pthread_cond_init(&non_full, NULL);
  pthread_cond_init(&non_empty, NULL);

  //crating producers
  for (int i=0; i<thread_num_p; i++)
  {
    struct th_args *p_args = malloc(sizeof(struct th_args)); 
    p_args->operations_per_th = number_operations / thread_num_p;
    p_args->ith_from_file_array = i * p_args->operations_per_th;

    //check if op is divisible with thread num
    if((i == thread_num_p - 1) && (number_operations % thread_num_p != 0)){
      p_args->operations_per_th += number_operations % thread_num_p;
    }
    
    p_args->th_queue = store_queue;

    p_args->th_elements_array = all_elements;

    pthread_create(&(producers[i]), NULL, &t_producer, p_args);
    //res = producer(i, all_elements, store_queue);
    //if (res != 0){
      //printf("Error putting in queue the element number n%i\n", i);
    //}
  }

  //-------------------
  //for (int i=0; i<thread_num_p;i++){
    //pthread_join(producers[i], NULL);
  //}

  int profits = 0;
  int product_stock[5] = {0};

  for (int i=0; i<thread_num_c; i++){
    struct th_args *c_args = malloc(sizeof(struct th_args)); 
    c_args->operations_per_th = number_operations / thread_num_c;
    c_args->ith_from_file_array = i * c_args->operations_per_th;

    //check if op is divisible with thread num
    if((i == thread_num_c - 1) && (number_operations % thread_num_c != 0)){
      c_args->operations_per_th += number_operations % thread_num_c;
    }
    
    c_args->th_queue = store_queue;

    c_args->th_elements_array = all_elements;

    //pthread_create(&(producers[i]), NULL, &t_producer, c_args);

    c_args->profit = &profits;
    c_args->stock = product_stock;

    pthread_create(&(consumers[i]), NULL, &t_consumer, c_args);
    //res = consumer(&profits, product_stock, store_queue);
    //if (res != 0){
      //printf("Error putting in queue the element number n%i\n", i);
    //}
  }

  for (int i=0; i<thread_num_c;i++){
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
  }

  //t = clock() -t;
  //double time_taken = ((double) t)/CLOCKS_PER_SEC;
  //printf("time taken %f\n", time_taken);

  //// Output
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