/*programma che legge da file i dati Dimes e li randomizza con diverse percentuali prima di analizzare la struttura a cores*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct my_vertex{
  int size;
  int deg;
  int new_deg;
  int coreness;
  int core_number; 
  int *neigh;
  int index;
  int cut; 
  /*cut indica se il vertice  stato o meno tagliato durante il k-pruning*/
 };

typedef struct{
  int N, M;
  struct my_vertex *ver;
  int maxDeg, minDeg;
}graph;

typedef struct{
  int size, numData, front, rear;
  int *Q;	
}queue;

typedef struct my_vertex vertex;

void my_error(char *err);
int extractInt(int n);
graph *readGraph(FILE *inputf);
graph *randomize(graph *grafo, int times);
int *degDistribution(graph *grafo);
FILE *printDistribution(graph *grafo, int *d, FILE *dfile);
void addElement(queue *coda, int new);
void delElement(queue *coda);
void makeCut(graph *grafo, int v);
graph *createCores(graph *grafo, FILE *kfile, int control);
graph *liberaTutti(graph *grafo);


	
int main(int argc, char *argv[]){
	int i,j,c;
	unsigned percent;
	FILE *kfile, *inputf;
	graph *grafo;
	srand(time(NULL));
	
	if(argc!=4){
      printf("\nERRORE\nUtilizzo: %s  <input file> <distribuzione dei gradi? (si=1 no=0)> <percentuale di randomizzazione>\n", argv[0]);
      exit(1);
      }
    c = (unsigned)atoi(argv[2]);
    if((c!=1)&&(c!=0)) my_error("Per calcolare la distribuzione dei gradi scrivere 1, altrimenti 0");
    
    inputf = (FILE*)fopen(argv[1], "r");
    if(inputf==NULL){
    	my_error("Impossibile leggere il file");
    	}
    kfile = (FILE*)fopen("kdati_rand.txt", "w");
    if(kfile==NULL){
    	my_error("Impossibile scrivere risultati su file");
    	}
	percent = (unsigned)atoi(argv[3]);
	
	grafo = readGraph(inputf);
	fprintf(kfile,"#k-Shell: N = %d M = %d percentRandom = %d\n#k   numVer   numEdges\n", grafo->N, grafo->M, percent);
	grafo = randomize(grafo, percent);
	fflush(stdout);
	grafo = createCores(grafo, kfile, c);
	grafo = liberaTutti(grafo);
  
	fclose(kfile);
}

void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);
}

int extractInt(int n){
  double my_rand;
  double ver;
  my_rand = (double)(rand()/(double)RAND_MAX);
  ver = (my_rand)*(double)(n) ;
  return (int)ver;
}

graph *readGraph(FILE *inputf){
	graph *graphp;
	int i,j,first,second,n,m;
	graphp=(graph *)malloc(sizeof(graph));
	if(graphp == NULL){
		my_error("Malloc di graphp in lettura fallita");
	}
	graphp->N=36000;
	graphp->M=63000;
	graphp->maxDeg=0;
	graphp->minDeg=36000;
	graphp->ver=(vertex *)malloc(graphp->N*sizeof(vertex));
	if(graphp->ver == NULL){
		my_error("Malloc di graphp->ver fallita");
	}
	for(i=0;i<graphp->N;i++){
		graphp->ver[i].deg=0;
		graphp->ver[i].new_deg=0;
		graphp->ver[i].index=i;
		graphp->ver[i].cut=1;
		graphp->ver[i].size=4*graphp->M/graphp->N;
		graphp->ver[i].neigh=(int *)malloc(graphp->ver[i].size*sizeof(int));
	}
		
    n=0;
    m=0;

	while((fscanf(inputf, "%d %d\n", &first, &second))!=EOF){
			++m;
			if(first>=n){
				n = first;
			}else if(second>=n){
				n = second;
			}
			
			if((first>=graphp->N)||(second>=graphp->N)){
				fflush(stdout);
				graphp->ver = (vertex *)realloc(graphp->ver, 2*n*sizeof(vertex));
				for(j=graphp->N;j<n*2;j++){
				  graphp->ver[j].deg=0;
		          graphp->ver[j].new_deg=0;
				  graphp->ver[j].index=j;
				  graphp->ver[j].cut=1;
				  graphp->ver[j].size=4*graphp->M/graphp->N;
			      graphp->ver[j].neigh=(int *)malloc(graphp->ver[j].size*sizeof(int));
				}
		    graphp->N = n*2;
		    }

			if(graphp->ver[first].deg+2>graphp->ver[first].size){
  			  graphp->ver[first].size*=2;
  			  graphp->ver[first].neigh=(int *)realloc(graphp->ver[first].neigh,graphp->ver[first].size*sizeof(int));
  			}
  		    if(graphp->ver[second].deg+2>graphp->ver[second].size){
  			  graphp->ver[second].size*=2;
  			  graphp->ver[second].neigh=(int *)realloc(graphp->ver[second].neigh,graphp->ver[second].size*sizeof(int));
  			}
  		
  		  graphp->ver[first].neigh[graphp->ver[first].deg]=second;
  		  graphp->ver[second].neigh[graphp->ver[second].deg]=first;
		  graphp->ver[first].deg++;
  		  graphp->ver[second].deg++;
  		  graphp->ver[first].new_deg++;
  		  graphp->ver[second].new_deg++;
  		
  		  if(graphp->ver[first].deg>graphp->maxDeg){
           	graphp->maxDeg = graphp->ver[first].deg;
          }else if(graphp->ver[second].deg>graphp->maxDeg){
            graphp->maxDeg = graphp->ver[second].deg;
            }
      	  if(graphp->ver[first].deg<graphp->minDeg){
            graphp->minDeg = graphp->ver[first].deg;
          }else if(graphp->ver[second].deg<graphp->minDeg){
            graphp->minDeg = graphp->ver[second].deg;
            }   
  		

				
		}
	    graphp->N = n+1;
		graphp->M = m;
	  	
  	return graphp;
}

