#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct my_vertex{

  int deg;
  int *neigh;
  double dist;
  int visit;
  int pos;
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

typedef struct{
  
  int *passi;
  int num_passi;
  int weight_dist;

}my_path;

void my_error(char *err);

void initDistGraph(graph *grafo);

void heapify(my_heap *Q,int index, graph *grafo);

void createHeap(my_heap *Q, graph *grafo);

void shortest_path(my_heap *Q, graph *grafo, int inizio);

void BuildGraph(FILE *infl, graph *grafo, my_heap *Q, int inizio, int fine,int iteraction, FILE* outfl);

void PrintGraph(FILE *outfl, graph *grafo);

void printPath(graph *grafo,my_path *min_path,int inizio,int fine);

void dist_diag(graph *grafo, my_path *min_path);

int main(int argc, char *argv[]){
  
  FILE *infl,*outfl;
  graph *grafo; 
  my_heap *Q;
  my_path *min_path;
  int inizio, fine, iteraction;

  if(argc!=6){
    printf("\nERRORE\nUtilizzo: %s <InputFile> <OutputFile> <InizioCammino> <FineCammino> <Iterazioni>\n", argv[0]);
    exit(1);
  }
  printf("\nCALCOLO DEL CAMMINO MINIMO IN UN GRAFO NON DIRETTO QUALSIASI\nValentina Biagini\n");
  infl = (FILE*)fopen(argv[1],"r");
  outfl = (FILE*)fopen(argv[2], "w");

  if(infl==NULL){
    my_error("Impossibile leggere da file.");
  }
  if(outfl==NULL){
    my_error("Impossibile scrivere su file.");
  }
  inizio = (int)atoi(argv[3]);
  fine = (int)atoi(argv[4]);
  iteraction = (int)atoi(argv[5]);

  grafo = (graph*)malloc(sizeof(graph));
  Q = (my_heap*)malloc(sizeof(my_heap));
  min_path = (my_path*)malloc(sizeof(my_path));
  fscanf(infl,"N = %d M = %d", &grafo->num_ver, &grafo->num_edge); 
  
  if(inizio>=grafo->num_ver){
    printf("Il vertice %d considerato non appartiene al grafo, dev'essere < %d.\n",inizio, grafo->num_ver);
    exit(0);
  }
  
  if(fine>=grafo->num_ver){
    printf("Il vertice %d considerato non appartiene al grafo, dev'essere < %d.\n", fine, grafo->num_ver);
    exit(0);
  }
  
  Q->size = grafo->num_ver +1;
  Q->data =(vertex*)calloc(Q->size,sizeof(vertex));

  BuildGraph(infl, grafo, Q, inizio,fine, iteraction, outfl);

  
  printf("\nNumero Vertici = %d\n",grafo->num_ver);
  printf("Numero Archi = %d\n",grafo->num_edge);
 
  /*PrintGraph(outfl, grafo);*/



  printPath(grafo,min_path,inizio,fine);

  /*dist_diag(grafo, min_path);*/

}

/*Funzione standard di errore*/  

void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);
}


/*Questa funzione inizializza le variabili della struct vertice per ogni vertice 
 *presente nella struct grafo esclusi gli array da allocare alla luce delle informazioni 
 *presenti sul file*/

void initDistGraph(graph *grafo){
  int i;
  for(i=0;i<grafo->num_ver;i++){
    grafo->ver[i].deg=0;
    grafo->ver[i].visit=0;
    grafo->ver[i].dist= grafo->num_ver; 
    
    /*Assegno un valore "infinito",cioè non raggiungibile,alla distanza dalla sorgente, 
     *sia questo valore il numero dei vertici*/
    
    grafo->ver[i].pred = (vertex*)malloc(sizeof(vertex));
    grafo->ver[i].pred = NULL;
    grafo->ver[i].pos = i;
  }
  
}
/*Questa funzione gestisce l'ordinamento nell'heap Q, in ordine dal più piccolo al più 
 *grande secondo le distanze dall'origine che hanno i vertici*/

