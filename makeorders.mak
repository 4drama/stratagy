all:
	g++ -c -Wall -std=c++14 orders.cpp -o orders.o

clean:
	rm orders.o