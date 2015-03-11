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
void initNodeRand(node *my_node);
void insertNode(node *new, node *old);
void print_tree(node *tree, FILE *sortfl);
void histogram_leaves(node *tree, int num_node, FILE *hfl);


int main(int argc, char *argv[]){

  FILE *outfl, *hfl, *sortfl;
  node my_node, root, *tree, *new_node;
  unsigned int seed;
  int num_node, iteractions, max, control, i, *deep, j;
  int maxdepth =0, N;
  dataType *new;
  double dat,avedepth=0;

  outfl = (FILE*)fopen("profondita_log.txt", "w");
  hfl = (FILE*)fopen("isto_foglie.txt","w");
  sortfl = (FILE*)fopen("sorted_tree.txt", "w");

  if(argc!=4){
    printf("\nERRORE\nUtilizzo: %s <maxNumeroNodi> <Iterazioni> <semeRand>\n", argv[0]);
    exit(1);
  }
  num_node = (unsigned)atoi(argv[1]);
  iteractions = (unsigned)atoi(argv[2]);
  seed = (unsigned)atoi(argv[3]);
  srand(seed);

  tree = malloc(num_node*sizeof(node));
  fprintf(outfl, "# N(log)     <prof max>    <prof media>\n");
  fprintf(hfl, "# <Profondita'> <NumeroFoglie> (NumeroNodi: %d)\n", num_node);
  max = num_node;
 
  for(j=1;j<=iteractions;j++){
    N=1;
    if(j!=1)fprintf(outfl,"\n");

    for(num_node=1;num_node<=max;num_node++){
      initNodeRand(&tree[0]);

      for(i=1;i<num_node;i++){
	initNodeRand(tree+i);
	insertNode(tree+i, &tree[0]);

      }
      maxdepth = max_depth(tree, num_node);
      avedepth = (double)ave_depth(tree, num_node);
      if(i==N){
	fprintf(outfl, "%d\t   %d\t    %f\t\n", num_node, maxdepth, avedepth);
	N=(int)(N*1.2)+1;
      }
    }
    histogram_leaves(tree, max, hfl);
  }

/*Se chiamo print_tree() solo qui stampo solo l'utimo albero generato, pi˘ che altro mi serve come controllo*/
  print_tree(tree, sortfl);


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

/*Questa funzione calcola la profondit√† media dei nodi dell'albero*/

double ave_depth(node *tree, int num_node){
  int i;
  double ave=0;
  for(i=0;i<num_node;i++){
    ave+=depth_tree(tree+i);
  }
  ave =(double) ave/num_node;
  return ave;
}


void histogram_leaves(node *tree, int num_node, FILE *hfl){
int tot, num_leaves,i,j, num;
int *leaves;
node *temp;
leaves = (unsigned int*)calloc(num_node+1,sizeof(int));
  for(i=0;i<num_node;i++){
    leaves[i]=0;
  }
  num = 0;
  num_leaves = 0;
  tot = 0;
  for(j=0;j<num_node;j++){
    temp = tree+j;
    if((temp->left==NULL)&&(temp->right==NULL)){
      num = depth_tree(temp);
      leaves[num]++;
      num_leaves++;
    }
  }
  temp = &tree[0];
  tot = max_depth(temp, num_node);


  for(i=0;i<=tot;i++){
    if(leaves[i]){
      fprintf(hfl, "%d\t   %d\t\n", i, leaves[i]);
    }
  }
  fprintf(hfl,"\n");
}

/*Questa funzione stampa l'albero ordinato*/

void print_tree(node *tree, FILE *sortfl){
if(tree==NULL)return;
  if(tree->left!=NULL){
    print_tree(tree->left, sortfl);
  }
  fprintf(sortfl,"%f\t\n", tree->datum);
  if(tree->right!=NULL){
    print_tree(tree->right,sortfl);
  }
}
