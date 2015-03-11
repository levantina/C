NF && $1 != "#"{
tot += $2;
var[$1] += $2^2;
media += $1*$2;

}
END{
valmed = media/ tot;
for(i=0;i<=tot;i++){
varianza+= (var[i] - valmed^2);

}
print valmed, varianza/sqrt(tot);
}