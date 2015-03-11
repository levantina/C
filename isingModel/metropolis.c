#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L 5.0

double min(double pi, double pj);
double func(double j);

int main(int argc, char *argv[])
{
  double num;
  double pi,pj;
  double w_ij, i;
  double j,step = 0.0;
  unsigned int n, seed;
  FILE *out;

  if (argc != 2) {
    fprintf(stderr, "usage: %s seed\n", argv[0]);
    exit(EXIT_FAILURE);
  }
 
  seed = atoi(argv[1]);
  out = fopen("metropolis.dat", "w");
  srand(seed);
  i = 0.0;
  n = 0;

  for(n=0;n<1000000;++n)
    {
      num = (double)rand()/(RAND_MAX + 0.0);
      if(num > 0.5)step = 1.;
      else step = -1.;

      j = i + step;
      pi = func(i);
      pj = func(j);
      w_ij = min(pi,pj);

      num = (double)rand()/(RAND_MAX + 0.0);

      if(num < w_ij)
	{
	  i = j;
	}

      fprintf(out,"%u\t%f\n",n,i);
    }

  return EXIT_SUCCESS;
}


/****************************************************************************/
double min(double pi, double pj)
{
  double temp;

  temp = pj/pi;

  if(temp > 1.0)
    {
      return 1.0;
    }

  return temp;
}

/****************************************************************************/
double func(double j)
{
  double p;

  p = exp(-((j + L)*(j + L))/2.) + exp(-((j - L)*(j - L))/2.);

  return p;
}

/****************************************************************************/
