while read line; do
  unbuffer ./SMHasher --test=$line $1 > $line.t_$1_$2.results.txt 2>&1 &
done < names.txt
watch grep '!!!!' *t_$1_$2.results.txt
