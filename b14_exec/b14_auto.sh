## # Finding number of branches hit [final]
## cat <input file> | grep -A2 "Final" | grep "Branch"
## 
## # Finding number of vectors in the test set
## cat <input file> | grep -A1 "Final" | grep -o "210(.*)"
## 
## # Finding number of vectors required to hit 194 branches
## cat <input file> | grep -B1 -m2 "Branch 194" | grep -o "210(.*)"

for i in $(ls Ab14_$1_$2_512_*);
do
	echo $i
	grep -B2 -m1 "Branch 194" $i | grep -o "210(.*)"
done
