/*  based on the work of https://randu.org/tutorials/threads/  */
/* pthread implementation of montecarlo method to calculate pi */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 8 // number of threads running
#define THREAD_POINTS 1250000000L // points per thread

#define RAND_MAX_EN (((RAND_MAX+1.0)*(RAND_MAX+1.0))-1.0)
#define RAND_MAX_P1 RAND_MAX+1.0


// data passed to the thread5
typedef struct _thread_data_t {
  int tid; // thread id given by the order of creation
  int belongs; // stores here the number of points that belong the circle
} thread_data_t;


// thread function
void *thr_func(void *arg) {
  thread_data_t *data = (thread_data_t *)arg;

 // making sure that every thread gets a different set of random numbers
  int seed = time(NULL) + (1 + data->tid);

  printf("Thread %d is running\n", data->tid); // just to say hi
  static const unsigned long int randmax_big=(((RAND_MAX+1.0)*(RAND_MAX+1.0))-1.0);
  double const irandmax_big = 1. / randmax_big;


  double x,y,x1,x2,y1,y2;
  unsigned int executed=0; // stores the number of executed points
  int count = 0;

  // go through each point
  for(executed;executed<THREAD_POINTS;executed++){
    x1=rand_r(&seed);
    x2=rand_r(&seed);
    y1=rand_r(&seed);
    y2=rand_r(&seed);

    //the enhanced random formula
    x=x1*RAND_MAX_P1+x2;
    x = x * irandmax_big;
    y=y1*RAND_MAX_P1+y2;
    y = y * irandmax_big;

    //chech if it belongs to the circle or not
    if(x*x+y*y<1){
      ++count;
    }
  }

  data->belongs = count; //initalizes to 0 the counter of matched points
  // say goodbye. the datas are stored in the thr_data array
  pthread_exit(NULL);
}



int main(int argc, char **argv) {

  pthread_t thr[NUM_THREADS];
  int i, rc;

  unsigned long long int piPoints=0; //store the sum of all belonging points

  // to calculate the computational time
  clock_t time = clock();

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
    piPoints=thr_data[i].belongs+piPoints;
    printf("%u\n",thr_data[i].belongs);
  }

  // calculating and printing the computational time
  time = clock() - time;
  //CPU time in linux, clock time in windows
  //To get the clock time in linux divide time by CLOCKS_PER_SEC*NUM_THREADS
  printf("Total CPU time: %lf seconds\n%llu\a", (double)time/(CLOCKS_PER_SEC*NUM_THREADS),(piPoints*4));

  return 0;
}
