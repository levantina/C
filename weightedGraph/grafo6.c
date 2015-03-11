#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct my_vertex{
  int deg;
  int *neigh;
  double dist;
  int visit;
  int pos;
  double weight_dist;
  double *weight;
  struct my_vertex *pred;

};

typedef struct my_graph{
  int num_ver, num_edge;
  struct my_vertex *ver;
  int count;
}graph;

typedef struct{
  struct my_vertex *data;
  int size;
  int numData;
}my_heap;

typedef struct my_vertex vertex;

void my_error(char *err);
void initDistGraph(graph *grafo);
void BuildGraph(FILE *infl, graph *grafo);
void PrintGraph(FILE *outfl, graph *grafo);
void heapify(my_heap *Q,int index, graph *grafo);
void createHeap(my_heap *Q, graph *grafo);
void shortest_path(my_heap *Q, graph *grafo, int inizio);
void printPath(graph *grafo,int inizio,int fine);


int main(int argc, char *argv[]){
  
  FILE *infl,*outfl;
  unsigned int seed;
  graph *grafo; 
  my_heap *Q;
  int i,j;
  int control=0;
  double my_rand;
  int inizio, fine;

  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <InputFile> <OutputFile> <semeRand>\n", argv[0]);
    exit(1);
  }

  infl = (FILE*)fopen(argv[1],"r");
  outfl = (FILE*)fopen(argv[2], "w");
  seed = (unsigned)atoi(argv[3]);
  srand(seed);

  if(infl==NULL){
    my_error("Impossibile leggere da file.");
  }
  if(outfl==NULL){
    my_error("Impossibile scrivere su file.");
  }
  printf("ora costruisco.");
  
  grafo = (graph*)malloc(sizeof(graph));
  Q = (my_heap*)malloc(sizeof(my_heap));

  BuildGraph(infl, grafo);

  Q->size = grafo->num_ver +1;
  Q->data =(vertex*)calloc(Q->size,sizeof(vertex));

  printf("\nNumero Vertici = %d\n",grafo->num_ver);
  printf("Numero Archi = %d\n",grafo->num_edge);
  printf("Inserire il vertice iniziale del cammino:");
  scanf("%d",&inizio);
  printf("\nVertice finale del cammino:");
  scanf("%d", &fine);
  
 
  printf("\nora stampo.\n");
  PrintGraph(outfl, grafo);
  printf("\ncammino");
  shortest_path(Q, grafo,inizio);
  printf("\nstampo cammino.\n");
  printPath(grafo,inizio,fine);

}

  

void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);
}

void initDistGraph(graph *grafo){
  int i;
  for(i=0;i<grafo->num_ver;i++){
    grafo->ver[i].deg=0;
    grafo->ver[i].visit=0;
    grafo->ver[i].dist= grafo->num_ver; 
/*Assegno un valore "infinito",cioè non raggiungibile,alladistanza dalla sorgente*/
    grafo->ver[i].weight_dist = grafo->num_ver;
    grafo->ver[i].neigh = (int*)calloc(grafo->ver[i].deg,sizeof(int));  
    grafo->ver[i].weight = (double*)calloc(grafo->ver[i].deg, sizeof(double));
    grafo->ver[i].pred = (vertex*)malloc(sizeof(vertex));
    grafo->ver[i].pred = NULL;
    grafo->ver[i].pos = i;
  }
  
}

void BuildGraph(FILE *infl, graph *grafo){ 
  int *temp;
  int i=0;
  int control=0;
  double my_rand;
 
  fscanf(infl,"N = %d M = %d", &grafo->num_ver, &grafo->num_edge); 
  grafo->ver = (vertex*)calloc(grafo->num_ver, sizeof(vertex));
  temp = (int*)calloc(2*grafo->num_edge, sizeof(int));

  i =0;
  while(fscanf(infl, "%d", &temp[i])!=EOF){
    control++;
    i++;
    printf("\n%d", i);
  }  
  printf("\n%d - NumVer\n%d - NumArchi\n", grafo->num_ver, grafo->num_edge);

  initDistGraph(grafo);

  for(i=0;i<grafo->num_edge*2;i+=2){
    printf("\nInserisco i pesi");
    my_rand = rand();  
    my_rand = my_rand/RAND_MAX;
    printf(" %f - ", my_rand);
    grafo->ver[temp[i]].weight[grafo->ver[temp[i]].deg] = my_rand;
    grafo->ver[temp[i+1]].weight[grafo->ver[temp[i+1]].deg] = my_rand; 
/*associo un peso (0,1] alla connessione tra i vertici*/

    grafo->ver[temp[i]].neigh[grafo->ver[temp[i]].deg] = temp[i+1];
    grafo->ver[temp[i+1]].neigh[grafo->ver[temp[i+1]].deg] = temp[i];
    grafo->ver[temp[i]].deg++;
    grafo->ver[temp[i+1]].deg++;
    
  }
  free(temp);
  
}

