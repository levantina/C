#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define a 1.2
#define MAXXI 10000

typedef struct{
  /*energia del sistema*/
  int E;
  /*temperatura*/
  double T;
  /*magnetizzazione*/
  int MAG;
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

void Boundaries(my_spin *spin, int L, int L2);

void printSpins(my_spin *spin, int L2);

double magnet(my_spin *spin, my_system *sys);

int var_energy(my_spin *spin,int index);

double tot_energy(my_spin *spin, my_system *sys);

void MCS_single_spin_flip(my_spin *spin, my_system *sys);



int main(int argc, char *argv[]){

  FILE *out;
  FILE *ave_out;
  unsigned int inizio;
  unsigned int seed;
  unsigned int iteractions;
  int num_spin,num_edge,i,j,L,L2;
  my_spin *spin;
  my_system *sys;
  int my_rand,count,k=0;
  double *Edata,*Mdata,ave,avem,Evar,Mvar;
  double meas_time=1;
  i=0;
  
  printf("* Modello di Ising - Grafo planare TORO*\n");
  sys = (my_system*)calloc(1,sizeof(my_system));
  
  if(argc!=6){
    printf("\nERRORE\nUtilizzo: %s <Inizio Hot/Cold 0/1> <numeroSpinLato> <Temperatura> <Iterazioni> <semeRand>\n", argv[0]);
    exit(1);
  }

  out = (FILE*)fopen("dat.txt","w");
  if(out==NULL)
     my_error("Impossibile scrivere su file.\n");
  ave_out = (FILE*)fopen("ave_dat.txt","w");
  if(ave_out==NULL)
     my_error("Impossibile scrivere su file.\n");
  
  inizio = (unsigned)atoi(argv[1]);
  num_spin = atoi(argv[2]);  
  if(num_spin <=0) my_error("Il numero di spin dev'essere > 0.");
  sys->T = (double) atof(argv[3]);
  if(sys->T <0) my_error("La temperatura dev'essere > 0.");
  iteractions = (unsigned) atoi(argv[4]);
  seed = (unsigned)atoi(argv[5]);
  srand(seed);
  L2 = num_spin*num_spin;
  L = num_spin;
  sys->size = L2;
  sys->E = 0;
  sys->MAG = 0;
  
  spin = (my_spin*)calloc(L2,sizeof(my_spin));
  Edata = (double*)calloc(iteractions+1,sizeof(double)); 
  Mdata = (double*)calloc(iteractions+1,sizeof(double));

  
  createSpins(spin, L, L2);
  
  /*printSpins(spin, L2);*/ /*controllo*/
  printf("energia iniziale %lf magn iniziale %lf\n",tot_energy(spin,sys), magnet(spin,sys));
  fprintf(out,"#<Tempo> <Magnetizzazione> <Energia> \n#(NumeroSpin = %d Temperatura = %lf Iterazioni = %d)\n",sys->size, sys->T, iteractions);
  fprintf(ave_out,"#<Tempo> <MagnetMediaABlocchi> <var> <EnergiaMediaABlocchi> <var> \n#(NumeroSpin = %d Temperatura = %lf Iterazioni = %d)\n",sys->size, sys->T, iteractions);  
   
  for(k=0;k<MAXXI;k++){ 
    i=0;
    meas_time =1;
    ave = 0;
    avem = 0;
    count = 0;
    Evar = 0;
    Mvar = 0;
    if(inizio==0){
      /*Inizializzazione spin a Caldo*/
      HotInitSpins(spin, L);
    }
    if(inizio==1){
      ColdInitSpins(spin, L);
      /*inizalizza a freddo*/
    }
  
    Edata[0]= tot_energy(spin,sys);
    Mdata[0] = magnet(spin,sys);
  
    fprintf(out,"%d\t%lf\t%lf\n", i, Mdata[0], Edata[0]);
    fprintf(ave_out,"%d\t%lf\t%lf\t%lf\t%lf\n", i, Mdata[0], 0.0, Edata[0], 0.0);
    
    for(i=1;i<=iteractions;i++){
      ave = 0;
      avem = 0;
      count = 0;
      Evar = 0;
      Mvar = 0;
    
      MCS_single_spin_flip(spin, sys);
      Edata[i]= (double)sys->E/(double)(sys->size);
      Mdata[i] = (double)sys->MAG/(double)sys->size;
   
      fprintf(out,"%d\t%lf\t%lf\n",(i),(double)sys->MAG/(double)sys->size, (double)sys->E/(double)(sys->size));
    
      if(i==(int)meas_time){
      
         for(j=(int)meas_time;j>(int)meas_time/a-1;j--){
	   ave+=Edata[j];
	   Evar+=(Edata[j]*Edata[j]);
           avem+=Mdata[j];
           Mvar+=(Mdata[j]*Mdata[j]);
	   count++;
         }
         if(count==1)count++;
         Evar = ((Evar/(double)(count-1)) - ((ave*ave)/(double)((count-1)*(count-1))));
         Mvar = ((Mvar/(double)(count-1)) - ((avem*avem)/(double)((count-1)*(count-1))));
         ave = ave/(double)count;
         avem = avem/(double)count;
         Evar = sqrt(fabs(Evar));
         Mvar = sqrt(fabs(Mvar));

         fprintf(ave_out,"%d\t%lf\t%lf\t%lf\t%lf\n",i,avem,Mvar,ave,Evar);
       
         meas_time = meas_time*a+1;
      }
    }
  fprintf(ave_out,"\n");
  fprintf(out,"\n");
  }
  printf("energia finale = %lf sys->E =%lf sys->MAG = %lf magn fin = %lf\n", (double)tot_energy(spin,sys),(double)sys->E,(double)sys->MAG/(double)sys->size,magnet(spin,sys));
  
  /*printSpins(spin, L2);*/
  /*puo' far comodo come controllo*/

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
    
    spin[i].value = extractRandSpin();

  }
  
  
}

