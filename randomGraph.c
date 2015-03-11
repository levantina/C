#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct my_vertex{
  struct my_vertex* root;
  /*punta al vertice di riferimento del cluster di appartenenza*/
  int dim_cluster;
  /*dimensione cluster di appartenenza*/
  long int dim2_cluster;
  /*dimensione al quadrato*/
  int id_cluster; 
  /*serve ad identificare il cluster a cui appartiene il vertice*/

};

typedef struct{
  struct my_vertex* vertici;
  int N;
  int M;
}graph;


typedef struct my_vertex vertex;

void my_error(char *err);
void InitGraph(graph *grafo);
int extractVertex(graph *grafo);
int control(int index, graph *grafo);
void createEdges(graph* grafo);

int main(int argc, char *argv[]){
  FILE* out;
  vertex* tmp;
  graph *grafo;
  int num_ver;
  int max_num_edge;
  int i,j,num_cluster;
  unsigned int seed;
  
  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <OutputFile> <NumeroVertici> <seed>\n", argv[0]);
    exit(1);
  }
  
  out = (FILE*)fopen(argv[1], "w");
  
  if(out==NULL) my_error("Impossibile scrivere su file");
 
  num_ver = atoi(argv[2]);
  seed = (unsigned)atoi(argv[3]);
  srand(seed);
  grafo = (graph*)malloc(sizeof(graph));
  grafo->N = num_ver;
  grafo->M = num_ver;
  num_cluster = grafo->N;
  grafo->vertici = (vertex*)calloc(num_ver+1, sizeof(vertex));
 /*scelgo il valore massimo di M = N, avendo scelto il valore di C = 2M/N = 2*/
  
  InitGraph(grafo);
  
  createEdges(grafo);

  tmp = (vertex*)malloc(sizeof(vertex));

  for(i=1;i<grafo->N+1;i++){
    if(grafo->vertici[i].root == NULL){
      /*il vertice e' rimasto sconnesso, lo aggiorno di conseguenza*/
      grafo->vertici[i].root = &grafo->vertici[i];
    }
    if(grafo->vertici[i].root == grafo->vertici[i].root->root){
      printf("Per vertice %d-> Cluster ID: %d  Cluster Size: %d Cluster Size2: %ld\n", i,grafo->vertici[i].root->id_cluster, grafo->vertici[i].root->dim_cluster,grafo->vertici[i].root->dim2_cluster  );
      printf("\n");
    }else{
      tmp = grafo->vertici[i].root;
      while(tmp->root != tmp){
	tmp = tmp->root;
      }
      if(tmp->root == tmp){
	printf("Per vertice %d-> Cluster ID: %d  Cluster Size: %d Cluster Size2: %ld\n", i,tmp->id_cluster, tmp->dim_cluster, tmp->dim2_cluster);
	printf("\n");  
      } 
    }
    
    
    /*printf("cluster j = %d dimensione cluster %d -> %d dim2->%ld\n",j,grafo->vertici[j].root->id_cluster, grafo->vertici[j].root->dim_cluster,grafo->vertici[j].root->dim2_cluster );*/
    
  }
  
  
}



void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);
}


void InitGraph(graph *grafo){
  int i;
  /*inizializzo gli N vertici*/
  for(i=1;i<(grafo->N+1);i++){
    grafo->vertici[i].root = (vertex*)malloc(sizeof(vertex));
    grafo->vertici[i].root = NULL;
    /*metto a NULL i puntatori al vertice root di ciascun cluster (potrei anche fare un loop su se stesso, ma cosi' sarebbe equivocabile)*/ 
    grafo->vertici[i].dim_cluster = 1;
    /*esistono N cluster ciascuno di 1 elemento*/
    grafo->vertici[i].dim2_cluster = 1;
    /*dimensione al quadrato*/
    grafo->vertici[i].id_cluster = i;
    /*ognuno appartiene al cluster di se stesso*/
  }
  

}


int extractVertex(graph *grafo){
  double my_rand;
  double ver;
  my_rand = (double)(rand()/(double)RAND_MAX);
  ver = 1+(my_rand)*(double)(grafo->N) ;
  printf("rand: %d %f\n",(int)ver, my_rand);
  return (int)ver;
}

/*il problema e' che se estraggo un root e ne cambio il root, quelli che stavano nel suo cluster non hanno il root aggiornato, questo non va controllato solo nella stampa ma anche nella costruzione degli archi, con le dimensioni, senno' prendo le dimensioni sbagliate :(*/