void PrintGraph(FILE *outfl, graph *grafo){
  int i,j;
  
  fprintf(outfl, "# Vertice --> Connessi Vicini (NumVertici = %d)\n", grafo->num_ver); 
  for(i=0;i<grafo->num_ver;i++){
    if(grafo->ver[i].deg!=0){
      fprintf(outfl,"%d -->",i);
      for(j=0;j<grafo->ver[i].deg;j++){
	fprintf(outfl, "%d ", grafo->ver[i].neigh[j]);    
	
      }
      fprintf(outfl, "\n");
    }
  }
  fclose(outfl);
}



void heapify(my_heap *Q, int index, graph *grafo){
  int left, right, min;
  vertex temp;

  printf("aperto heapify\n");

  if(Q->numData == 0)return;

  left = 2*index;
  if(left <= Q->numData){
    if( grafo->ver[Q->data[index].pos].dist > grafo->ver[ Q->data[left].pos].dist){
      min = left;
    }else {
      min = index;
    }
    right = left +1;
    if(right <= Q->numData){
      if(grafo->ver[Q->data[right].pos].dist < grafo->ver[Q->data[min].pos].dist){

	min = right;
      }
    }
    if(index!=min){

      temp = Q->data[index];
      Q->data[index] = Q->data[min];
      Q->data[min] = temp;
      heapify(Q, min, grafo);

    }

  }

}

void createHeap(my_heap *Q, graph *grafo){
   int i;
   for(i=(int)(Q->numData/2);i>0;i--){
     printf("\nfor del createHeap con i=%d\n",i);
     heapify(Q,i, grafo);
   }
   
 }



void shortest_path(my_heap *Q, graph *grafo, int inizio){
   int i,j;
   double d, temp;
   vertex u;
   d=0;
   /*Inizializzo i valori della sorgente*/   
   grafo->ver[inizio].weight_dist = 0;
   grafo->ver[inizio].dist = 0;
   grafo->ver[inizio].pred = NULL;
   for(i=1;(i<=grafo->num_ver);i++){
     Q->data[i]= grafo->ver[i-1];
   }
   Q->numData = grafo->num_ver;
   createHeap(Q, grafo);  
   j=0;
   while(Q->numData>0){

     j++;printf("While ciclo n:%d\n",j);

     u = Q->data[1];

     printf("Posizione: %d\n", Q->data[1].pos);

     if(u.deg==0 && grafo->ver[u.pos].dist == grafo->num_ver){
       /*il cammino finisce qui se non ho più vicini*/ 
       return; 
     }

       grafo->ver[u.pos].visit = 1;

       for(i=0;i<u.deg;i++){

	 if(grafo->ver[u.neigh[i]].visit == 0){
	   d = grafo->ver[u.pos].dist + u.weight[i];
	   printf("|distanza %f\n",d);
	   temp = grafo->ver[u.neigh[i]].dist; 

	   if(d<temp){
	     grafo->ver[u.neigh[i]].dist = d;
	     grafo->ver[u.neigh[i]].pred = &grafo->ver[u.pos];
	   }
	 }
       }
      
     Q->data[1] = Q->data[Q->numData];
     Q->numData--;
     heapify(Q,1, grafo);
     printf("NumData: %d\n", Q->numData);
   }
  
}

void printPath(graph *grafo,int inizio,int fine){
  int i=0;
  vertex temp;
  if(grafo->ver[fine].pred!=NULL){
  printf("\nLungezza cammino: %f", grafo->ver[fine].dist);
  printf("\nCammino più breve tra i vertici %d e %d :\n", fine, inizio);
  temp = grafo->ver[fine];

  while(temp.pos != inizio){
    i++;
    printf("%d\n", temp.pos);
    temp = *grafo->ver[temp.pos].pred;
  }
  printf("%d\n", grafo->ver[inizio].pos);
  printf("Numero di passi: %d\n", i);
  }
  else{
    printf("\nI due vertici considerati non sono connessi.\n");
  }
}
