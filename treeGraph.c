#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float dataType;

struct treeNode{
  dataType datum;
  struct treeNode *up, *left, *right;
};

typedef struct treeNode node;

dataType* my_rand(unsigned int seed, int num_node);
int depth_tree(node *my_node);
int max_depth(node *tree, int num_node);
double ave_depth(node *tree, int num_node);
int* leaves(node *tree, int num_node);
void initNode(node *my_node);
void initTree(node *tree, int lenght);
void insertNode(node *new, node *old);
void print_tree(node *tree, FILE *outfl);



int main(int argc, char *argv[]){
  
  FILE *outfl;
  node my_node, root, *tree, *new_node;
  unsigned int seed;
  int num_node,num_trees, equity, control,i, *deep;
  dataType *new;
  double dat;
  outfl = (FILE*)fopen("prova.txt", "w");

  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <numeroNodi> <numeroAlberi> <semeRand>\n", argv[0]);
    exit(1);
  }
  num_node = atoi(argv[1]);
  num_trees = atoi(argv[2]);
  seed = (unsigned)atoi(argv[3]);

  initNode(&root);
  tree = malloc((num_node)*sizeof(node));
  initTree(tree, num_node);
  new = my_rand(seed, num_node);
  root.datum = new[0];
  tree[0]= root;
  printf("\nroot: %f", tree[0].datum);

  for(i=1;i<num_node;i++){
    tree[i].datum = new[i] ;
    insertNode(tree+i, &root);
  }

  print_tree(tree,outfl);
  printf("\nprofondita max: %d", max_depth(tree, num_node));
  printf("\nprofondita media: %lf", ave_depth(tree, num_node));
  printf("\nnumero foglie - profondita\n");

  deep=leaves(tree, num_node);
/*da fare una funzione che stampi su file #numero nodi, prof_max, prof_med per diverse volte num_trees che genero gli alberi*/
/*fai una funzione o un ciclo che stampi su file quante foglie hanno quale profondità*/
}

dataType* my_rand(unsigned int seed, int num_node){
  int i;
  dataType *num;
  num = malloc((num_node)*sizeof(dataType));
  srand(seed);
    for(i=0;i<num_node;i++){
      num[i]=rand();
      num[i]=(num[i]/10000000);
      printf("\nrandom: %f ", num[i]);
    }

  return num;
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

/*Questa funzione trova il numero di foglie e la loro profondità e mette i dati in un array*/

int* leaves(node *tree, int num_node){
  node *my_node;
  int num_leaves,leaves;
  int *deep, i;
  leaves=0;
  deep=malloc(15*sizeof(int));
  for(i=0;i<num_node;i++){
    my_node=&tree[i];
    if(my_node->left==NULL && my_node->right==NULL){
      if(depth_tree(my_node)!=0&& depth_tree(my_node)!=1){
      deep[leaves]=depth_tree(my_node);
      leaves++;
      }
    }
  }
  num_leaves=leaves;
  printf("\nnumero di foglie: %d\n", num_leaves);
  for(i=0;i<leaves;i++){
    printf("%d  -  %d\n", i, deep[i]);
  }
  return deep;
}

void initNode(node *my_node){
  my_node->up = NULL;
  my_node->left = NULL;
  my_node->right = NULL;
}

void initTree(node *tree, int lenght){
  int i;
  for(i=0;i<lenght;i++){
    initNode(tree+i);
    (tree+i)->datum = 0;
  }
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


/*Questa funzione stampa i numeri dell'albero in ordine, da sinistra a destra (verifica che l'albero sia stato riempito correttamente).*/

void print_tree(node *tree, FILE *outfl ){
  if(tree->left!=NULL){
    print_tree(tree->left, outfl);
  }else{
    fprintf(outfl, "%f\n", tree->datum);
  }
  if(tree->right!=NULL){
    print_tree(tree->right, outfl);
  }/*else{
    fprintf(outfl,"%f\n", tree->datum);
    }*/
}