void heapify(my_heap *Q, int index, graph *grafo){
  int left, right, min;
  vertex temp;

  if(Q->numData == 0)return;

  left = 2*index;
  if(left <= Q->numData){
    if( grafo->ver[Q->data[index].pos].dist > grafo->ver[Q->data[left].pos].dist){
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

/*Questa funzione ordina l'heap, qualora i suoi elementi vengano cambiati verifica che 
 *ognuno sia nella giusta posizione tramite heapify*/

void createHeap(my_heap *Q, graph *grafo){
   int i;
   for(i=(int)(Q->numData/2);i>0;i--){

     heapify(Q,i, grafo);
   }
   
}


/*Questa funzione è la parte più importante, tramite l'heap Q viene calcolato il percorso   *
 *minimo per raggiungere ogni vertice del grafo dato un inizio, la memoria di questo        *
 *percorso resta nelle struct di ciascun vertice con un puntatore al vertice che lo precede *
 *e il valore della sua distanza dalla sorgente*/

void shortest_path(my_heap *Q, graph *grafo, int inizio){
  int i,j;
  double d, temp;
  vertex u;
  d=0;
  
  /*Inizializzo i valori della sorgente*/   
  
  grafo->ver[inizio].dist = 0;
  grafo->ver[inizio].pred = NULL;
  
  /*All'inizio nella struct heap ci sono tutti i vertici, poichè tutti non sono stati toccati, 
   *il primo elemento sarà ver[inizio] perchè è l'unico inizalizzato al valore più piccolo*/
  
  for(i=1;(i<=grafo->num_ver);i++){
    Q->data[i]= grafo->ver[i-1];
  }
  
  Q->numData = grafo->num_ver;
  
  createHeap(Q, grafo);  /*ordino gli elementi nell'heap*/
 
  while(Q->numData>0){
    
    u = Q->data[1];
    
    if(u.deg==0 && grafo->ver[u.pos].dist == grafo->num_ver){
      /*il cammino finisce qui se non ho più vicini*/ 
      return; 
    }
    grafo->ver[u.pos].visit = 1;

    for(i=0;i<u.deg;i++){
      
      /*per ogni vicino verifico che non sia già stato visitato, in caso trovo il vicino con 
       *la distanza minore dalla sorgente e la aggiorno nella struct del vertice*/
      
      if(grafo->ver[u.neigh[i]].visit == 0){

	d = grafo->ver[u.pos].dist + grafo->ver[u.pos].weight[i];

	temp = grafo->ver[u.neigh[i]].dist; 
	
	if(d<temp){
	  grafo->ver[u.neigh[i]].dist = d;
	  grafo->ver[u.neigh[i]].pred = &grafo->ver[u.pos];

	}
      }
    }
    
    /* Ora l'elemento visitato viene estratto dall'heap (o meglio, cancellato) e il vettore 
     * viene riordinato dopo che le distanze sono cambiate. In questo modo saremo sicuri che 
     * Q->data[1] è il vertice tra i vertici connessi non visitati (visit = 0) con distanza 
     * minore dalla sorgente*/
  
    Q->data[1] = Q->data[Q->numData];
    Q->numData--;
    createHeap(Q,grafo);
 
  } 
}


/*Questa funzione costruisce il grafo nella struct leggendo da file, alloca i vettori 
 *neigh[] e weigh[] secondo il numero di vicini per ciascun vertice. Utilizza tre vettori 
 *temporanei d'appoggio la cui memoria alla fine viene liberata.*/

void BuildGraph(FILE *infl, graph *grafo, my_heap *Q, int inizio, int fine,int iteraction, FILE* outfl){ 
  int *temp1,*temp2,*Num_neigh;
  int i,j, step;
  double *pesi;
  vertex my_ver;

  grafo->ver = (vertex*)calloc(grafo->num_ver, sizeof(vertex));
  temp1 = (int*)calloc(grafo->num_edge, sizeof(int));
  temp2 =(int*)calloc(grafo->num_edge,sizeof(int));
  pesi = (double*)calloc(grafo->num_edge,sizeof(double));
  
  Num_neigh=(int*)calloc(grafo->num_ver,sizeof(int));
  /*grandezza dei vettori weight[] e neigh[]*/
  
  step = 0;
  fprintf(outfl,"#<Cammino>  <Passi> (NumeroVerticiLato = %d)\n",grafo->num_ver/grafo->num_ver);
  for(j=0;j<iteraction;j++){
    for(i=0;i<grafo->num_edge;i++){
      fscanf(infl, "%d %d %lf", &temp1[i],&temp2[i],&pesi[i]);
      
    }  
    
    for(i=0;i<grafo->num_edge;i++){
      Num_neigh[temp1[i]]++;
      Num_neigh[temp2[i]]++;
    }
    
    for(i=0;i<grafo->num_ver;i++){
      grafo->ver[i].neigh = (int*)calloc(Num_neigh[i],sizeof(int));  
      grafo->ver[i].weight = (double*)calloc(Num_neigh[i], sizeof(double));
    }
    
    initDistGraph(grafo);
    
    for(i=0;i<grafo->num_edge;i++){
      
      grafo->ver[temp1[i]].weight[grafo->ver[temp1[i]].deg] = (double)pesi[i];
      grafo->ver[temp1[i]].neigh[grafo->ver[temp1[i]].deg] = temp2[i];
      grafo->ver[temp1[i]].deg++;
      
      grafo->ver[temp2[i]].weight[grafo->ver[temp2[i]].deg] = (double)pesi[i]; 
      grafo->ver[temp2[i]].neigh[grafo->ver[temp2[i]].deg] = temp1[i];
      grafo->ver[temp2[i]].deg++;
    
      
    }
    
    free(temp1);
    free(temp2);
    free(pesi);
    
    shortest_path(Q, grafo,inizio);
    
    my_ver = grafo->ver[fine];

    while(my_ver.pos != inizio){

      my_ver = *grafo->ver[my_ver.pos].pred;
      step++;
    }

    fprintf(outfl,"%lf %d\n", grafo->ver[fine].dist, step);
  
  }
  
}
/*Questa funzione stampa su file per ogni vertice i suoi vicini, può far comodo come controllo*/

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



/*Questa funzione stampa i vertici per cui passa il cammino usando i puntatori al vertice 
 *precedete presenti nella struct*/

void printPath(graph *grafo,my_path *min_path,int inizio,int fine){
  int i=0;
  int path;
  vertex temp;
  
  min_path->num_passi=0;
  
  if(grafo->ver[fine].pred!=NULL){
    
    /*Nel caso in cui al vertice[fine] non viene assegnato nessun puntatore al precedente 
     *(valore NULL di inizializzazione) vuol dire che i vertici 'inizio' e 'fine' non 
     *appartengono allo stesso connesso*/
    
    min_path->passi = (int*)calloc(grafo->num_ver,sizeof(int));
    
    temp = grafo->ver[fine];

    while(temp.pos != inizio){
      min_path->passi[i]= temp.pos;
      temp = *grafo->ver[temp.pos].pred;
      i++;
    }
    min_path->num_passi = i;

    printf("\n#Lungezza cammino = %lf Numero Passi = %d\n", grafo->ver[fine].dist,min_path->num_passi);
    printf("#Cammino più breve tra i vertici %d e %d :\n", fine, inizio);
    for(i=0;i<min_path->num_passi;i++){
      printf("%d\n", min_path->passi[i]);  
      
    }
    printf("%d\n", grafo->ver[inizio].pos);
 
  }
  else{
    printf("\nI due vertici considerati non sono connessi.\n");
  }
}

void dist_diag(graph *grafo, my_path *min_path){
  int *diag;
  int i;
  int L;
  
  L = (int)grafo->num_ver/grafo->num_ver;
  diag = (int*)calloc(L, sizeof(int));
  diag[0]=0;
  
  for(i=1;i<L;i++){
    diag[i]=i+L-1;
    printf("diag i: %d\n", i, diag[i]);
  }


}
