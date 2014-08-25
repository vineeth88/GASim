
# Finding number of branches hit [final]
grep -A2 "Final" Ab14_$1_$2_512_* | grep "Branch"

# Finding number of vectors in the test set
grep -A1 "Final" Ab14_$1_$2_512_* | grep -o "210(.*)"
