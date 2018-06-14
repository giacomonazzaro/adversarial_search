COMPILE = g++ -std=c++11 -O3
N = 8

# all: minimax

all: connect_four.cpp
	$(COMPILE) -o connect_four.o connect_four.cpp
	./connect_four.o

allo: minimax.cpp
	$(COMPILE) -o minimax.o minimax.cpp
	# ./minimax.o
	./minimax.o > graph.txt
	dot -Tpng graph.txt > graph.png



# build/csp.o: csp.cpp csp.h
# 	$(COMPILE) -o build/csp.o csp.cpp -c

# clean:
# 	rm -r bin
# 	rm -r build

# $(shell mkdir bin build)
