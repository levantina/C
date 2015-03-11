#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct{
  /*energia del sistema*/
  double E;
  /*temperatura*/
  double T;
  /*magnetizzazione*/
  double MAG;
  /*taglia del sistema*/
  int size;

}my_system;

struct all_spins{
  int *neigh;
  int deg;
  int value;
};
typedef struct all_spins my_spin;


void my_error(char *err);

int extractRandSpin();

void HotInitSpins(my_spin *spin, int L);

void ColdInitSpins(my_spin *spin,int L);

void createSpins(my_spin *spin, int L, int L2);

void printSpins(my_spin *spin, int L2);

double magnet(my_spin *spin, my_system *sys);

double var_energy(my_spin *spin,int index);

int tot_energy(my_spin *spin, my_system *sys);

void MCS_single_spin_flip(my_spin *spin, my_system *sys);



int main(int argc, char *argv[]){

  FILE *out;
  unsigned int seed;
  unsigned int iteractions;
  int num_spin,num_edge,i,j,L,L2;
  my_spin *spin;
  my_system *sys;
  int my_rand;
  double magn;
  i=0;
  
  printf("* Modello di Ising - Grafo planare TORO*\n");
  sys = (my_system*)calloc(1,sizeof(my_system));
  
  if(argc!=6){
    printf("\nERRORE\nUtilizzo: %s <Outputfile> <numeroSpinLato> <Temperatura> <Iterazioni> <semeRand>\n", argv[0]);
    exit(1);
  } 

  out = (FILE*)fopen(argv[1],"w");
  if(out==NULL)
     my_error("Impossibile scrivere su file.\n");
  
  num_spin = atoi(argv[2]);  
  if(num_spin <=0) my_error("Il numero di spin dev'essere > 0.");
  sys->T = (double) atoi(argv[3]);
  if(sys->T <=0) my_error("La temperatura dev'essere > 0.");
  iteractions = (unsigned) atoi(argv[4]);
  seed = (unsigned)atoi(argv[5]);
  srand(seed);
  L2 = num_spin*num_spin;
  L = num_spin;
  sys->size = L2;
  sys->E = 0;
  sys->MAG = 0;
  
  spin = (my_spin*)calloc(L2,sizeof(my_spin));
  
  /*Inizializzazione spin a Caldo*/
  HotInitSpins(spin, L);
   /*ColdInitSpins(spin, L); */
  /*inizalizza a freddo*/
  createSpins(spin, L, L2);
  
  /*printSpins(spin, L2);*/ /*controllo*/
  magn = magnet(spin, sys);
  /*sys->MAG = magn;*/
  sys->E = tot_energy(spin,sys);
  printf("energia iniziale %lf magn iniziale %lf\n",sys->E, sys->MAG/sys->size);
  fprintf(out,"#<Tempo> <Magnetizzazione> <Energia> (NumeroSpin = %d Temperatura = %lf)\n",sys->size,sys->T);
  
  fprintf(out,"%d\t%lf\t%lf\n",i,(double)sys->MAG/(double)sys->size, sys->E/sys->size);
  for(i=0;i<iteractions;i++){
    MCS_single_spin_flip(spin, sys);
    fprintf(out,"%d\t%lf\t%lf\n",(i+1),(double)sys->MAG/(double)sys->size, sys->E/sys->size);
  }
  printf("energia finale = %lf sys->E =%lf sys->MAG = %lf magn fin = %lf\n", (double)tot_energy(spin,sys),(double)sys->E,(double)sys->MAG/(double)sys->size,magnet(spin,sys));
  
  printSpins(spin, L2);

}



void my_error(char *err){
  printf("\nERRORE: ");
  printf(" %s\n", err);
  exit(1);

}

int extractRandSpin(){
  double num;

  num = (double)rand()/(double)RAND_MAX;
  
  if(num>=0.5) 
    return 1;
  
  if(num<0.5) 
    return -1;
  
}


/*inizializzo il sistema a caldo (random)*/

void HotInitSpins(my_spin *spin, int L){
  int i, end;
  end = L*L;

  for(i=0;i<end;i++){
    spin[i].neigh = (int*)calloc(6,sizeof(int));
    spin[i].value = extractRandSpin();
    spin[i].deg = 0;
  }
  
  
}

/*inizializzo il sistema a freddo (spin tutti allineati)*/

