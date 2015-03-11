#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max_size 1000;

void swap(double *data1, double *data2);
void quicksort(int first, int last, double *data);


int main(int argc, char *argv[]){

  FILE *ifp;
  FILE *ofp;
  double *data;
  double datum;
  int i, j;
  int size, real_size;

  if(argc != 3){
    printf("\n   ERRORE\n\nUtilizzo: \n%s <inputfile> <outputfile>\nIl file da leggere e' inputfile\nI risultati vengono scritti in outputfile.\n\n", argv[0]);
    exit(1);
  }

  ifp = fopen(argv[1], "r");
  if(ifp == NULL){
    printf("\nImpossibile accedere al file < %s > in apertura.\n", argv[1]);
  }
  ofp = fopen(argv[2], "w");
  if(ofp == NULL){
    printf("\nImpossibile creare il file < %s >.\n", argv[2]);
    printf("  Formato non valido.\n");
  }

  i=0;
  size = max_size;
  data = (double*)malloc(size*sizeof(double));
  while(fscanf(ifp, "%lf", &datum) != EOF){
    data[i] = datum;
    real_size = i;
    ++i;
      if(i == size){
         size = size*2;
	 data = (double*)realloc(data,size*sizeof(double));
      }

  }
  printf("\nIl numero di dati è : %d\n\n    QUICKSORT\n", real_size+1);
  quicksort(0, real_size , data);

  for(j=0;j<=real_size;j++){
    fprintf(ofp, "%lf\n", data[j]);
  }
  fclose(ifp);
  fclose(ofp);
  printf("\nOra è possibile accedere al file in output.\n");
}


void swap(double *data1, double *data2) {
double temp;
 temp = *data1;
 *data1 = *data2;
 *data2 = temp;
}


void quicksort(int first, int last, double *data){
double cut;
int i,j;

if(first<last){
  cut = data[last];
  i = first;
  j = last;
    while(i<=j){
      while((i<=last)&&(data[i]<=cut)){
        i++;
      }
      while(data[j]>cut){
        j--;
      }
      if(i<j){
	swap(data+i,data+j);
        i++;
        j--;
      }
    }
    if(i<=last){
      quicksort(first, j, data);
      quicksort(i, last, data);
    }else{
      quicksort(first, last-1, data);
    }
}

}
