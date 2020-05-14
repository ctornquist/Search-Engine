make clear
make all
clear

./indexer ../crawler/test6/ let_dep5
./indextest let_dep5 let_dep5_remake
gawk -f indexsort.awk let_dep5 > let_dep5.sorted
gawk -f indexsort.awk ans_let_dep5 > ans_let_dep5.sorted
gawk -f indexsort.awk let_dep5_remake > let_dep5_remake.sorted
diff --brief let_dep5.sorted ans_let_dep5.sorted
diff --brief ans_let_dep5.sorted let_dep5_remake.sorted