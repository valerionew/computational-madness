/*  based on the work of https://randu.org/tutorials/threads/  */
/* pthread implementation of montecarlo method to calculate pi */
/*                 for windows, non portable                   */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>

#define NUM_THREADS 8 // number of threads running
#define THREAD_POINTS 12500000 // points per thread
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

  // making sure that every thread gets a different set of ra
  srand(time(NULL)*data->tid);

  printf("Thread %d is running\n", data->tid); // just to say hi
  static const unsigned long int randmaxen=(((RAND_MAX+1.0)*(RAND_MAX+1.0))-1.0);
  double x,y,x1,x2,y1,y2;
  unsigned int executed=0; // stores the number of executed points
  data->belongs=0; //initalizes to 0 the counter of matched points

  // go through each point
  for(executed;executed<THREAD_POINTS;executed++){
    x1=rand();
    x2=rand();
    y1=rand();
    y2=rand();

    //the enhanced random formula
    x=x1*RAND_MAX_P1+x2;
    x=x/randmaxen;
    y=y1*RAND_MAX_P1+y2;
    y=y/randmaxen;

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
    printf("%u\n",thr_data[i].belongs);
  }

  // calculating  and printing the computational time
  time = clock() - time;
  printf("Total CPU time: %lf seconds\n", (double)time/(CLOCKS_PER_SEC));
  Beep(3000,500);

  return 0;
}
