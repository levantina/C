#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float dataType;

struct treeNode{
  dataType datum;
  struct treeNode *up, *left, *right;
};

typedef struct treeNode node;

int depth_tree(node *my_node);
int max_depth(node *tree, int num_node);
double ave_depth(node *tree, int num_node);
/*int* leaves(node *tree, int num_node);*/
void initNodeRand(node *my_node);
void insertNode(node *new, node *old);
void print_tree(node *tree, FILE *outfl);



int main(int argc, char *argv[]){
  
  FILE *outfl;
  node my_node, root, *tree, *new_node;
  unsigned int seed;
  int num_node, iteractions, max, control, i, *deep, j;
  int maxdepth =0; 
  dataType *new;
  double dat,avedepth=0;
  outfl = (FILE*)fopen("profondita_iter.txt", "w");

  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <maxNumeroNodi> <Iterazioni> <semeRand>\n", argv[0]);
    exit(1);
  }
  num_node = (unsigned)atoi(argv[1]);
  iteractions = (unsigned)atoi(argv[2]);
  seed = (unsigned)atoi(argv[3]);
  srand(seed);
  
  tree = malloc(num_node*sizeof(node));
  fprintf(outfl, "# N      <prof max>     <prof media>\n");
  max = num_node;
  
  for(j=1;j<=iteractions;j++){

    if(j!=1)fprintf(outfl,"\n");
    
    for(num_node=1;num_node<=max;num_node++){
      initNodeRand(&tree[0]);
      
      for(i=1;i<num_node;i++){
	initNodeRand(tree+i);
	insertNode(tree+i, &tree[0]);
	
      }
      maxdepth = max_depth(tree, num_node);
      avedepth = (double)ave_depth(tree, num_node);
      fprintf(outfl, "%d\t  %d\t  %f\t\n", num_node, maxdepth, avedepth); 
    }
    
  }
  
}


void initNodeRand(node *my_node){
  my_node->datum = (float)rand()/10000000;
  my_node->up = NULL;
  my_node->left = NULL;
  my_node->right = NULL;
}


void insertNode(node *new, node *old){
  if(new->datum<=old->datum){
    if(old->left == NULL){
      old->left = new;
      new->up = old;
    } else{
      insertNode(new, old->left);
    }
  } else{
    if(old->right == NULL){
      old->right = new;
      new->up = old;
    } else{
      insertNode(new, old->right);
    }
  }
}


int depth_tree(node *my_node){
  int depth = 1;
  while(my_node->up!=NULL){
    my_node = my_node->up;
    depth++;
  }
  return depth;
}

int max_depth(node *tree, int num_node){
  int i, max,tmp; 
  max=0;
  for(i=0;i<num_node;i++){
    tmp = depth_tree(tree+i);
    if(tmp>max){
      max = tmp;
    }
  }
  return max;
}

/*Questa funzione calcola la profondità media dei nodi dell'albero*/

double ave_depth(node *tree, int num_node){
  int i;
  double ave=0;
  for(i=0;i<num_node;i++){
    ave+=depth_tree(tree+i);
  }
  ave =(double) ave/num_node;
  return ave;
}


