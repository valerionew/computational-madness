/*  based on the work of https://randu.org/tutorials/threads/  */
/* pthread implementation of montecarlo method to calculate pi */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 8 // number of threads running
#define THREAD_POINTS 1250000000 // points per thread

// data passed to the thread
typedef struct _thread_data_t {
  int tid; // thread id given by the order of creation
  int belongs; // stores here the number of points that belong the circle
} thread_data_t;

// thread function
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;

  // making sure that every thread gets a different set of ra
  srand(time(NULL)*data->tid);

  printf("Thread %d is running\n", data->tid); // just to say hi

  double x,y; // store x and y of each point
  int executed=0; // stores the number of executed points
  data->belongs=0; //initalizes to 0 the counter of matched points

  // go through each point
  for(executed;executed<THREAD_POINTS;executed++){
    x=rand()/(double)RAND_MAX;
    y=rand()/(double)RAND_MAX;

    //chech if it belongs to the circle or not
    if(x*x+y*y<1){
      data->belongs++;
    }
  }

  // say goodbye. the datas are stored in the thr_data array
  pthread_exit(NULL);
}

int main(int argc, char **argv) {

  pthread_t thr[NUM_THREADS];
  int i, rc;

  // for calculating the computational time
  clock_t begin = clock();

  // creating the array of thr_data that will be passed to threads
  thread_data_t thr_data[NUM_THREADS];

  // creating the threads and handling some errors
  for (i = 0; i < NUM_THREADS; ++i) {
    thr_data[i].tid = i;
    if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
      fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
    }
  }
  // wait each thread to be over and print the number of matching points for each thread
  for (i = 0; i < NUM_THREADS; ++i) {
    pthread_join(thr[i], NULL);
    printf("%u\n",thr_data[i].belongs);
  }

  // calculating the computational time, not sure if it works
  clock_t end = clock();
  printf("Elapsed time: %lf seconds\n", (double)(end - begin)/CLOCKS_PER_SEC);

  return 0;
}
