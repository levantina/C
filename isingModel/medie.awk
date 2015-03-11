
BEGIN{
max = 0;
}

NF && $1!="#"{
  
  magn[$1] += $2;
  var_magn[$1] += $3;
  ener[$1] += $4;
  var_ener[$1] += $5;
  count[$1]++;
  if(max<$1) max = $1;

}

END{

for(i=0;i<=max;i++){

 if(count[i]!=0){
    ave_magn = magn[i] / count[i]; 
    ave_ener = ener[i] / count[i];
    ave2_ener = ener[i]*ener[i]/((count[i]-1)*(count[i]-1));   
    ave2_magn = magn[i]*magn[i]/((count[i]-1)*(count[i]-1));
    
    varM = ave2_magn - ave_magn*ave_magn;
    varE = ave2_ener - ave_ener*ave_ener;
    
    print i, "  ", ave_magn, "  ", varM, "  ", ave_ener, "  ", varE;

  }
 }
}