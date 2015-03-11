#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double T;

typedef struct{
  int val;            /*valore dello spin*/
}vertex;

typedef struct{
  vertex *spin;
  unsigned int L;
  int En;
  int magn;
}str_sis;


void init(str_sis *sis);
void mcs_step(str_sis *sis);
int prob(str_sis *sis, int i);


int main(int argc, char *argv[])
{
  unsigned int N_passi, i;
  unsigned int seed;
  str_sis *sis;
  FILE *out;

  sis = (str_sis *)malloc(1 * sizeof(str_sis));

  if (argc != 5) {
    fprintf(stderr, "usage: %s Temperatura Lato Numero_passi seed\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  T = (float)atof(argv[1]);
  sis->L = (unsigned int)atoi(argv[2]);
  N_passi = (unsigned int)atoi(argv[3]);
  seed = (unsigned int)atoi(argv[4]);
  srand(seed);

  sis->spin = (vertex *)malloc((sis->L * sis->L) * sizeof(vertex));

  out = fopen("E_m.dat","w");

  init(sis);

  fprintf(out,"%d\t%d\t%lf\n",0,sis->En,(double)sis->magn/(double)(sis->L * sis->L));

  for(i=0;i<N_passi;++i)
    {
      mcs_step(sis);
      fprintf(out,"%u\t%d\t%lf\n",(i + 1),sis->En,(double)sis->magn/(double)(sis->L * sis->L));
    }

  fclose(out);

  return EXIT_SUCCESS;
}


/**************************************************************************************************************************************/
void init(str_sis *sis)
{
  int i,j;
  float num;

  for(i=0;i<(sis->L);++i)
    {
      for(j=0;j<(sis->L);++j)
	{
	  num = (float)rand()/(RAND_MAX + 0.);
	  
	  /*scelta possibile spin a caso alte temperature*/
	   if(num > 0.5)sis->spin[i + j].val = 1;
	  else sis->spin[i + j].val = -1;
	  
	  /*altra scelta possibile tutti orientati nello stesso modo cioe' a basse temperature*/
	  /*spin[i + j].val = + 1;*/

	}
    }

  sis->En = 0;
  sis->magn = 0;

  for(i=0;i<(sis->L);++i)
    {
      for(j=0;j<(sis->L);++j)
	{
	  if(j != (sis->L - 1))sis->En -= (sis->spin[(i*sis->L) + j].val * sis->spin[(i*sis->L) + j + 1].val);
	  else sis->En -= (sis->spin[(i*sis->L) + j].val * sis->spin[(i*sis->L)].val);

	  if(i != (sis->L - 1))sis->En -= (sis->spin[(i*sis->L) + j].val * sis->spin[(i*sis->L) + j + sis->L].val);
	  else sis->En -= (sis->spin[(i*sis->L) + j].val * sis->spin[j].val);

	  sis->magn += sis->spin[(i*sis->L) + j].val;
	}
    }

  return;
}


/**************************************************************************************************************************************/
void mcs_step(str_sis *sis)
{
  int i,num, dEn;

  for(i=0;i<(sis->L * sis->L);++i)
    {
      num = 1 + (int)((float)(sis->L * sis->L) * rand()/(RAND_MAX + 1.0));

      dEn = prob(sis,num);

      if(dEn >= 0)
	{
	  if(sis->spin[num].val == 1)
	    {
	      sis->spin[num].val = -1;
	      sis->magn -= 2;
	    }
	  else 
	    {
	      sis->spin[num].val = 1;
	      sis->magn += 2;
	    }
	  sis->En += (-dEn);
	}
    }

  return;
}


/*************************************************************************************************************************************/
int prob(str_sis *sis, int i)
{
  int dE, x,y;
 
  dE = 0;
  y = (int)(i / sis->L);
  x = i % sis->L;

  if((x != (sis->L - 1)) && (x != 0))
    {
      dE += sis->spin[i + 1].val;
      dE += sis->spin[i - 1].val;

      if((y != (sis->L - 1)) && (y != 0))
	{
	  dE += sis->spin[i + sis->L].val;
	  dE += sis->spin[i - sis->L].val;
	}
      else if(y == (sis->L - 1))dE += sis->spin[x].val;
      else dE += sis->spin[((sis->L - 1)* sis ->L) + x].val;
    }
  else if(x == (sis->L - 1))
    {
      dE += sis->spin[(y * sis->L)].val;
      dE += sis->spin[i - 1].val;

      if((y != (sis->L - 1)) && (y != 0))
	{
	  dE += sis->spin[i + sis->L].val;
	  dE += sis->spin[i - sis->L].val;
	}
      else if(y == (sis->L - 1))dE += sis->spin[x].val;
      else dE += sis->spin[((sis->L - 1)* sis ->L) + x].val;
    }
  else
    {
      dE += sis->spin[i + 1].val;
      dE += sis->spin[y + (sis->L - 1)].val;

      if((y != (sis->L - 1)) && (y != 0))
	{
	  dE += sis->spin[i + sis->L].val;
	  dE += sis->spin[i - sis->L].val;
	}
      else if(y == (sis->L - 1))dE += sis->spin[x].val;
      else dE += sis->spin[((sis->L - 1)* sis ->L) + x].val;
    }

  dE = dE * 2 * sis->spin[i].val;

  return dE;
}