graph *randomize(graph *grafo, int times){
	int v1,v2,u1,u2;
	int i,j,deg1,deg2;
	times = times*grafo->N/400;
	
	for(j=0;j<times;j++){
	  v1 = extractInt(grafo->N);
	  v2 = extractInt(grafo->N);
	  if(v1==v2){
	  	v2 = extractInt(grafo->N);
	  	}
	  if((grafo->ver[v1].deg!=0)&&(grafo->ver[v2].deg!=0)){
	  	deg1 = extractInt(grafo->ver[v1].deg);
	  	u1 = grafo->ver[v1].neigh[deg1];
	    grafo->ver[v1].neigh[deg1] = v2;
	    deg2 = extractInt(grafo->ver[v2].deg);
	    u2 = grafo->ver[v2].neigh[deg2];
	    grafo->ver[v2].neigh[deg2] = v1;
	    for(i=0;i<grafo->ver[u1].deg;i++){
	      if(grafo->ver[u1].neigh[i]==v1) grafo->ver[u1].neigh[i] = u2;
	    }
	    for(i=0;i<grafo->ver[u2].deg;i++){
	      if(grafo->ver[u2].neigh[i]==v2) grafo->ver[u2].neigh[i] = u1;
		}
      
      }else times++;
	  	  }
	return grafo;
	}
	
int *degDistribution(graph *grafo){
  int *distr,i,j;
  distr = (int*)malloc(grafo->maxDeg*sizeof(int));
  for(i=0;i<grafo->maxDeg;i++){
	distr[i]=0;
  }
  for(j=0;j<grafo->N;j++){
    if(grafo->ver[j].cut==1){
    	distr[grafo->ver[j].deg]++;	
    }
	
  }

  return distr;	
}

FILE *printDistribution(graph *grafo, int *d, FILE *dfile){
  int i;
  for(i=0;i<grafo->maxDeg;i++){
  	fprintf(dfile, "%d\t%d\n", i, d[i]);
  }	
  fprintf(dfile, "\n");
  return dfile;
}

	
void addElement(queue *coda, int new){
  if(coda->rear == coda->size){
  	coda->size = coda->size*2;
  	coda->Q = (int*)realloc(coda->Q, coda->size*sizeof(int));	
  	}
  coda->rear = coda->rear+1;
  coda->numData++;
  coda->Q[coda->rear] = new;
	
}

void delElement(queue *coda){
  if(coda->front == coda->size){
  	coda->size = coda->size*2;
  	coda->Q = (int*)realloc(coda->Q, coda->size*sizeof(int));
  	}
  coda->front = coda->front+1;
  coda->numData--;
}

