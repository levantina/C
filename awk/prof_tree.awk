NF && $1 != "#"{
  hmaxmed[$1]+=$2;
  hmedmed[$1]+=$3;  
  varmax[$1]+= $2*$2;
  varmed[$1]+= $3*$3;
  cont[$1]++;
  if(max<=$1) max=$1;
  
}


END{
  print "# < N > <hmaxmed> <sigma> <hmedmed> <sigma>";
  for(i=1;i<=max;i++){
    if(cont[i]){
      avehmax = hmaxmed[i]/cont[i];
      avehmed = hmedmed[i]/cont[i];
      stdmax = varmax[i]/cont[i];
      stdmed = varmed[i]/cont[i];
      print i,"   ",avehmax,"   ",sqrt(stdmax - avehmax*avehmax ),"    ",avehmed,"   ", sqrt(stdmed - avehmed*avehmed);      
    }
    
  }
  
}
