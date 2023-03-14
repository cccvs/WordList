n=30
for (( i = 16; i < n; i++ )); do
  name=$(printf "test%02d.txt"  $[i+1])
  touch "$name"
done
# c/w/n head tail jail -/r