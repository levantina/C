NF && $1 != "#"{
  h[$1]+=$2;
  cont[$1]++;
  if(max<=$1) max=$1;
  
}

(i<max){
  i++;
  if(cont[i]){
    hmed[i]=h[i]/cont[i];
    
  }
}

NF && $1!= "#" {
  var[$1]+=($2-hmed[$1])^2;
  
}

END{
  print "# < N > <hmaxmed> <sigma>";
  for(j=1;j<=max;j++){
    if(cont[j]){
      var[j]=var[j]/cont[j];
      print j,"  ", hmed[j],"  ",sqrt( var[j]);
      
    }
    
  }
  
}
