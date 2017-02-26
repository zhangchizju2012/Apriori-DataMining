main.cpp is my code.
data.txt is full 10000 transactions.
1000.txt is top 1000 transactions.
100.txt is top 100 transactions.
apriori is executable file compiled in mac.
apriori_linux executable file compiled in linux.
rule.txt includes all strong association rules for the 10,000 transaction case.
freq.txt includes all frequent itemsets for the 10,000 transaction case.
test_easy.txt is my toy data set when I implemented the algorithm.

There are two executable files in the document. One is called apriori, it is compiled in mac, and it should be able to run in mac OS. The other one is called apriori_linux, this one is compiled in linux, and it should be able to run in any linux operating system. If you want to recompile it, please follow the following instructions.

compile: 
g++ -std=c++11 -O3 -o apriori main.cpp

use: 
./apriori data.txt 0.001 0.8 r > rule.txt
./apriori data.txt 0.001 0.8 f > freq.txt
./apriori data.txt 0.001 0.8 a
./apriori data.txt 0.001 0.8
./apriori 1000.txt 0.01 0.8
./apriori 100.txt 0.1 0.8