void ColdInitSpins(my_spin *spin,int L){
 int i, end;
 end = L*L;
  
  for(i=0;i<end;i++){
    spin[i].neigh = (int*)calloc(6,sizeof(int));
    spin[i].value = 1;
    spin[i].deg = 0;
  }
  
  
}


void createSpins(my_spin *spin, int L, int L2){
  int i;

 for(i=0;i<L2-1;i++){
   if(i<=L-1){
     /*grafo con condizioni periodiche al contorno*/
      spin[i].neigh[spin[i].deg] =  L2-L+i;
      ++spin[i].deg;
      spin[L2-L+i].neigh[spin[L2-L+i].deg] = i;
      ++spin[L2-L+i].deg;
    }
    if(i<=L2-1-L){
      if((i+1)%L){
	spin[i].neigh[spin[i].deg] =  i+1;
	++spin[i].deg;
	spin[i+1].neigh[spin[i+1].deg] = i;
	++spin[i+1].deg;
	
      }else{
	spin[i].neigh[spin[i].deg]= i-L+1; /*condizione di connessione per avere un toro*/
	++spin[i].deg;
	spin[i-L+1].neigh[spin[i-L+1].deg] = i;
	++spin[i-L+1].deg;
      }
      
      spin[i].neigh[spin[i].deg] = i+L;
      ++spin[i].deg;
      spin[i+L].neigh[spin[i+L].deg] =i;
      ++spin[i+L].deg;
    }else{
      spin[i].neigh[spin[i].deg]= i+1;
      ++spin[i].deg;
      spin[i+1].neigh[spin[i+1].deg] = i;
      ++spin[i+1].deg;
     
    }
  }

  /*aggiungo l'ultima connessione fuori dal ciclo*/
  
  spin[L2-1].neigh[spin[L2-1].deg] = L2-L;
  ++spin[L2-1].deg;
  spin[L2-L].neigh[spin[L2-L].deg] = L2-1;
  ++spin[L2-L].deg;
  

}


void printSpins(my_spin *spin, int L2){
  int i, j;

  for(i=0;i<L2;i++){
    printf("Elemento %d (%d)->",i, spin[i].value);
    for(j=0;j<spin[i].deg;j++){
      printf("%d (%d) ",spin[i].neigh[j], spin[spin[i].neigh[j]].value);
    }
    printf("\n");
  }
}


double magnet(my_spin *spin, my_system *sys){
  int sum,i;
  double m = 0.0;
  sum=0;
  
  for(i=0;i<sys->size;i++){
    sum += spin[i].value;
    
  }
  
  sys->MAG = (double)sum;
  m = (double)sum / (double)sys->size;

  return m;

}



double var_energy(my_spin *spin, int index){
  double dE=0.0;
  int i,sum=0;
  for(i=0;i<spin[index].deg;i++){
    sum += spin[index].value*spin[spin[index].neigh[i]].value; 
  }
  dE = (double) sum*(double) spin[index].value;
  
  return (double)2*(double)sum;
}


int tot_energy(my_spin *spin, my_system *sys){
  int i,j;
  int en = 0;
  int tot_en; 
  tot_en = 0;

  for(i=0;i<sys->size;i++){
    for(j=0;j<spin[i].deg;j++){
   /* en += spin[spin[i].neigh[j]].value;*/
    en -= spin[i].value *spin[spin[i].neigh[j]].value;
    }
    /*tot_en += spin[i].value * en;*/
  }
  
  /*return -tot_en;*/
  sys->E = en;
  return en;
  
}



double minimum(double first, double second){
  if(first <= second){
    return first;
  }
  else{
    return second;
  }
}


void MCS_single_spin_flip(my_spin *spin, my_system *sys){

  int i, num;
  double prob, random, min, my_rand;
  double dE=0.0;

    for(i=0;i<sys->size;i++){

        /*random = (double)rand()*(sys->size)/(double)RAND_MAX;
        num = (int)random;*/
        num = i; /*eventuale scelta in sequenza*/
     
        dE = var_energy(spin,num);

        my_rand = (double)rand()/(double)RAND_MAX;
        prob = exp(-dE/(double)sys->T);
      
	if((dE<=0)||(my_rand<= prob)){
	
	  sys->MAG -= 2*(spin[num].value);
	  sys->E += dE;
          /*sys->E = tot_energy(spin,sys);
          sys->MAG = magnet(spin,sys);*/
	  spin[num].value = - spin[num].value;
      
	}
    }

}