int control(int index, graph *grafo){
   vertex *temp;
  temp = (vertex*)malloc(sizeof(vertex)); 
  temp = &grafo->vertici[index];
  
  while(temp!=temp->root){
    temp = temp->root;
    grafo->vertici[index].id_cluster = temp->id_cluster;
    grafo->vertici[index].dim_cluster = temp->dim_cluster;
    grafo->vertici[index].dim2_cluster = temp->dim2_cluster;
    grafo->vertici[index].root = temp;
  }
  printf("control %d ->id = %d\n",index, grafo->vertici[index].id_cluster);
  return grafo->vertici[index].id_cluster;
  
}


void createEdges(graph* grafo){
 int first,second;
 int i, num_cluster;
 long int prod;

 num_cluster = grafo->N;

 for(i=1;i<grafo->M+1;i++){
    first = extractVertex(grafo);
    second = extractVertex(grafo);
   
    if(first!=second){
      if(grafo->vertici[first].root == NULL && grafo->vertici[second].root == NULL){
	
	grafo->vertici[first].root = &(grafo->vertici[first]);
	grafo->vertici[second].root = grafo->vertici[first].root;
	grafo->vertici[second].id_cluster = grafo->vertici[first].id_cluster;
	grafo->vertici[first].root->dim_cluster++;
	grafo->vertici[first].root->dim2_cluster = 4;
	num_cluster--;

      }else if(grafo->vertici[first].root == NULL && grafo->vertici[second].root != NULL){
	  
	grafo->vertici[second].id_cluster = control(second, grafo);
	grafo->vertici[first].root = grafo->vertici[second].root;
	grafo->vertici[first].id_cluster = grafo->vertici[second].root->id_cluster;
	grafo->vertici[second].root->dim_cluster++;
	grafo->vertici[second].root->dim2_cluster = (grafo->vertici[second].root->dim_cluster)*(grafo->vertici[second].root->dim_cluster);
	num_cluster--;
	
      }else if(grafo->vertici[second].root == NULL && grafo->vertici[first].root != NULL){
	
	grafo->vertici[first].id_cluster = control(first, grafo);
	grafo->vertici[second].root = grafo->vertici[first].root;
	grafo->vertici[second].id_cluster = grafo->vertici[first].root->id_cluster;
	grafo->vertici[first].root->dim_cluster++;
	grafo->vertici[first].root->dim2_cluster = (grafo->vertici[first].root->dim_cluster)*(grafo->vertici[first].root->dim_cluster);
	
	num_cluster--;      
	
      }else if(control(first,grafo)!=control(second,grafo)){
	num_cluster--;
	
	if(grafo->vertici[first].root->dim_cluster >= grafo->vertici[second].root->dim_cluster){
	  
	  grafo->vertici[second].id_cluster = grafo->vertici[first].root->id_cluster;
	  prod = (long int)(grafo->vertici[first].root->dim_cluster)*(grafo->vertici[second].root->dim_cluster);
	  grafo->vertici[first].root->dim_cluster +=grafo->vertici[second].root->dim_cluster;
	  grafo->vertici[first].root->dim2_cluster +=grafo->vertici[second].root->dim2_cluster;
	  grafo->vertici[first].root->dim2_cluster+= 2*prod;
	  grafo->vertici[second].root = grafo->vertici[first].root;
	
	  printf("id cluster: second = %d first = %d\n", grafo->vertici[second].root->id_cluster, grafo->vertici[first].root->id_cluster);
	  
	}else if(grafo->vertici[first].root->dim_cluster < grafo->vertici[second].root->dim_cluster){
	  
	  grafo->vertici[first].id_cluster = grafo->vertici[second].root->id_cluster;
	  prod = (long int)(grafo->vertici[first].root->dim_cluster)*(grafo->vertici[second].root->dim_cluster);
	  grafo->vertici[second].root->dim_cluster +=grafo->vertici[first].root->dim_cluster;
	  grafo->vertici[second].root->dim2_cluster +=grafo->vertici[first].root->dim2_cluster;
	  grafo->vertici[second].root->dim2_cluster+= 2*prod;
	  grafo->vertici[first].root = grafo->vertici[second].root;
	  
	  printf("id cluster: first = %d second = %d\n", grafo->vertici[first].root->id_cluster, grafo->vertici[second].root->id_cluster);
	}
      }
	
      
      printf("Numero Archi = %d  Numero Cluster = %d\n", i, num_cluster);  
    }else{
      i--;
    }
    
    
 }
 
}
