all:
	g++ -c -Wall -std=c++14 order_subordinate.cpp -o order_subordinate.o

clean:
	rm order_subordinate.o