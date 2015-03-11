#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void my_error(char *err);


int main(int argc, char *argv[]){

  FILE *out;
  unsigned int seed;
  int num_ver,num_edge,i,L,L2;
  int *vertice;
  double my_rand;

  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <Outputfile> <numeroVerticiLato> <semeRand>\n", argv[0]);
    exit(1);
  } 

  out = (FILE*)fopen(argv[1],"w");
  num_ver = atoi(argv[2]);  
  seed = (unsigned)atoi(argv[3]);
  srand(seed);
  L2 = num_ver*num_ver;
  L = num_ver;
  
  num_edge =3*(L-1)*(L-1)+(L-1)*2;
  
  if(out == NULL){
    my_error("Impossibile scrivere su file.\n");
  }

  vertice = (int*)calloc(L2, sizeof(int));
  fprintf(out, "N = %d M = %d\n", L2, num_edge);

  for(i=0;i<L2-1;i++){
    if(i<L2-L){
      if((i+1)%L){
	my_rand = (double)rand()/RAND_MAX; 
	fprintf(out,"%d %d %f\n", i, i+1,my_rand);
	my_rand =  (double)rand()/RAND_MAX;
	fprintf(out,"%d %d %f\n", i, i+1+L,my_rand);
      }
      my_rand =  (double)rand()/RAND_MAX;
      fprintf(out,"%d %d %f\n", i, i+L,my_rand);
      
    }else{
      my_rand =  (double)rand()/RAND_MAX;
      fprintf(out,"%d %d %f\n", i, i+1,my_rand);
    }

  }
 
}

void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);

}
