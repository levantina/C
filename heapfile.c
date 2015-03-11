#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1000;

typedef double dataType;

typedef struct{
dataType *data;
int size, numData;
} dataStruct;

void heapify(dataStruct *heap, int index);
void createHeap(dataStruct *heap);
void heapSort(dataStruct *heap, FILE *outfl);
int ReadFile(dataStruct *heap, FILE *infl);

int main(int argc, char *argv[]){

  FILE *infl;
  FILE *outfl;

  dataStruct heap;
  heap.size = (int)MAX + 1;

  if(argc!=3){
    printf("\nERRORE\nUtilizzo: %s  inputfile outputfile\nIl file da leggere e' inputfile.\nI risultati sono scritti in outputfile.\n\n", argv[0]);
    exit(1);
  }

  heap.numData = 0;
  heap.data = malloc((heap.size)*sizeof(dataType));

  infl = (FILE*)fopen(argv[1], "r");
  outfl = (FILE*)fopen(argv[2], "w");

  if(readFile( &heap, infl)){
    printf("\nLettura file eseguita con successo.\n");
  }else{
    printf("\nLettura file fallita.\n");
    exit(0);
  }

  createHeap(&heap);

  printf("\n    HEAPSORT \n");
  if(outfl!=NULL){
  heapSort(&heap, outfl);
  printf("\nSrittura su file eseguita con successo.\n\n");
  }else{
    printf("\nImpossibile scrivere su file.\n");
    exit(0);
  }
  free(heap.data);
}

/*La funzione HEAPIFY verifica che sia rispettata la relazione fondamentale tra l'elemento padre della struttura e gli elementi figli, ovvero left=index*2 e right=index*2+1. Nel caso in cui uno dei due elementi figli sia maggiore del padre li scambia. E' ricorsiva perch? ad ogni scambio verifica nuovamente che la relazione sia soddisfatta.*/

void heapify(dataStruct *heap, int index){
  int left, right, max;
  dataType temp;

  left = 2*index;
  if(left <= heap->numData){
    if( heap->data[index] < heap->data[left]){
      max = left;
    }else {
      max = index;
    }
    right = left +1;
    if(right <= heap->numData){
      if(heap->data[right] > heap->data[max]){
/*max indica l'elemento temporaneamente a massima priorita'*/
	max = right;
      }
    }
    if(index!=max){
/*se il valore in index non e' il valore a massima priorita'(ovvero al posto giusto nella struttura) lo scambio con quello a massima priorita'rispetto a lui*/
      temp = heap->data[index];
      heap->data[index] = heap->data[max];
      heap->data[max] = temp;
      heapify(heap, max);
/*verifico che la nuova posizione sia corretta o meno chiamando di nuovo la funzione heapify*/
    }

  }

}

/*La funzione CREATEHEAP applica la relazione fondamentale di un 'HEAP' ad un array di numeri casuali*/

void createHeap(dataStruct *heap){
  int j;
  for(j=(int)(heap->numData/2);j>0;j--){
    heapify(heap,j);
  }

}

/*La funzione HEAPSORT ordina (ord.decrescente) i numeri nell'array precedentemente messi nell'ordine 'HEAP' sfruttandone la propriet? fondamente. Scrive direttamente su file.*/

void heapSort(dataStruct *heap, FILE *outfl){
  dataType tmp;
  int i;
  int dim = 1;
  int end;
  end = heap->numData +1;
  while(dim < end){
    tmp = heap->data[1];
    fprintf(outfl,"%lf\n", heap->data[1]);
    heap->data[1]=heap->data[heap->numData];
    heap->data[heap->numData]=tmp;
    --heap->numData;
    heapify(heap, 1);
    dim++;
  }
  fclose(outfl);
}

/*La funzione READFILE legge da file i valori da inserire nella struttura, in caso di dimensione maggiore alla dimensione predefinita viene riallocata memoria (il doppio) finch? non si raggiunge la dimensione sufficiente. E' inoltre presente un controllo sull'effettiva apertura del file.*/

int readFile(dataStruct *heap, FILE *infl){
  int i = 1;
  if(infl!=NULL){
    while((fscanf(infl, "%lf\n",&heap->data[i]))!=EOF){
      i++;
      heap->numData++;
      if(i==heap->size){
        heap->size=2*heap->size;
        heap->data = realloc(heap->data, heap->size*sizeof(dataType));
      }

    }
    return 1;
  }else{
    return 0;
  }

}

