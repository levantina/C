$1 != "#" && NF {
  bin[$1] += $2;
  binQuad[$1] += $2^2;
  contatore[$1]++;

  if($1 > max) max = $1;
}

END {
  for(i = 1; i <= max; i++) {
    meanBin += i * (bin[i] / contatore[i]);
    meanBinQuad += i * i (bin[i] / contatore[i]);
  }

  binVar = meanBinQuad - meanBin^2;
  for(i = 1; i <= max; i++)
    if(contatore[i])
      print i, (bin[i] / contatore[i]) * sqrt(binVar), meanBin, sqrt(binVar);
}
