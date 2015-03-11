NF && $1 != "#"{
tot += $2;
var[$1]+= $2^2;
media += $1*$2;
bin[$1]+=$2;
dati[$1]++;
if($1>max) max = $1;
}

END{
valmed = media/max;
for(i=0;i<=max;i++){
if(dati[i]){
avebin+= i*(bin[i]/dati[i]);
avevar+= i*i*(var[i]/dati[i]);
}

}
veryvar= avevar - avebin^2;
for(i=1;i<=max;i++){
if(dati[i])
print i,(bin[i]/dati[i])*sqrt(veryvar), avebin, sqrt(veryvar);
}
print valmed;
}