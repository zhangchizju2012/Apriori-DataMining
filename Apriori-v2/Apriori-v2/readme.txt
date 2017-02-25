compile: 
g++ -std=c++11 -O3 -o run main.cpp

use: 
./run data.txt 0.001 0.8 r > rule.txt
./run data.txt 0.001 0.8 f > freq.txt
./run 100.txt 0.1 0.8