/*inizializzo il sistema a freddo (spin tutti allineati)*/

void ColdInitSpins(my_spin *spin,int L){
 int i, end;
 end = L*L;
  
  for(i=0;i<end;i++){

    spin[i].value = 1;

  }
  
  
}

void createSpins(my_spin *spin, int L, int L2){
  int i;
/*grafo con condizioni periodiche al contorno*/
 for(i=0;i<L2;i++){
   spin[i].neigh = (int*)calloc(6,sizeof(int));
   spin[i].deg = 4;
   spin[i].neigh[0] = (i+1)%L2;
   spin[i].neigh[1] = (i+L2-1)%L2;
   spin[i].neigh[2] = (i+L)%L2;
   spin[i].neigh[3] = (i+L2-L)%L2;
   if(i%L == 0) spin[i].neigh[1] = i+L-1;
   if((i+1)%L == 0) spin[i].neigh[0] = i-L+1;
   
 }
}




void Boundaries(my_spin *spin, int L, int L2){
  int i;
/*grafo con condizioni periodiche al contorno*/
 for(i=0;i<L2;i++){
   spin[i].deg = 4;
   spin[i].neigh[0] = (i+1)%L2;
   spin[i].neigh[1] = (i+L2-1)%L2;
   spin[i].neigh[2] = (i+L)%L2;
   spin[i].neigh[3] = (i+L2-L)%L2;
   if(i%L == 0) spin[i].neigh[1] = i+L-1;
   if((i+1)%L == 0) spin[i].neigh[0] = i-L+1;
   
 }
 
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
  sum=0;
  
  for(i=0;i<sys->size;i++){
    sum += spin[i].value;
    
  }
  
  sys->MAG = sum;

  return (double)sum / (double)sys->size;

}



int var_energy(my_spin *spin, int index){
  int i,sum=0;
  for(i=0;i<spin[index].deg;i++){
    sum += spin[spin[index].neigh[i]].value; 
  }
  return 2* spin[index].value* sum;
}


double tot_energy(my_spin *spin, my_system *sys){
  int i,j;
  int en = 0;

  for(i=0;i<sys->size;i++){
    for(j=0;j<spin[i].deg;j++){

    en -= spin[i].value *spin[spin[i].neigh[j]].value;
    }
 
  }
  
 
  sys->E = en/2;
  return (double)en/(2*(double)sys->size);
  
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

  int i, num, dE;

  for(i=0;i<sys->size;i++){

        /*random = (double)rand()*(sys->size)/(double)RAND_MAX;
        num = (int)random;*/
    num = i; /*eventuale scelta in sequenza*/
     
    dE = var_energy(spin,num);

    if((dE<=0)||(rand()<= RAND_MAX* exp((double)-dE/(double)sys->T))){
	  
          spin[num].value = (- spin[num].value);
	  sys->MAG += (2*spin[num].value);
	  sys->E += dE;
    }
  }
}