void makeCut(graph *grafo, int v){
  grafo->ver[v].cut = 0; 
  /*il vertice viene segnato cut=0 perchè fa parte della coda rimuovibili durante il k-pruning*/	
}

graph *createCores(graph *grafo, FILE *kfile, int control){
  int i,j,k;
  int *ddistr;
  int num = 0, max;
  int inizio, vicini;
  int end = 0;
  int *shell, *edges;
  queue *coda;
  FILE *ddfile;
  
  if(control==1){
  	ddfile = (FILE*)fopen("degDistr_rand.txt","w");
    if(ddfile == NULL) my_error("Impossibile creare il file degDistr_rand.txt");
  }
  shell = (int*)malloc((grafo->maxDeg+1)*sizeof(int));
  if(shell == NULL) my_error("Malloc di shell fallita");
  edges = (int*)malloc((grafo->maxDeg+1)*sizeof(int));
  if(edges == NULL) my_error("Malloc di edges fallita");
  coda = (queue*)malloc(1*sizeof(queue));
  if(coda == NULL) my_error("Malloc di coda fallita");
  coda->size = grafo->N*10; 
  coda->Q = (int*)malloc(coda->size*sizeof(int));
  if(coda->Q == NULL) my_error("Malloc di coda->Q fallita");
  
  for(i=0;i<coda->size;i++){
	coda->Q[i]=0;
	}
  inizio = 0;
  vicini = 0;
  
  for(i=0;i<=grafo->maxDeg;i++){
  	shell[i]=0;
  	edges[i]=0;
  	}
    
  for(i=0;i<grafo->N;i++){
  	if(grafo->ver[i].deg<1 && grafo->ver[i].cut==1){
  		shell[0]++;
  		makeCut(grafo, grafo->ver[i].index);
  		}
  	}
  if(control==1){
  	ddistr = degDistribution(grafo);
    ddfile = printDistribution(grafo, ddistr, ddfile);
    free(ddistr);
  }
  
  for(k=2;k<=grafo->maxDeg;k++){
  	coda->numData = 0;
    coda->front = 0;
    coda->rear = 0;
  	max = 0;
  	num = 0;
  	for(i=0;i<grafo->N;i++){
  		if(grafo->ver[i].deg>max){
  			max = grafo->ver[i].deg;
  		}
  		if(grafo->ver[i].deg<k && grafo->ver[i].cut==1){
  			addElement(coda, grafo->ver[i].index);
  			makeCut(grafo, grafo->ver[i].index);
  		}
  	}
  	
  	if(max<k){
  		shell[k-1] = coda->numData;
  		break;
  		}
  		
  	while(coda->numData>0){
  	inizio = coda->Q[coda->front];
  	edges[k-1] += grafo->ver[inizio].deg;
   		for(i=0;i<grafo->ver[inizio].deg;i++){
   			vicini = grafo->ver[inizio].neigh[i];
  			for(j=0;j<grafo->ver[vicini].deg;j++){
  				if(grafo->ver[vicini].neigh[j] == inizio){
  					end = grafo->ver[vicini].deg-1;
  				    grafo->ver[vicini].neigh[j] = grafo->ver[vicini].neigh[end];  				    
  				    --grafo->ver[vicini].deg;
  				    }
  				
  				}
   			if(grafo->ver[vicini].deg<k && grafo->ver[vicini].cut==1){
  				addElement(coda, vicini);
  				makeCut(grafo, vicini);
  				}
  			
  			}
  		
  		grafo->ver[inizio].deg = 0;
  		delElement(coda);
  		num++;
  		}
  	
  	shell[k-1] = num;
  	  if(control==1){
  		ddistr = degDistribution(grafo);
        ddfile = printDistribution(grafo, ddistr, ddfile);
        free(ddistr);
      }  

  	} 
   
   for(i=0;i<=grafo->maxDeg;i++){
   	  if(shell[i]!=0){
   	  	fprintf(kfile,"%d\t%d\t%d\n", i, shell[i], edges[i]);
   	  	}
   	
   	}
   	fprintf(kfile,"\n");
    free(shell);
   	free(edges);
   	free(coda->Q);
   	free(coda);
   	
   	return grafo;
}

graph *liberaTutti(graph *grafo){
	int i;
	    for(i=0;i<grafo->N;i++){
		  free(grafo->ver[i].neigh);
		}
    free(grafo->ver);
    free(grafo);
	fflush(stdout);
    return grafo;
}
