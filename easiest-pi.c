/* easy program to implement montecarlo method to estimate the value of pi */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POINTS 1000000000  //the number of random points to be generated

int main()
{
  srand(time(NULL)); //setting the random seed
  double x,y; //the generated point coordinates

  unsigned long int belongs=0,executed=0;


  for(executed=0;executed<POINTS;executed++)
  {
    //generate the point
    x=rand()/(double)RAND_MAX;
    y=rand()/(double)RAND_MAX;
    //check if the generated point belongs to the circle or not
    if(x*x+y*y<=1)
    {
      belongs++;
    }
  }

  //print my result out
  printf("%lf\n", (4.0*belongs)/executed);

  return 0;
}
