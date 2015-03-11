/*genero N grafi random con grado medio da immettere nella shell di valore intero, stampa su file*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef struct{
  int N;
  int M;
  int aveDeg;
  
}graph;


void my_error(char *err);
int extractVertex(graph *grafo);

int main(int argc, char *argv[]){
  FILE* out;
  int i, j, first, second, count = 0;
  unsigned int seed, iterate;
  graph *grafo;
  grafo = (graph*)calloc(1,sizeof(graph));

  if(argc!=6){
      printf("\nERRORE\nUtilizzo: %s <OutputFile> <NumeroVertici> <GradoMedio> <Iterazioni> <seed>\n", argv[0]);
      exit(1);
  }
  out = (FILE*)fopen(argv[1], "w");
  if(out==NULL) my_error("Impossibile scrivere su file");

  grafo->aveDeg = (unsigned)atoi(argv[3]);
  iterate = (unsigned)atoi(argv[4]);
  seed = (unsigned)atoi(argv[5]);
  grafo->N = (unsigned)atoi(argv[2]);
  grafo->M = (int) ((grafo->aveDeg*grafo->N)/2);
  srand(seed);
  
  fprintf(out,"#N = %d M = %d I = %d\n", grafo->N, grafo->M, iterate);
  
  for(j=0;j<iterate;j++){
  	count = grafo->M;
  	for(i=0;i<count;i++){
  	  first = extractVertex(grafo);
  	  second = extractVertex(grafo);
  	  if(first!=second){
  		fprintf(out,"%d\t%d\n", first, second);
  		}else{
  			count++;
  			}
  	
  	}
  	/*fprintf(out, "\n");*/
  }
  fclose(out);
  
}



void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);	
}


int extractVertex(graph *grafo){
  double my_rand;
  double ver;
  my_rand = (double)(rand()/(double)RAND_MAX);
  ver = (my_rand)*(double)(grafo->N) ;
  return (int)ver;
}

