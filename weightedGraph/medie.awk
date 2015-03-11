NF && $1 != "#"{
  L=20;
  passi+=$1;
  dist+=$2;  
  varpassi+= $1*$1;
  vardist+= $2*$2;
  cont++;
 
  
}

END{
  print "#<L> <passi med>  <sigma>   <dist med>  <sigma> ";
      
      passimed = passi/cont;
      distmed = dist/cont;
      stdpassi = varpassi/cont;
      stddist = vardist/cont;
      print L,"  ",passimed,"   ",sqrt(stdpassi - passimed*passimed ),"    ",distmed,"   ", sqrt(stddist - distmed*distmed);      
    
    
  
  
}
